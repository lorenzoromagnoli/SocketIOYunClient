#include "Arduino.h"
#include "SocketIOYunClient.h"


SocketIOYunClient::SocketIOYunClient(String address, String port)
{
	_address=address;
	_port=port;	
}
void SocketIOYunClient::begin(){
  _p.begin("sh");	// Process that launch the "curl" command
  _p.addParameter("/opt/connectedObject/socket.io/mysh.sh");	// Process that launch the "curl" command
  _p.addParameter(_address);
  _p.addParameter(_port);
  _p.runAsynchronously();
}

void SocketIOYunClient::connect(){
  sendCommand("connect\n");
	
}

void SocketIOYunClient::sendMsg(String m){
  sendCommand("\n");
  delay(100);
  sendCommand("send "+m + '\n');
}

void SocketIOYunClient::sendCommand(String m){
  Serial.println("Senging new message: " + m);
  uint8_t payload[m.length()];
  m.getBytes(payload, m.length() + 1);

  for (int i = 0; i < m.length(); i++) {
    _p.write(payload[i]);
  //  Serial.print(payload[i]);
  }
	
}


void SocketIOYunClient::close(){
  _p.close();
	
}


