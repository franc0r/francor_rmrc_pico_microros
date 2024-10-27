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
#include <rcl_interfaces/msg/log.h>

// Variables ------------------------------------------------------------------

pico_uros_base_t g_uros_base;

rcl_timer_t g_timer;

rcl_subscription_t g_led_subs;
std_msgs__msg__Int32 g_led_msg;

rcl_publisher_t    g_co2_pub;
std_msgs__msg__Int32 g_co2_msg;

uint g_led_status;

// Pub / Sub / Callbacks ------------------------------------------------------

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    if(g_led_msg.data == 1)
    {
        gpio_put(PI_PICO_LED_PIN, g_led_status);
        g_led_status = ~g_led_status;
    }
    else
    {
        gpio_put(PI_PICO_LED_PIN, 0);
    }

    /* Publish adc value */
    g_co2_msg.data = (int32_t)adc_read();
    rcl_ret_t ret = rcl_publish(&g_co2_pub, &g_co2_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        /* Error */
    }
}

/**
  * @brief Subscriber callback function
  */
void led_subscriber_callback(const void *msgin)
{
    const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
    
    g_led_msg.data = msg->data;
}

// Main -----------------------------------------------------------------------

/**
 * @brief Main function of uC code
 */
int main()
{
    /* Initialize variables */
    //memset((void*)&g_uros_base, 0, sizeof(pico_uros_base_t));
    g_led_msg.data = 0;
    g_led_status = 0;

    /* Initialize the micro-ROS stack */
    setupROS(&g_uros_base);

    /* Initialize GPIOs of pi pico */
    gpio_init(PI_PICO_LED_PIN);
    gpio_set_dir(PI_PICO_LED_PIN, GPIO_OUT);
    gpio_put(PI_PICO_LED_PIN, g_led_status);

    /* Initialize ADC */
    adc_init();
    adc_gpio_init(PI_PICO_CO2_ADC_PIN);
    adc_select_input(0);

    /* Create timer */
    rclc_timer_init_default2(&g_timer, 
                             &g_uros_base.support,
                             RCL_MS_TO_NS(500),
                             timer_callback,
                             true);

    /* Add timer to executor */
    rclc_executor_add_timer(&g_uros_base.executor, &g_timer);

    /* Create subscriber */
    rclc_subscription_init_best_effort(
        &g_led_subs,
        &g_uros_base.node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_led_ctrl");

    /* Add subscriber to executor */
    rclc_executor_add_subscription(&g_uros_base.executor, 
                                   &g_led_subs, 
                                   &g_led_msg,
                                   &led_subscriber_callback, 
                                   ON_NEW_DATA);

    /* Create publisher */
    rclc_publisher_init_default(
        &g_co2_pub,
        &g_uros_base.node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_co2_sensor");

    /* Spin */
    while (true)
    {
        rclc_executor_spin_some(&g_uros_base.executor, RCL_MS_TO_NS(STARTUP_CONN_TIMEOUT_MS));
    }

    return 0;
}
