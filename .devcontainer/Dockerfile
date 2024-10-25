FROM ubuntu:22.04

RUN apt-get update \
    && apt-get install -y \
    sudo cmake doxygen wget bzip2 \
    git python3 nano g++ \
    && apt-get clean

RUN useradd -u 1000 -ms /bin/bash vscode \
    && usermod -aG sudo vscode  \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
    && echo "export PICO_SDK_PATH=/workspaces/rmrc_pico_microros/external/pico-sdk" >> /home/vscode/.bashrc 

WORKDIR /opt/

RUN wget -O /opt/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
    && tar -xvjf gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
    && rm gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
    && echo "export PATH=$PATH:/opt/gcc-arm-none-eabi-9-2020-q2-update/bin" >> /home/vscode/.bashrc

# Set the user as the default user
USER vscode

# Set the working directory
WORKDIR /home/vscode/