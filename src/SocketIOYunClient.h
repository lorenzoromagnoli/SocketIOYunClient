/*
  SocketIOYunClient.h - Library to connect to a nodejs server implementing Socket.io.
  Created by Lorenzo Romagnoli, March 27, 2014.
  Released into the public domain.
*/

#ifndef SocketIOYunClient_h
#define SocketIOYunClient_h

#include "Arduino.h"
#include <Bridge.h>
#include <Console.h>
#include <Process.h>

enum SBmsg {
  CONNECTION_START = char(28),
  CONNECTION_END = char(27),
  CONNECTION_ERROR = char(26),
  MSG_CONFIRM = char(3),
  MSG_ADDR = char(29),
  MSG_VAR = char(30),
  MSG_DATA = char(31),
	MSG_END = char(7)
		
};

int const pidLength = 6;
int const sbPidsLen = 4;


class SocketIOYunClient{
public:
	SocketIOYunClient(const String&, const String&);
    void sendMsg(const String& addr, const String& var,const String& data);
		void connect();
		void close();
		
    void monitor();
    void onMessage();
		void onConfirm();
		
		
    typedef void (*OnBooleanMessage)(String name, boolean value);
    typedef void (*OnRangeMessage)(String name, int value);
    typedef void (*OnStringMessage)(String name, String value);
    typedef void (*OnCustomMessage)(String name, String value, String type);
    typedef void (*OnSIOOpen)();
    typedef void (*OnSIOClose)();
    typedef void (*OnSIOError)(int code, String message);

    void onOpen(OnSIOOpen function);
    void onClose(OnSIOClose function);
    void onRangeMessage(OnRangeMessage function);
    void onStringMessage(OnStringMessage function);
    void onBooleanMessage(OnBooleanMessage function);
    void onCustomMessage(OnCustomMessage function);
    void onError(OnSIOError function);
		
		
//		String receiveMsg();
		
		
  private:
    void sendCommand(String m);
		String _address;
		String _port;
		Process SIO;	
		boolean _stringComplete;
		String _receivedString;
    Process pids;
		boolean _verbose;
    boolean _connected;
    boolean _error_msg;
		
    /**Output should be at least 5 cells**/
    static OnBooleanMessage _onBooleanMessage;
    static OnRangeMessage _onRangeMessage;
    static OnStringMessage _onStringMessage;
    static OnCustomMessage _onCustomMessage;
    static OnSIOOpen _onOpen;
    static OnSIOClose _onClose;
    static OnSIOError _onError;
		
    String received_address;
    String received_var;
    String received_data;
		
		
    boolean read_address;
    boolean read_var;
    boolean read_data;
    boolean read_confirm;
		
    static int SIO_address_max;
    static int SIO_var_max;
    static int SIO_data_max;
    static int SIO_data_str_max;
    static int SIO_data_int_max;
    static int SIO_data_bool_max;

		
		
		
		
    char pid [6];
    int sbPids [4];
    void killPids();
    void getPids();
		
};
#endif