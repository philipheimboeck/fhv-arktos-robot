//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_BLUETOOTH_H
#define ARCTOS_ROBOT_BLUETOOTH_H

#include <stdio.h>
#include "../general.h"

#define BLUETHOOTH_BUFFER_SIZE 128

class Bluetooth {

protected:
    int fd;

public:
    Bluetooth(serial_port_options_t* options);

    ssize_t bluetooth_read(char* buffer, size_t buffer_size);

    bool bluetooth_write(const char* data, size_t data_length);
};



#endif //ARCTOS_ROBOT_BLUETOOTH_H
