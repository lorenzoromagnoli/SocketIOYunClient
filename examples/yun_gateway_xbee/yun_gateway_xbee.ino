/*
this is the code that goes on the node
*/

/*
// constants won't change. They're used here to 
// set pin numbers:
const int sensorPin = A0;    // the number of the pushbutton pin

// Variables will change:
int sensorState;             // the current reading from the input pin
int lastSensorState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  delay(1000);
int reading;
  int val = analogRead(sensorPin);
  if (val>512){
    reading=1;
  }
  else{
    reading=0;
  }
  
  Serial.println(reading);

  if (reading != lastSensorState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != sensorState) {
      sensorState = reading;

      // only toggle the LED if the new button state is HIGH
      if (sensorState == LOW) {
      Serial.println("coffe");
      }
    }
  }

  // set the LED:

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastSensorState = reading;
}

*/

#include <Bridge.h>
#include <SocketIOYunClient.h>
#include <AltSoftSerial.h>
AltSoftSerial xbee;


String address = "54.84.192.169";
String port = "3000";


boolean stringComplete = false;
String inString; //string received on the serial Port

SocketIOYunClient mySocketIO = SocketIOYunClient(address, port); //create a new SocketIOYunClient object


void setup() {
//  Serial.begin(57600);
//  while (!Serial) {
//    ;
//  }
  
//  Serial.println("starting bridge");
  Bridge.begin(); //start Bridge
//  Serial.println("bridge started");

  mySocketIO.connect(); //start the connectoin

  xbee.begin(9600);
  xbee.println("Hello World");
//  Serial.println("finished Setup");

}

void loop() {
  xbeeEvent();

}


void xbeeEvent() {
  inString = xbee.readStringUntil('\n');
  //Serial.println(inString);
  if (inString != "") {
    //Serial.println("received" + inString);
    if (inString[0] == 'c') {
      //Serial.println("newCoffe");
      mySocketIO.sendMsg("newCoffe", "Socket.io", "Server");
 //     Serial.println("senrt new coffe");
    }

  }
}
