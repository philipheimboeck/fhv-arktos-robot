cmake_minimum_required(VERSION 3.2)
project(arctos_robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp robot.cpp robot.h communication/serial.cpp communication/serial.h general.cpp general.h includes/DMCC/DMCC.cpp Controller.cpp Controller.h communication/Bluetooth.cpp communication/Bluetooth.h communication/rfid.cpp communication/rfid.h communication/protocol/protocolLayer.cpp communication/protocol/protocolLayer.h communication/protocol/ProtocolLayer.cpp communication/protocol/ProtocolLayer.h communication/TcpClient.cpp communication/TcpClient.h)
add_executable(arctos_robot ${SOURCE_FILES})