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