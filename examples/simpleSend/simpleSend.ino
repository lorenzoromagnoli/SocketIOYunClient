#include <Process.h>
#include <SocketIOYunClient.h>

String address = "54.84.192.169";
String port = "3000";

SocketIOYunClient mySocketIO(address, port); //create a new SocketIOYunClient object

void setup() {
  Bridge.begin(); //start Bridge

  mySocketIO.begin(); //start python process

  delay(5000);

  mySocketIO.connect(); //start the connectoin
}

void loop() {
  mySocketIO.sendMsg("ciao SocketIO!");
  delay(1000);
}
