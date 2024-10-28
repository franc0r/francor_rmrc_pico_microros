# Rapid Manufactured Robots Workshop - PiPico MicroROS

This project shall help you to gain a basic understanding of micro-ROS and the raspberry pi pico 
microcontroller. 

This project is based on [micro_ros_raspberrypi_pico_sdk](https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk.git).


# Dependencies

To allow a fast ram up with not caring about tool installation this project uses Visual Studio Code
with a development docker container, which will setup all tools necessary.

To use this project you need:
- [Visual Studio Code](https://code.visualstudio.com/)
- [Docker CE](https://docs.docker.com/engine/install/ubuntu/)

The project contains:
- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [Raspberry Pi Pico micro-ROS](https://github.com/micro-ROS/micro_ros_raspberrypi_pico_sdk)

# Get Started

1. Clone this project to your local hard drive ```git clone https://github.com/franc0r/francor_rmrc_pico_microros.git```
2. Use ```File -> Open Workspace from File...``` and select ```rmrc_pico_microros.code-workspace``` 
3. In Visual Studio Code use ```CTRL + SHIFT + P``` and select ```>Dev Containers: Reopen in container```.
   This will create the container and open the project in it, which may take a few minutes.
4. 