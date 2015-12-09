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
		return 0;
	}

	// Set the baud rates
	if (cfsetispeed(&port_attrs, options->speed) != 0 || cfsetospeed(&port_attrs, options->speed) != 0)
	{
		error_message("RFID: Unable to set baud rate");
		close(fd);
		return 0;
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
		return 0;
	}

	return fd;
}

ssize_t rfid_read(int fd, char* buffer, size_t buffer_size) {
    // init input set
    fd_set input;
    FD_ZERO(&input);
    FD_SET(fd, &input);

    // select call
    if(select(fd+1, &input, NULL, NULL, NULL) == -1) {
        error_message("RFID: Selection timeout");
        return 0;
    }

    // check for input
    int received_bytes;
    if (FD_ISSET(fd, &input)) {
    	// read input and save it into buffer
        if((received_bytes = read(fd, buffer, buffer_size)) < 0) {
        	write(2, "An error occurred in the read.\n", 31);
        }

        // start byte and stop byte have to be correct
        if (buffer[0] != START_BYTE && buffer[buffer_size - 1] != STOP_BYTE) {
        	error_message("RFID: Start or Stop byte is not correct");
        	return 0;
        }
    } else {
    	// nothing set
    	return 0;
    }

    return received_bytes;
}
