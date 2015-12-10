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
    if(lowerLayer != NULL) {
        delete(lowerLayer);
    }
}

bool ProtocolLayer::send(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->compose_pdu(pdu);
    if(out != NULL && lowerLayer != NULL) {
        // Pass it to the lower layer
        result = lowerLayer->send(out);
    }
    free(out);

    return result;
}

bool ProtocolLayer::receive(pdu_t* pdu) {
    bool result = false;

    // Receive a message
    result = lowerLayer->receive(pdu);

    if(result) {
        // Decompose the message
        lowerLayer->decompose_pdu(pdu);
    }

    return result;
}


/** Transport Layer **/

pdu_t* TransportLayer::compose_pdu(pdu_t* in) {
    size_t new_size = sizeof(in);
    pdu_t* out = (pdu_t*) malloc(new_size);
    strncpy(out->message, in->message, in->length);
    out->length = new_size;
    return out;
}

void TransportLayer::decompose_pdu(pdu_t* in) {
    return;
}

bool TransportLayer::send(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->compose_pdu(pdu);
    if(out != NULL) {
        result = this->bluetooth->bluetooth_write(pdu->message, pdu->length);
    }
    free(out);

    return result;
}


bool TransportLayer::receive(pdu_t* pdu) {
    char buffer[BLUETHOOTH_BUFFER_SIZE];

    size_t size = (size_t) bluetooth->bluetooth_read(buffer, BLUETHOOTH_BUFFER_SIZE);

    if(size > 0) {
        strncpy(pdu->message, buffer, size);
        pdu->length = size;
    }
    return false;
}

/** Session Layer **/


pdu_t* SessionLayer::compose_pdu(pdu_t* in) {
    size_t new_size = sizeof(in);
    pdu_t* out = (pdu_t*) malloc(new_size);
    strncpy(out->message, in->message, in->length);
    out->length = new_size;
    return out;
}

void SessionLayer::decompose_pdu(pdu_t* in) {
    return;
}


/** Presentation Layer **/


pdu_t* PresentationLayer::compose_pdu(pdu_t* in) {
    size_t new_size = sizeof(in);
    pdu_t* out = (pdu_t*) malloc(new_size);
    strncpy(out->message, in->message, in->length);
    out->length = new_size;
    return out;
}

void PresentationLayer::decompose_pdu(pdu_t* in) {
    return;
}


/** Application Layer **/


pdu_t* ApplicationLayer::compose_pdu(pdu_t* in) {
    size_t new_size = sizeof(in);
    pdu_t* out = (pdu_t*) malloc(new_size);
    strncpy(out->message, in->message, in->length);
    out->length = new_size;
    return out;
}

void ApplicationLayer::decompose_pdu(pdu_t* in) {
    return;
}
