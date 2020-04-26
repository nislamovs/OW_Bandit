# Set a variable for commands below
set(PROJECT_NAME OW_Bandit)

# Define your project and language
project(${PROJECT_NAME} C CXX)

#set(${PROJECT_NAME}_BOARD mega2560)
set(${PROJECT_NAME}_BOARD mega2560)

# Define the sources
set(${PROJECT_NAME}_SRCS OW_Bandit.cpp)

# Define the headers


generate_arduino_library(OW_Bandit_lib
        SRCS OW_Bandit_lib/OW_Bandit_lib.cpp
        MAX17043/MAX17043.cpp
        OneWireSlave/OneWireSlave.cpp
        OneWire/OneWire.cpp
        EE24C32/EE24C32.cpp
        HC06/HC06.cpp
        signaling/Signaling.cpp
        HDRS OW_Bandit_lib/OW_Bandit_lib.h
        MAX17043/MAX17043.h
        OneWireSlave/OneWireSlave.h
        EE24C32/EE24C32.h
        OneWire/OneWire.h
        HC06/HC06.h
        constants/constants.h
        signaling/Signaling.h
        EEPROM/src/EEPROM.h
        BOARD mega2560)


# Define the port for uploading code to the Arduino
#set(${PROJECT_NAME}_PORT COM1)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME}
        SRCS OW_Bandit.cpp
        HDRS OW_Bandit.h
        LIBS OW_Bandit_lib
        BOARD mega2560)