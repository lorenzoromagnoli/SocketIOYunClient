#include "SocketIOYunClient.h"

SocketIOYunClient::SocketIOYunClient(const String& address, const String& port)
{
	_address=address;
	_port=port;	
	_stringComplete=false;
	
  _verbose = true;
	
	Process SIO;	
	
	
  for ( int i = 0; i < pidLength; i++ ) {
    pid [i] = '\0';
  }

  for ( int i = 0; i < sbPidsLen; i++ ) {
    sbPids [i] = '\0';
  }

  Console.buffer(64);
	
}

int SocketIOYunClient::SIO_data_int_max = 6;
int SocketIOYunClient::SIO_data_bool_max = 5;
int SocketIOYunClient::SIO_data_str_max = 50;
int SocketIOYunClient::SIO_address_max = 20;
int SocketIOYunClient::SIO_var_max = 20;

SocketIOYunClient::OnBooleanMessage SocketIOYunClient::_onBooleanMessage = NULL;
SocketIOYunClient::OnRangeMessage SocketIOYunClient::_onRangeMessage = NULL;
SocketIOYunClient::OnStringMessage SocketIOYunClient::_onStringMessage = NULL;
SocketIOYunClient::OnCustomMessage SocketIOYunClient::_onCustomMessage = NULL;
SocketIOYunClient::OnSIOOpen SocketIOYunClient::_onOpen = NULL;
SocketIOYunClient::OnSIOClose SocketIOYunClient::_onClose = NULL;
SocketIOYunClient::OnSIOError SocketIOYunClient::_onError = NULL;

void SocketIOYunClient::onOpen(OnSIOOpen function) {
  _onOpen = function;
}
void SocketIOYunClient::onClose(OnSIOClose function) {
  _onClose = function;
}
void SocketIOYunClient::onRangeMessage(OnRangeMessage function) {
  _onRangeMessage = function;
}
void SocketIOYunClient::onStringMessage(OnStringMessage function) {
  _onStringMessage = function;
}
void SocketIOYunClient::onBooleanMessage(OnBooleanMessage function) {
  _onBooleanMessage = function;
}
void SocketIOYunClient::onCustomMessage(OnCustomMessage function) {
  _onCustomMessage = function;
}
void SocketIOYunClient::onError(OnSIOError function) {
  _onError = function;
}


void SocketIOYunClient::connect(){
	
  killPids();
	SIO.begin("sh");	// Process that launch the "curl" command
	
  SIO.addParameter("/opt/myScripts/SocketIOYunClient/socketIOClient.sh");	// Process that launch the "curl" command
  SIO.addParameter(_address);
  SIO.addParameter(_port);
	
	Console.begin();
  if (_verbose) {
    Serial.println(F("Starting console "));
  }
	
	SIO.runAsynchronously();
  
	if (_verbose) {
    Serial.println(F("SocketIoCLient initialized"));
  }
	if (_verbose) {
    Serial.println(F("SocketIoCLient connected"));
  }
	
  while (!Console) {
    ;
  }
  if (_verbose) {
    Serial.println(F("console started "));
  }
	
}

void SocketIOYunClient::sendMsg(const String& addr, const String& var,const String& data){
  Console.print(char(29));
  Console.print(addr);
  Console.print(char(30));
  Console.print(var);
  Console.print(char(31));
  Console.print(data);
  Console.print(char(7));
  Console.flush();
	
	if (_verbose) {
		Serial.print("sending (");
	  Serial.print(addr);
		Serial.print(" ");
	  Serial.print(var);
		Serial.print(" ");
		Serial.print(data);
		Serial.println(")");
		
	}

}


/*
String SocketIOYunClient::receiveMsg(){
  String rs="";
  while (SIO.available() > 0) {
		char c = SIO.read();
		
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
*/
void SocketIOYunClient::close(){
  SIO.close();
	
}


void SocketIOYunClient::monitor() {
  while (Console.available() > 0) {
    char c = Console.read();
		
    if (_verbose) {
      Serial.print(c);
    }
    if (c == char(CONNECTION_START) && !_connected) {
      if (_verbose) {
        Serial.print(F("Connected to SocketIO server at: "));
        Serial.println(_address);
      }
      if (_onOpen != NULL) {
        _onOpen();
      }
      _connected = true;
    }
    else if (c == char(CONNECTION_END) && _connected) {
      _connected = false;
      if (_verbose) {
        Serial.print(F("Disconnected from SocketIO server at: "));
        Serial.println(_address);
      }
      if (_onClose != NULL) {
        _onClose();
      }
    }
		
    if (_verbose) {
      if (c == char(CONNECTION_ERROR)) {
        _error_msg = true;
        Serial.println(F("ERROR :: with SocketIOYunClient.py Connection ::"));
      }
      else if (_error_msg && c == char(MSG_END)) {
        _error_msg = false;
        Serial.println();
      }
      if (_error_msg) {
        Serial.print(c);
      }
    }
		
    if (_connected) {
      if (c == char(MSG_ADDR)) { 
        read_address = true;
      } else if (c == char(MSG_VAR) || received_address.length() > SIO_address_max) {
        read_address = false;
        read_var = true;
      } else if (c == char(MSG_DATA) || received_var.length() > SIO_data_str_max) {
        read_address = false;
        read_var = false;
        read_data = true;		
      } else if (c == char(MSG_END) || received_data.length() > SIO_data_str_max) {
        if (read_data == true) {
          read_data = false;
          onMessage();
          // delay(2);
        }
        if (read_confirm == true) {
          read_confirm = false;
          onConfirm();
          delay(2);
        }
      } else if (c == char(3)) {
        read_confirm = true;
      } else {
        if (read_address == true) {
          received_address += c;
       // } else if (read_confirm == true) {
       //   sub_name += c;
        } else if (read_var == true) {
          received_var += c;
        
				} else if (read_data == true) {
          received_data += c;
        }

      }
    }
  }
	//maybe resend the messages if no confirmation is received
}

void SocketIOYunClient::onConfirm() {

	 if (_verbose) {
	 Serial.println(F(" confirmed"));

	 received_address= "";
	 received_var= "";
	 received_data= "";
 }
}

void SocketIOYunClient::onMessage() {
	Serial.println();
  Serial.print("received (");
  Serial.print(received_address);
  Serial.print(" ");
  Serial.print(received_var);
  Serial.print(" ");
  Serial.print(received_data);
  Serial.println(")");
	
	
 received_address= "";
 received_var= "";
 received_data= "";
}



void SocketIOYunClient::getPids() {

  // request the pid of all python processes
  // brew.begin("run-getsbpids"); // Process should launch the "curl" command
  pids.begin("python");
  pids.addParameter("opt/myScripts/SocketIOYunClient/getprocid.py"); // Process should launch the "curl" command
  pids.run();

  if (_verbose) {
    Serial.println(F("Checking if any socketIO process already running"));
  }

  int sbPidsIndex = 0;
  int pidCharIndex = 0;
  char c = '\0';

  while ( pids.available() > 0 ) {

    c = pids.read();

    if ( c >= '0' && c <= '9' ) {
      pid[pidCharIndex] = c;
      pidCharIndex = (pidCharIndex + 1) % pidLength;
    }

    else if ( (c == ' ' || c == '\n') && pidCharIndex > 0) {
      sbPids[sbPidsIndex] = atoi(pid);
      if ( sbPidsIndex < (sbPidsLen - 1) ) sbPidsIndex = (sbPidsIndex + 1);

      for ( int i = 0; i < pidLength; i++ ) {
        pid[i] = '\0';
        pidCharIndex = 0;
      }
    }
  }
}


/**
 * method that kills all of the spacebrew.py instances that are running
 * on the linino.
 */
void SocketIOYunClient::killPids() {
  getPids();
  delay(400);
	Console.end();

  for (int i = 0; i < sbPidsLen; i ++) {
    if (sbPids[i] > 0) {
      char * newPID = itoa(sbPids[i], pid, 10);

      if (_verbose) {
        Serial.print(F("Stopping existing socketIO processes with pids: "));
        Serial.println(newPID);
      }

      Process p;
      p.begin("kill");
      p.addParameter("-9");
      p.addParameter(newPID);		// Process should launch the "curl" command
      p.run();            		// Run the process and wait for its termination

      delay(400);
    }
  }
}

