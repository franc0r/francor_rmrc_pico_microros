/**
 * @file main.h
 * @author Martin Bauernschmitt
 * @brief Example program for using Micro-ROS on Raspberry Pi Pico
 *        based on the example program from the Micro-ROS repository
 *        modified for the RMRC workshop.
 *
 * @version 0.1
 * @date 2024-10-27
 *
 */

#ifndef PICO_RMRC_UROS__
#define PICO_RMRC_UROS__

 // Includes -------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>


#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Defines --------------------------------------------------------------------

#define ROS_NODE_NAME               "pico_node"
#define ROS_NAMESPACE               ""

/* IMPORTANT: Update this number to the number of callback functions
   e.g. one subscription + one timer handle = 2 handles */
#define ROS_NUM_HANDLES           2

#define STARTUP_CONN_TIMEOUT_MS 1000
#define STARTUP_CONN_ATTEMPTS   120

#define PI_PICO_LED_PIN         25
#define PI_PICO_CO2_ADC_PIN     26

#endif /* PICO_RMRC_UROS__ */