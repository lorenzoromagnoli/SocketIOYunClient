#include <Bridge.h>
#include <SocketIOYunClient.h>

String address = "54.84.192.169";
String port = "3000";

SocketIOYunClient mySocketIO = SocketIOYunClient(address, port); //create a new SocketIOYunClient object

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ;
  }

  Serial.println("starting bridge");
  Bridge.begin(); //start Bridge
  Serial.println("bridge started");
  Serial.println("connecting mysocket");
  mySocketIO.connect(); //start the connectoin

}

void loop() {
  
  mySocketIO.sendMsg("ciao", "Socket.io", "Server");
  mySocketIO.monitor();
  delay(5000);


}