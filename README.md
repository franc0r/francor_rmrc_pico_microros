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
