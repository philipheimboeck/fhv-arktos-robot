//
// Created by Philip Heimböck (Privat) on 09.12.15.
//

#ifndef ARCTOS_ROBOT_PROTOCOLLAYER_H
#define ARCTOS_ROBOT_PROTOCOLLAYER_H

#include <stddef.h>

#include "../bluetooth.h"

namespace communication {

	typedef struct {
		char data[122];
		char *data_start;
	} tuple_t;

	char* tuple_get_key(tuple_t* tuple);

	typedef struct {
		void* message;
		size_t length;
	} pdu_t;

	class ProtocolLayer {
	protected:
		ProtocolLayer* lowerLayer;
		virtual pdu_t* composePdu(pdu_t* in) = 0;
		virtual void decomposePdu(pdu_t* in) = 0;
		pdu_t* copyPdu(pdu_t* in, size_t increase);

	public:
		ProtocolLayer(ProtocolLayer* lower);
		ProtocolLayer();
		virtual ~ProtocolLayer();

		virtual bool sendData(pdu_t* pdu);
		virtual bool receiveData(pdu_t* pdu);
	};

	class PresentationLayer : public ProtocolLayer {

		virtual pdu_t* composePdu(pdu_t* in) override;

		virtual void decomposePdu(pdu_t* in) override;

	public:
		PresentationLayer(ProtocolLayer* lower) : ProtocolLayer(lower) { }
	};

	class SessionLayer : public ProtocolLayer {

		virtual pdu_t* composePdu(pdu_t* in) override;

		virtual void decomposePdu(pdu_t* in) override;

	public:
		SessionLayer(ProtocolLayer* lower) : ProtocolLayer(lower) { }
	};

	class TransportLayer : public ProtocolLayer {
	private:
		Bluetooth* bluetooth;

	protected:
		virtual pdu_t* composePdu(pdu_t* in) override;

		virtual void decomposePdu(pdu_t* in) override;

	public:
		TransportLayer(Bluetooth* bluetooth) : ProtocolLayer() {
			this->bluetooth = bluetooth;
		}

		virtual ~TransportLayer() {
			delete(bluetooth);
		}

		virtual bool sendData(pdu_t* pdu) override;

	private:
		virtual bool receiveData(pdu_t* pdu) override;
	};
}

#endif //ARCTOS_ROBOT_PROTOCOLLAYER_H
