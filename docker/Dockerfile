FROM arm:latest

COPY docker/stm32cubeprg-lin-v2-20-0.zip /opt
COPY docker/auto_install.xml /opt
RUN apt-get update \
    && apt-get install -y \
    unzip \
    libusb-1.0-0 \
    libglib2.0-0 \
    python3-full \
    python3-pip \
    pipenv \
    python3-serial \
    python3-parse \
    && rm -rf /var/lib/apt/lists/* 

WORKDIR /opt
RUN unzip /opt/stm32cubeprg-lin-v2-20-0.zip && \
    chmod +x /opt/SetupSTM32CubeProgrammer-2.20.0.linux && \
    /opt/SetupSTM32CubeProgrammer-2.20.0.linux /opt/auto_install.xml

ENV STM32_PROGRAMMER=/usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI
COPY controller /opt/controller
WORKDIR /opt/controller

COPY p3 /source
WORKDIR /source

COPY docker/binaries /binaries
COPY docker/scripts /scripts

ENV STM32_SWD_SN=002C001E3233510D39363634
ENV SERIAL_PORT=/dev/ttyACM0

RUN cmake .