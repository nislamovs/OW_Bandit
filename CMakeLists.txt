#=============================================================================#
# Author: Nizami Islamovs                                                     #
# Date:   14.10.2018                                                          #
#                                                                             #
# Description: One wire reader / slave                                        #
#                                                                             #
#=============================================================================#
set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain


cmake_minimum_required(VERSION 2.8.5)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(OW_Bandit C CXX)

set(${CMAKE_PROJECT_NAME}_BOARD mega)
set(mega.build.mcu atmega2560)

print_board_list()
print_programmer_list()

#add the example directory into build
add_subdirectory(src)