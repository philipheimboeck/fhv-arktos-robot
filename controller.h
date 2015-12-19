//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_CONTROLLER_H
#define ARCTOS_ROBOT_CONTROLLER_H

#include <stddef.h>
#include <sys/types.h>

#include "general.h"
#include "communication/CommunicationClient.h"

typedef struct {
    int (* controller_rfid_init)(serial_port_options_t* options);

    ssize_t (* controller_rfid_read)(int fd, char*, size_t);
    void (*controller_drive_init)(void);
    void (*controller_drive_left)(double);
    void (*controller_drive_right)(double);
} robot_callbacks_t;

typedef struct {
    serial_port_options_t serial_port_options_rfid;
    robot_callbacks_t callbacks;
} robot_options_t;

class Controller {
private:

    /** Variables **/

    int fd_rfid;

    location_t last_location;

    communication::CommunicationClient* client;

    bool shutdown_requested = false;

    /** Methods **/

    int compare_locations(location_t* l1, location_t* l2);

    bool notify_server(location_t* location);

    bool update_location(location_t location);

    /* Callbacks */

    /**
     * Returns a file descriptor
     */
    int (* controller_rfid_init)(serial_port_options_t* options);

    void drive(int left, int right);

    /**
     * Reads the input and saves it into the buffer
     */
    ssize_t (* controller_rfid_read)(int fd, char*, size_t);

    void (*controller_drive_init)(void);
	void (*controller_drive_left)(double);
	void (*controller_drive_right)(double);

public:
    Controller(robot_options_t* options, communication::CommunicationClient* client);

    void runBluetooth();

    void runRFID();

    void shutdown();

};

#endif //ARCTOS_ROBOT_CONTROLLER_H
