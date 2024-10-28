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

#include <std_msgs/msg/bool.h>

// Variables ------------------------------------------------------------------

/* uRos base instance -> contains node, support and executor */
pico_uros_base_t g_uros_base;

/* ROS timer used for LED blinking */
rcl_timer_t g_timer;

uint g_led_status;

/* ROS publisher to publish LED state */
rcl_publisher_t g_led_status_pub;
std_msgs__msg__Bool g_led_status_msg;

/* ROS subscription to control LED blinking */
rcl_subscription_t   g_pico_en_blink_subs;
std_msgs__msg__Bool  g_pico_en_blink_msg;

// Pub / Sub / Callbacks ------------------------------------------------------

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    if(g_pico_en_blink_msg.data == true)
    {
        /* Store led status */
        g_led_status_msg.data = g_led_status;

        /* Blink LED */
        gpio_put(PI_PICO_LED_PIN, g_led_status);
        g_led_status = ~g_led_status;
    }
    else
    {
        /* Turn off LED */
        gpio_put(PI_PICO_LED_PIN, 0);
        g_led_status = 0;
    }

    
    /* Publish LED state */
    rcl_publish(&g_led_status_pub, &g_led_status_msg, NULL);
}


void pico_en_blinking_callback(const void *msgin)
{
    /* Convert message to the corrent type */
    const std_msgs__msg__Bool* msg = (const std_msgs__msg__Bool*)msgin;

    /* Copy message data to our variable */
    g_pico_en_blink_msg.data = msg->data;
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

    /* Create timer */
    rclc_timer_init_default2(&g_timer, 
                             &g_uros_base.support,
                             RCL_MS_TO_NS(LED_TIMER_PERIOD_MS),
                             timer_callback,
                             true);

    /* Add timer to executor */
    rclc_executor_add_timer(&g_uros_base.executor, &g_timer);

    /* Create publisher */
    rclc_publisher_init_default(&g_led_status_pub, 
                                &g_uros_base.node,
                                ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), 
                                "pico_led_status");

    /* Create subscription */
    rclc_subscription_init_best_effort(&g_pico_en_blink_subs, 
                                       &g_uros_base.node,
                                       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
                                       "pico_en_blinking");

    /* Register subscription to executor */
    rclc_executor_add_subscription(&g_uros_base.executor, 
                                   &g_pico_en_blink_subs, 
                                   &g_pico_en_blink_msg, 
                                   &pico_en_blinking_callback, 
                                   ON_NEW_DATA);

    /* Spin */
    while (true)
    {
        rclc_executor_spin_some(&g_uros_base.executor, RCL_MS_TO_NS(STARTUP_CONN_TIMEOUT_MS));
    }

    return 0;
}
