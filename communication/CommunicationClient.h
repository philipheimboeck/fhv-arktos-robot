/*
 * CommunicationClient.h
 *
 *  Created on: 19.12.2015
 *      Author: phili
 */

#ifndef COMMUNICATION_COMMUNICATIONCLIENT_H_
#define COMMUNICATION_COMMUNICATIONCLIENT_H_

#include <functional>
#include "../general.h"
#include "protocol/ProtocolLayer.h"

namespace communication {

	class CommunicationClient {

	private:
		ProtocolLayer* connection;
		std::function<void(int,int)> driveCallback;

	public:
		CommunicationClient(serial_port_options_t serial_options);
		virtual ~CommunicationClient();

		void setDriveCallback(std::function<void(int,int)>);

		void retrieveData(void);
		bool sendLocation(location_t* location);

		void sendHeartbeat(void);
	};
}


#endif /* COMMUNICATION_COMMUNICATIONCLIENT_H_ */
