#include "Arduino.h"
#include "SocketIOYunClient.h"


SocketIOYunClient::SocketIOYunClient(String address, String port)
{
	_address=address;
	_port=port;	
	_stringComplete=false;
}
void SocketIOYunClient::begin(){
  _p.begin("sh");	// Process that launch the "curl" command
  _p.addParameter("/opt/myScripts/SocketIOYunClient/socketIOClient.sh");	// Process that launch the "curl" command
  _p.addParameter(_address);
  _p.addParameter(_port);
  //_p.runAsynchronously();
	
}

void SocketIOYunClient::connect(){
	delay(500); 
  sendCommand("connect\n");
}

void SocketIOYunClient::sendMsg(String m){
  sendCommand("\n");
  delay(100);
  sendCommand("send "+m + '\n');
}

void SocketIOYunClient::sendCommand(String m){
  uint8_t payload[m.length()];
  m.getBytes(payload, m.length() + 1);
  for (int i = 0; i < m.length(); i++) {
    _p.write(payload[i]);
  }	
}

String SocketIOYunClient::receiveMsg(){
  String rs="";
  while (_p.available() > 0) {
		char c = _p.read();
		
		if(c== '\n') {
			rs=_receivedString;
			_receivedString="";    
		} else{
			_receivedString += c ;
			rs="";	
		}
  }
	return (rs);	
}




void SocketIOYunClient::close(){
  _p.close();
	
}


