/**
 * @file main.c
 * @author Martin Bauernschmitt
 * @brief Example program for using Micro-ROS on Raspberry Pi Pico
 *        based on the example program from the Micro-ROS repository
 *        modified for the RMRC workshop.
 *
 * @version 0.1
 * @date 2024-10-27
 *
 */

#include "main.h"
#include "pico_ros_basic.h"

#include <std_msgs/msg/int32.h>

#include "distance_sensor.h"

// Variables ------------------------------------------------------------------

/* uRos base instance -> contains node, support and executor */
pico_uros_base_t g_uros_base;

/* ROS timer used for LED blinking */
rcl_timer_t g_timer;

uint g_led_status;

DistanceSensor* pDistanceSensor;

rcl_publisher_t g_distance_publisher;
std_msgs__msg__Int32 g_distance_msg;

// Pub / Sub / Callbacks ------------------------------------------------------

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    /* Blink LED */
    gpio_put(PI_PICO_LED_PIN, g_led_status);
    g_led_status = ~g_led_status;

    g_distance_msg.data = pDistanceSensor->distance;
    rcl_publish(&g_distance_publisher, &g_distance_msg, NULL);

    pDistanceSensor->TriggerRead();
}

// Main -----------------------------------------------------------------------

/**
 * @brief Main function of uC code
 */
int main()
{
    /* Initialize variables */
    memset((void*)&g_uros_base, 0, sizeof(pico_uros_base_t));
    g_led_status = 0;

    /* Initialize the micro-ROS stack */
    setupROS(&g_uros_base);

    /* Initialize GPIOs of pi pico */
    gpio_init(PI_PICO_LED_PIN);
    gpio_set_dir(PI_PICO_LED_PIN, GPIO_OUT);
    gpio_put(PI_PICO_LED_PIN, g_led_status);

    //pio_gpio_init(pio0, 5);
    //pio_gpio_init(pio0, 6);

    /* Create timer */
    rclc_timer_init_default2(&g_timer, 
                             &g_uros_base.support,
                             RCL_MS_TO_NS(LED_TIMER_PERIOD_MS),
                             timer_callback,
                             true);

    /* Add timer to executor */
    rclc_executor_add_timer(&g_uros_base.executor, &g_timer);

    DistanceSensor hcsr04{pio0, 0, 5};
    pDistanceSensor = &hcsr04;

    /* Create distance publisher */
    rclc_publisher_init_default(&g_distance_publisher,
                                &g_uros_base.node,
                                ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
                                "distance");

    /* Spin */
    while (true)
    {
        rclc_executor_spin_some(&g_uros_base.executor, RCL_MS_TO_NS(STARTUP_CONN_TIMEOUT_MS));
    }

    return 0;
}
