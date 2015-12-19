//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include "ProtocolLayer.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/** Protocol Layer **/

using namespace communication;

ProtocolLayer::ProtocolLayer(ProtocolLayer* lower) {
    this->lowerLayer = lower;
}

ProtocolLayer::ProtocolLayer() : ProtocolLayer(NULL) {
}

ProtocolLayer::~ProtocolLayer() {
    if (lowerLayer != NULL) {
        delete (lowerLayer);
    }
}

bool ProtocolLayer::sendData(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->composePdu(pdu);
    if (out != NULL && lowerLayer != NULL) {
        // Pass it to the lower layer
        result = lowerLayer->sendData(out);

        free(out->message);
        free(out);
    }

    return result;
}

bool ProtocolLayer::receiveData(pdu_t* pdu) {
    // Receive a message
    bool result = lowerLayer->receiveData(pdu);

    if (result) {
        // Decompose the message
        lowerLayer->decomposePdu(pdu);
    }

    return result;
}

pdu_t* ProtocolLayer::copyPdu(pdu_t* in, size_t increase) {
    size_t new_size = in->length + increase;
    char* new_msg = (char*) malloc(new_size * sizeof(char));
    if(new_msg > 0) {
        pdu_t* out = (pdu_t*) malloc(sizeof(pdu_t));

        if(out > 0) {
            out->message = new_msg;
            out->length = new_size;

            return out;
        }
    }

    return NULL;
}

/** Transport Layer **/

pdu_t* TransportLayer::composePdu(pdu_t* in) {
    pdu_t* out = copyPdu(in, 0);

    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }

    return NULL;
}

void TransportLayer::decomposePdu(pdu_t* in) {
    return;
}

bool TransportLayer::sendData(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->composePdu(pdu);
    if (out != NULL) {
        result = this->bluetooth->bluetooth_write((char*)pdu->message, pdu->length);
    }
    free(out->message);
    free(out);

    return result;
}


bool TransportLayer::receiveData(pdu_t* pdu) {
    ssize_t size = bluetooth->bluetooth_read((char*)pdu->message, pdu->length);

    if (size > 0) {
        pdu->length = (size_t) size;
        decomposePdu(pdu);
    } else {
        pdu->length = 0;
    }

    return size > 0;
}

/** Session Layer **/


pdu_t* SessionLayer::composePdu(pdu_t* in) {
    pdu_t* out = copyPdu(in, 0);
    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }
    return NULL;
}

void SessionLayer::decomposePdu(pdu_t* in) {
    return;
}


/** Presentation Layer **/


pdu_t* PresentationLayer::composePdu(pdu_t* in) {
	tuple_t* tuple = (tuple_t*)in->message;
	size_t key_size = strlen(tuple->data);
	size_t data_size = strlen(tuple->data_start);
	size_t package_size = 8 + key_size + data_size;

	char* message = (char*)malloc(package_size*sizeof(char));
	pdu_t* out = (pdu_t*)malloc(sizeof(pdu_t));
	if(message > 0 && out > 0) {
		// Create the message
		std::string key_size_str = std::to_string(key_size);
		std::string data_size_str = std::to_string(data_size);

		strcpy(message, "01");	// Version [0,1]
		strcpy(message+2, key_size_str.c_str()); // Key Size [2,3,4]
		strcpy(message+5, data_size_str.c_str()); // Data Size [5, 6, 7]
		strcpy(message+8, tuple->data); // Key
		strcpy(message+8+key_size, tuple->data_start); // Data

		// Return the new pdu object
		out->message = message;
		out->length = package_size;

		return out;
	}

    return NULL;
}

void PresentationLayer::decomposePdu(pdu_t* in) {

	// Package min length is 10 (8 bytes header, 1 byte key, 1 byte data)
    if (in->length > 10) {
        // Create the different tuples out of the message
        tuple_t* tuple = (tuple_t*) malloc(sizeof(tuple_t));
        if(tuple <= 0) {
            return;
        }

        char* message = (char*)in->message;

        char version[2];
        version[0] = message[0];
        version[1] = message[1];

        char size_key[3];
        size_key[0] = message[2];
        size_key[1] = message[3];
        size_key[2] = message[4];

        char size_data[3];
        size_data[0] = message[5];
        size_data[1] = message[6];
        size_data[2] = message[7];

        int isize_key = atoi(size_key);
        int isize_data = atoi(size_data);

        if (isize_data + isize_key <= 120 && isize_data > 0 && isize_key > 0) {

            // Split and copy the data
            memcpy(tuple->data, message + 8, isize_key);
            tuple->data[isize_key] = '\0';
            tuple->data_start = tuple->data + isize_key + 1;
            memcpy(tuple->data_start, message + 8 + isize_key, isize_data);
            tuple->data[isize_data + isize_data + 1] = '\0';

            // Free the buffer
            free(in->message);

            // Set the new data
            in->message = tuple;
            in->length = (size_t) (isize_data + isize_key + 1);
        }
    }

    return;
}
