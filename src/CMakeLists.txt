# Set a variable for commands below
set(PROJECT_NAME OW_Bandit)

# Define your project and language
project(${PROJECT_NAME} C CXX)

set(${PROJECT_NAME}_BOARD nano328)

# Define the sources
set(${PROJECT_NAME}_SRCS OW_Bandit.cpp)
#set(${PROJECT_NAME}_SRCS MAX17043.cpp)

# Define the headers
#set(${PROJECT_NAME}_HDRS serial.h)
#set(${PROJECT_NAME}_HDRS MAX17043.h)


# Define the port for uploading code to the Arduino
#set(${PROJECT_NAME}_PORT COM1)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})