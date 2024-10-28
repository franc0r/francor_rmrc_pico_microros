# Rapid Manufactured Robots Workshop - PiPico MicroROS

This project shall help you to gain a basic understanding of micro-ROS and the raspberry pi pico 
microcontroller. 

This project is based on [micro_ros_raspberrypi_pico_sdk](https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk.git).


# Dependencies

To allow a fast ram up with not caring about tool installation this project uses Visual Studio Code
with a development docker container, which will setup all tools necessary.

To use this project you need:
- A linux machine (Ubuntu, Debian, Arch, ...)
- [Visual Studio Code](https://code.visualstudio.com/)
- [Docker CE](https://docs.docker.com/engine/install/ubuntu/)

The project contains:
- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [Raspberry Pi Pico micro-ROS](https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk)

# Get Started


## Project Setup & Build

1. Clone this project to your local hard drive ```git clone https://github.com/franc0r/francor_rmrc_pico_microros.git```
2. Use ```File -> Open Workspace from File...``` and select ```rmrc_pico_microros.code-workspace``` 
3. In Visual Studio Code use ```CTRL + SHIFT + P``` and select ```>Dev Containers: Reopen in container```.
   This will create the container and open the project in it, which may take a few minutes.
4. Open a terminal and initialize the submodules via ```git submodule update --init --recursive```
5. Run a test build via ```CTRL + SHIFT + B``` and selection:
   1. ```CMake Build``` Create cmake files
   2. ```Build``` Build binaries
   3. ```Clean``` Removes binaries
   4. ```Clean All``` Removes complete content of build directory

## Updating Pi Pico

The pi pico can be flashed by simply copying the ```pico_rmrc_uros.uf2``` file.
If you connect the pi pico while pressing *BOOTSEL* pin it will show up as hard drive *RPI-RP2*.
Copy the file ```build\pico_rmrc_uros.uf2``` to the location.

After the update the drive will disapear.

## Running micro-ROS on PC

The pi pico is doing nothing after flashing the program. The reason is that we need to run the micro ROS application on the pc
which transfers the data between the pi pico microcontroller and the ros network.

To run the application we use again docker:
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host --name=microros microros/micro-ros-agent:jazzy serial --dev /dev/ttyACM0 -b 115200
```

This will run a docker container which micro ros and connects to the serial port */dev/ttyACM0*

The output shall look like this:
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host --name=microros microros/micro-ros-agent:jazzy serial --dev /dev/ttyACM0 -b 115200                                                                                              
[1730098135.562322] info     | TermiosAgentLinux.cpp | init                     | running...             | fd: 3
[1730098135.562799] info     | Root.cpp           | set_verbose_level        | logger setup           | verbose_level: 4
[1730098143.645352] info     | TermiosAgentLinux.cpp | fini                     | server stopped         | fd: 3
[1730098143.655490] info     | TermiosAgentLinux.cpp | init                     | Serial port not found. | device: /dev/ttyACM0, error 2, waiting for connection...
[1730098144.098112] info     | TermiosAgentLinux.cpp | init                     | running...             | fd: 3
[1730098144.188778] info     | Root.cpp           | create_client            | create                 | client_key: 0x4875BF04, session_id: 0x81
[1730098144.188847] info     | SessionManager.hpp | establish_session        | session established    | client_key: 0x4875BF04, address: 0
[1730098144.198701] info     | ProxyClient.cpp    | create_participant       | participant created    | client_key: 0x4875BF04, participant_id: 0x000(1)
```
*If you dont see this output just disconnect and reconnect the raspberry pi*

Now we will check if we can seen the node. Open a new terminal and connect to the running docker session via:
```bash
# Connect to running docker container
docker exec -it microros /bin/bash  

# Check if you can find the node
ros2 node list

# Output shall show
/pico_node
```

Maybe you have recognized that the LED is blinking. This is because the pi pico as recognized the connection to the ROS network and starts spinning.

# Lesson 01 - Analyzing the code

You need only to care about three files:
- include/main.h: Used for global defines
- src/main.c: Used for main source code
- CMakeLists.txt: Used to add peripheral packages for pico SDK

## main.h

At the top of the main.h you will find a list of common includes necessary to build your firmware.
Add additional include here if necessary.

```C
 // Includes -------------------------------------------------------------------

/* Common includes */
#include <stdio.h>
#include <string.h>

/* micro-ROS related includes */
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>

/* Raspberry Pi Pico related includes */
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
```

The next section is showing you the settings / defines which allow you to easily modify for 
example the blink rate of the LED or the name of the node.

An very **important** setting is ```ROS_NUM_HANDLES```! It defines the number of handles
which are updated by the ros spinner. At the moment we need one handle for the timer function,
which toggles the LED every 500 msec. If we add a subscription for a message we have to 
increment this number to 2. A publisher does not need a handle, because it has no callback
function.

```C
// Defines --------------------------------------------------------------------

#define ROS_NODE_NAME               "pico_node"
#define ROS_NAMESPACE               ""

/* IMPORTANT: Update this number to the number of callback functions
   e.g. one subscription + one timer handle = 2 handles */
#define ROS_NUM_HANDLES           1

#define STARTUP_CONN_TIMEOUT_MS 1000
#define STARTUP_CONN_ATTEMPTS   120

#define LED_TIMER_PERIOD_MS     500

#define PI_PICO_LED_PIN         25
#define PI_PICO_CO2_ADC_PIN     26
```

## main.c

In the main.c you implement your functions like the blinking
LED of the pico via a ROS timer.

At the beginning we again include the necessary files:

```C
#include "main.h"
#include "pico_ros_basic.h"
```

After the include section the global variables
are defined which are visible to every function in the main.c file.

The first variable ```g_uros_base``` structures all main micro ROS based instances.
You will need it if you want to add new subscribers, timers, ... .

The second variable ```g_timer``` is a timer instance, which toggles our LED.

The last variable ```g_led_status``` stores the current state
of the LED (if it is on or off).

```C
// Variables ------------------------------------------------------------------

pico_uros_base_t g_uros_base;

rcl_timer_t g_timer;

uint g_led_status;
```

The next section defines the callback functions. At the moment
you will only find the ```timer_callback```function which toggles our LED. 

```C
// Pub / Sub / Callbacks ------------------------------------------------------

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    /* Blink LED */
    gpio_put(PI_PICO_LED_PIN, g_led_status);
    g_led_status = ~g_led_status;
}
```

The last section is our main function, which is the starting
point for our firmware application.

At the beginning it initializes the variables with the value zero to have a defined starting point.

Via the function ```setupROS()``` we do all the initialization
for micro-ROS. You can look inside this function if you want,
but it is not neccessary, because the setup is all the time the same.

After that we initialize the GPIOs of the raspberry pi pico and
configure the PI_PICO_LED_PIN as output and set the pin to off (g_led_status = 0).

The next the functions add a timer with the period ```LED_TIMER_PERIOD_MS```defined in main.h

Every time we create a timer or a subscription we have to
register it to the executor, which is done via ```rclc_executor_add_timer```. 

After that we just update the spinner of micro ros which handles all the stuff, necessary to toggle the LED and communication with our ros network.

```C

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

    /* Spin */
    while (true)
    {
        rclc_executor_spin_some(&g_uros_base.executor, RCL_MS_TO_NS(STARTUP_CONN_TIMEOUT_MS));
    }

    return 0;
}

```

# Lesson 02 - Add a publisher

In the next lesson lets add a publisher to our application!

**Create a publisher with the topic name "pico_led_status". Which publishs the LED status as a Int32 value every time the LED toggles its state.**

I will not give you the direct solution, only the types and functions
necessary to archive it.

If you need more information ask CoPilot or Google, this will help you a lot ;-)

```C
/* Includes you need: */
#include <std_msgs/msg/bool.h>

/* Types you need */
rcl_publisher_t
std_msgs__msg__Bool

/* Functions you need */
rclc_publisher_init_default()
rcl_publish()

```

To test your firmware you can start the docker containers again, if not already running:

```bash
# Start container
docker run -it --rm -v /dev:/dev --privileged --net=host --name=microros microros/micro-ros-agent:jazzy serial --dev /dev/ttyACM0 -b 115200

# Connect to running docker container
docker exec -it microros /bin/bash  

# Check if you can see the messages on the topic
ros2 topic echo /pico_led_status
```

# Lesson 03 - Add a subscription

In the next lesson lets extend the firmware furthermore:

**Add a subscription with the topic "pico_en_blinking", which enables or disables your LED blinking via a ros message from the host with the std_msgs/Bool type**

```C

/* Includes you need */
#include <std_msgs/msg/bool.h>

/* Types you need */
rcl_subscription_t
std_msgs__msg__Bool

/* Functions you need */
rclc_subscription_init_best_effort()
rclc_executor_add_subscription();

/* Futhermore you need a callback function
   which is called when a message arrives from 
   the ros network */
void pico_en_blinking_callback(const void *msgin);

```

You can use the commands inside your docker container
to publish a message to the network


To test your firmware you can start the docker containers again, if not already running:

```bash
# Start container
docker run -it --rm -v /dev:/dev --privileged --net=host --name=microros microros/micro-ros-agent:jazzy serial --dev /dev/ttyACM0 -b 115200

# Connect to running docker container
docker exec -it microros /bin/bash  

# Publish message
ros2 topic pub /pico_en_blinking std_msgs/msg/Bool  "{data: 0}"

```

# Lesson 04 - Add CO2 sensor via ADC

TBD.

