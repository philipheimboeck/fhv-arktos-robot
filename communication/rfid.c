//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include "rfid.h"
#include <unistd.h> // for read & write functions
#include <fcntl.h> // for open related function
#include <sys/select.h> // fd_set functions


int rfid_init(serial_port_options_t* options) {
	// file descriptor for the port
	int fd;
	if ((fd = open(options->port_name, O_RDONLY | O_NOCTTY)) == -1) {
		error_message("RFID: Unable to open port");
	}

	// Get the current attributes for the port
	struct termios port_attrs;
	if(tcgetattr(fd,&port_attrs) != 0)
	{
		error_message("RFID: Unable to get port attributes");
		close(fd);
		return -1;
	}

	// Set the baud rates
	if (cfsetispeed(&port_attrs, options->speed) != 0 || cfsetospeed(&port_attrs, options->speed) != 0)
	{
		error_message("RFID: Unable to set baud rate");
		close(fd);
		return -1;
	}

	// Enable the receiver and set local mode
	// Set the new attributes for the port
	port_attrs.c_cflag |= (CLOCAL|CREAD);
	port_attrs.c_cflag &= ~CSIZE; // Mask the character size bits
	port_attrs.c_cflag |= CS8;    // Select 8 data bits
	port_attrs.c_cc[VMIN] = RFID_LEN; // Read returns only with RFID_LEN

	if(tcsetattr(fd, TCSAFLUSH, &port_attrs) != 0) {
		error_message("RFID: Unable to set port attributes");
		close(fd);
		return -1;
	}

	return fd;
}

int rfid_read(int fd, char* buffer, size_t buffer_size) {
    // init input set
    fd_set input;
    FD_ZERO(&input);
    FD_SET(fd, &input);

    // select call
    if(select(fd+1, &input, NULL, NULL, NULL) == -1) {
        error_message("RFID: Selection timeout");
        return -1;
    }

    // check for input
    if (FD_ISSET(fd, &input)) {
    	// read input and save it into buffer
        if(read(fd, buffer, buffer_size) < 0) {
        	write(2, "An error occurred in the read.\n", 31);
        }

        // start byte and stop byte have to be correct
        if (buffer[0] != START_BYTE && buffer[buffer_size - 1] != STOP_BYTE) {
        	return -1;
        }
    } else {
    	// nothing set
    	return -1;
    }

    return 1;
}
