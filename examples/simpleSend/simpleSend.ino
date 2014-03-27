#include <Process.h>
#include <SocketIOYunClient.h>

String address = "54.84.192.169";
String port = "3000";

SocketIOYunClient mySocketIO(address, port); //create a new SocketIOYunClient object

void setup() {
  Bridge.begin(); //start Bridge
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  mySocketIO.begin(); //start python process
  delay(500);
  mySocketIO.connect(); //start the connectoin
  delay(500);

}

void loop() {
  mySocketIO.sendMsg("ciao finocchio buco");
  delay(1000);

  String receivedMessage = mySocketIO.receiveMsg();
  Serial.print(receivedMessage);
  if (receivedMessage != 0) {
    Serial.println(receivedMessage);
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);

  }

}