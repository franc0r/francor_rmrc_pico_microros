/**
 * @file pico_ros_basic.h
 * @author Martin Bauernschmitt
 * @brief Basic functions for ROS setup.
 *
 * @version 0.1
 * @date 2024-10-27
 *
 */

 #ifndef PICO_ROS_BASIC_H__
 #define PICO_ROS_BASIC_H__

 // Structs --------------------------------------------------------------------

/**
 * @brief Struct for the Micro-ROS stack
 */
typedef struct {
    rcl_allocator_t allocator;
    rclc_support_t  support;
    rcl_node_t      node;
    rclc_executor_t executor;
} pico_uros_base_t;

// Functions ------------------------------------------------------------------

void setupROS(pico_uros_base_t* p_uros_base);

 #endif /* PICO_ROS_BASIC_H__ */