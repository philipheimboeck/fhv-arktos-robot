//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ProtocolLayer.h"

/** Protocol Layer **/


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

bool ProtocolLayer::send(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->compose_pdu(pdu);
    if (out != NULL && lowerLayer != NULL) {
        // Pass it to the lower layer
        result = lowerLayer->send(out);

        free(out->message);
        free(out);
    }

    return result;
}

bool ProtocolLayer::receive(pdu_t* pdu) {
    // Receive a message
    bool result = lowerLayer->receive(pdu);

    if (result) {
        // Decompose the message
        lowerLayer->decompose_pdu(pdu);
    }

    return result;
}

pdu_t* ProtocolLayer::copy_pdu(pdu_t* in, size_t increase) {
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

pdu_t* TransportLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }

    return NULL;
}

void TransportLayer::decompose_pdu(pdu_t* in) {
    return;
}

bool TransportLayer::send(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->compose_pdu(pdu);
    if (out != NULL) {
        result = this->bluetooth->bluetooth_write((char*)pdu->message, pdu->length);
    }
    free(out);

    return result;
}


bool TransportLayer::receive(pdu_t* pdu) {
    ssize_t size = bluetooth->bluetooth_read((char*)pdu->message, pdu->length);

    if (size > 0) {
        pdu->length = (size_t) size;
        decompose_pdu(pdu);
    } else {
        pdu->length = 0;
    }

    return size > 0;
}

/** Session Layer **/


pdu_t* SessionLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }
    return NULL;
}

void SessionLayer::decompose_pdu(pdu_t* in) {
    return;
}


/** Presentation Layer **/


pdu_t* PresentationLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }
    return NULL;
}

void PresentationLayer::decompose_pdu(pdu_t* in) {

    if (in->length > 8) {
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


/** Application Layer **/


pdu_t* ApplicationLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    if(out != NULL) {
        memcpy(out->message, in->message, in->length);
        return out;
    }
    return NULL;
}

void ApplicationLayer::decompose_pdu(pdu_t* in) {
    return;
}
