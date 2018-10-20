#=============================================================================#
# Author: Nizami Islamovs                                                     #
# Date:   14.10.2018                                                          #
#                                                                             #
# Description: One wire reader / slave                                        #
#                                                                             #
#=============================================================================#
set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain


cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(ArduinoExample C CXX)

print_board_list()
print_programmer_list()

#add the example directory into build
add_subdirectory(src)
#add_subdirectory(src/MAX17043)
#add_subdirectory(src/OneWire)