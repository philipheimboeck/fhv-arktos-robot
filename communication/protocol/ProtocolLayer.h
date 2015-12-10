//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_PROTOCOLLAYER_H
#define ARCTOS_ROBOT_PROTOCOLLAYER_H

#include "../Bluetooth.h"

typedef struct {
    char* message;
    size_t length;
} pdu_t;

class ProtocolLayer {
protected:
    ProtocolLayer* lowerLayer;
    virtual pdu_t* compose_pdu(pdu_t* in) = 0;
    virtual void decompose_pdu(pdu_t* in) = 0;
    pdu_t* copy_pdu(pdu_t* in, size_t increase);

public:
    ProtocolLayer(ProtocolLayer* lower);
    ProtocolLayer();
    virtual ~ProtocolLayer();

    virtual bool send(pdu_t* pdu);
    virtual bool receive(pdu_t* pdu);
};

class ApplicationLayer : public ProtocolLayer {

    virtual pdu_t* compose_pdu(pdu_t* in);

    virtual void decompose_pdu(pdu_t* in);

public:
    ApplicationLayer(ProtocolLayer* lower) : ProtocolLayer(lower) { }
};

class PresentationLayer : public ProtocolLayer {

    virtual pdu_t* compose_pdu(pdu_t* in) override;

    virtual void decompose_pdu(pdu_t* in) override;

public:
    PresentationLayer(ProtocolLayer* lower) : ProtocolLayer(lower) { }
};

class SessionLayer : public ProtocolLayer {

    virtual pdu_t* compose_pdu(pdu_t* in) override;

    virtual void decompose_pdu(pdu_t* in) override;

public:
    SessionLayer(ProtocolLayer* lower) : ProtocolLayer(lower) { }
};

class TransportLayer : public ProtocolLayer {
private:
    Bluetooth* bluetooth;

protected:
    virtual pdu_t* compose_pdu(pdu_t* in) override;

    virtual void decompose_pdu(pdu_t* in) override;

public:
    TransportLayer(Bluetooth* bluetooth) : ProtocolLayer() {
        this->bluetooth = bluetooth;
    }

    virtual ~TransportLayer() {
        delete(bluetooth);
    }

    virtual bool send(pdu_t* pdu) override;

private:
    virtual bool receive(pdu_t* pdu) override;
};

#endif //ARCTOS_ROBOT_PROTOCOLLAYER_H
