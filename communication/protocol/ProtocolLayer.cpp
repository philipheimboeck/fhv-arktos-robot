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
    }
    free(out);

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
    pdu_t* out = (pdu_t*) malloc(sizeof(pdu_t));
    out->message = new_msg;
    out->length = new_size;

    return out;
}

/** Transport Layer **/

pdu_t* TransportLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    memcpy(out->message, in->message, in->length);
    return out;
}

void TransportLayer::decompose_pdu(pdu_t* in) {
    return;
}

bool TransportLayer::send(pdu_t* pdu) {
    bool result = false;

    // Compose the message
    pdu_t* out = this->compose_pdu(pdu);
    if (out != NULL) {
        result = this->bluetooth->bluetooth_write(pdu->message, pdu->length);
    }
    free(out);

    return result;
}


bool TransportLayer::receive(pdu_t* pdu) {
    ssize_t size = bluetooth->bluetooth_read(pdu->message, pdu->length);

    if(size > 0) {
        pdu->length = (size_t) size;
    }

    return size > 0;
}

/** Session Layer **/


pdu_t* SessionLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    memcpy(out->message, in->message, in->length);
    return out;
}

void SessionLayer::decompose_pdu(pdu_t* in) {
    return;
}


/** Presentation Layer **/


pdu_t* PresentationLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    memcpy(out->message, in->message, in->length);
    return out;
}

void PresentationLayer::decompose_pdu(pdu_t* in) {
    return;
}


/** Application Layer **/


pdu_t* ApplicationLayer::compose_pdu(pdu_t* in) {
    pdu_t* out = copy_pdu(in, 0);
    memcpy(out->message, in->message, in->length);
    return out;
}

void ApplicationLayer::decompose_pdu(pdu_t* in) {
    return;
}
