/*
 * CommunicationClient.h
 *
 *  Created on: 19.12.2015
 *      Author: phili
 */

#ifndef COMMUNICATION_COMMUNICATIONCLIENT_H_
#define COMMUNICATION_COMMUNICATIONCLIENT_H_

#include "../general.h"
#include "protocol/ProtocolLayer.h"

namespace communication {

	class CommunicationClient {

	private:
		ProtocolLayer* connection;
		void (*driveCallback)(int left, int right);

	public:
		CommunicationClient(serial_port_options_t serial_options);
		virtual ~CommunicationClient();

		void setDriveCallback(void (*callback)(int left, int right));

		void retrieveData(void);
		bool sendLocation(location_t* location);
	};
}


#endif /* COMMUNICATION_COMMUNICATIONCLIENT_H_ */
