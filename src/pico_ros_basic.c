/**
 * @file pico_ros_basic.c
 * @author Martin Bauernschmitt
 * @brief Basic functions for ROS setup.
 *
 * @version 0.1
 * @date 2024-10-27
 *
 */

#include "main.h"
#include "pico_ros_basic.h"
#include "pico_uart_transports.h"

/**
 * @brief This function initializes the micro-ROS stack 
 */
void setupROS(pico_uros_base_t* p_uros_base)
{
    /* Setup custom transport via serial interface */
    rmw_uros_set_custom_transport(
		true,
		NULL,
		pico_serial_transport_open,
		pico_serial_transport_close,
		pico_serial_transport_write,
		pico_serial_transport_read
	);

    /* Get the default allocator -> reserves memory if necessary*/
    p_uros_base->allocator = rcl_get_default_allocator();

    /* Try to connect to host */
    rcl_ret_t ret = rmw_uros_ping_agent(STARTUP_CONN_TIMEOUT_MS, STARTUP_CONN_ATTEMPTS);

    if (ret != RCL_RET_OK)
    {
        // Force piPico reset
        reset_usb_boot(0, 0);
    }

    /* Connection to host possible -> initialize uRos stack */

    /* Initialize the micro-ROS */
    rclc_support_init(&p_uros_base->support, 0, NULL, &p_uros_base->allocator);

    /* Initialize the node */
    rclc_node_init_default(&p_uros_base->node, ROS_NODE_NAME, ROS_NAMESPACE, &p_uros_base->support);

    /* Create an executor */
    rclc_executor_init(&p_uros_base->executor, &p_uros_base->support.context, ROS_NUM_HANDLES, &p_uros_base->allocator);
}