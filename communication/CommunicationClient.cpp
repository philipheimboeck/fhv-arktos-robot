/*
 * CommunicationClient.cpp
 *
 *  Created on: 19.12.2015
 *      Author: phili
 */

#include "CommunicationClient.h"

#include <stddef.h>
#include <cstdlib>
#include <cstring>

#include "bluetooth.h"
#include "protocol/ProtocolLayer.h"

using namespace communication;

CommunicationClient::CommunicationClient(serial_port_options_t serial_options) : driveCallback(NULL) {
	this->driveCallback = NULL;

	this->connection = new communication::PresentationLayer(
			new communication::SessionLayer(
					new communication::TransportLayer(
							new Bluetooth(&serial_options)
					)
			)
	);
}

CommunicationClient::~CommunicationClient() {
	if(this->connection != NULL) {
		delete this->connection;
	}
}

void CommunicationClient::retrieveData(void) {
	char* bluetooth_buffer = (char*) malloc(BLUETHOOTH_BUFFER_SIZE * sizeof(char));
	communication::pdu_t* bluetooth_data = (communication::pdu_t*) malloc(sizeof(communication::pdu_t));

	// Could reserve data?
	if (bluetooth_buffer > 0 && bluetooth_data > 0) {

		bluetooth_data->message = bluetooth_buffer;
		bluetooth_data->length = BLUETHOOTH_BUFFER_SIZE;

		this->connection->receiveData(bluetooth_data);

		if (bluetooth_data->length > 0) {
			// Bluetooth data received

			communication::tuple_t* data = (communication::tuple_t*) bluetooth_data->message;

			// Execute bluetooth command
			if(strcmp(data->data, "drive") == 0) {

				// Find the values
				char left[4];
				char right[4];
				size_t separator = 0;
				size_t data_length = (bluetooth_data->length - (data->data_start - data->data));

				while(*(data->data_start + separator) != ',' && separator < data_length) {
					++separator;
				}

				if(data->data_start[separator] == ',') {
					// Separator found
					strncpy(left, data->data_start, separator);
					strncpy(right, data->data_start + separator + 1, 4);

					// Drive
					int l = atoi(left);
					int r = atoi(right);

					// Return callback
					if(this->driveCallback != NULL) {
						this->driveCallback(l, r);
					}
				}
			}
		}
	}

	// Free the allocated data
	free(bluetooth_data->message);
	free(bluetooth_data);
}

bool CommunicationClient::sendLocation(location_t* location) {
	bool result = false;
	tuple_t* tuple = (tuple_t*) malloc(sizeof(tuple_t));
	pdu_t* pdu = (pdu_t*) malloc(sizeof(pdu_t));

	if(tuple > 0 && pdu > 0) {
		strcpy(tuple->data, "rfid");
		tuple->data[4] = '\0';
		tuple->data_start = &tuple->data[5];
		memcpy(tuple->data_start, location->id, sizeof(tuple->data) - 6);

		result = this->connection->sendData(pdu);
	}

	if(pdu > 0) {
		free(pdu->message);
		free(pdu);
	}
	return result;
}

void CommunicationClient::setDriveCallback(void (*callback)(int left, int right)) {
	this->driveCallback = callback;
}
