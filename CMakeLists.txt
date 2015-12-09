cmake_minimum_required(VERSION 3.2)
project(arctos_robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c")

set(SOURCE_FILES main.c robot.c robot.h communication/serial.c communication/serial.h general.c general.h includes/DMCC/DMCC.c controller.c controller.h communication/bluetooth.c communication/bluetooth.h communication/rfid.c communication/rfid.h)
add_executable(arctos_robot ${SOURCE_FILES})