/*
  SocketIOYunClient.h - Library to connect to a nodejs server implementing Socket.io.
  Created by Lorenzo Romagnoli, March 27, 2014.
  Released into the public domain.
*/

#ifndef SocketIOYunClient_h
#define SocketIOYunClient_h

#include "Arduino.h"
#include "Process.h"


class SocketIOYunClient{
public:
	SocketIOYunClient(String address, String port);
  	void begin();
    void sendMsg(String m);
		void connect();
		void close();
		
  private:
    void sendCommand(String m);
		String _address;
		String _port;
		Process _p;	
		
};
#endif