# Set a variable for commands below
set(PROJECT_NAME OW_Bandit)

# Define your project and language
project(${PROJECT_NAME} C CXX)

set(${PROJECT_NAME}_BOARD nano328)

# Define the sources
set(${PROJECT_NAME}_SRCS OW_Bandit.cpp)

# Define the headers


generate_arduino_library(OW_Bandit_lib
        SRCS OW_Bandit_lib/OW_Bandit_lib.cpp
        MAX17043/MAX17043.cpp
        OneWireSlave/OneWireSlave.cpp
        OneWire/OneWire.cpp
        EE24C32/EE24C32.cpp
        HDRS OW_Bandit_lib/OW_Bandit_lib.h
        MAX17043/MAX17043.h
        OneWireSlave/OneWireSlave.h
        EE24C32/EE24C32.h
        OneWire/OneWire.h
        EEPROM/src/EEPROM.h
        BOARD uno)


# Define the port for uploading code to the Arduino
#set(${PROJECT_NAME}_PORT COM1)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME}
        SRCS OW_Bandit.cpp
        HDRS OW_Bandit.cpp
        LIBS OW_Bandit_lib
        BOARD uno
        )