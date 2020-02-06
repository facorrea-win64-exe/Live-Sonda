/*********************************
  Simple demonstration of HC-12 433mHz Radio
  Connect Vcc to Arduino 5volts
  Connect Gnd to ArduinoGnd
  Connect SET to Arduino pin 10 (Take LOW to send AT Commands)

  To switch HC-12 to AT Mode, take AT PIN LOW
  To return to Normal Mode take ATPIN HIGH

*********************************/

#include <SoftwareSerial.h>
#define ATpin 10 // used to switch to AT mode
#define LEDpin 13
SoftwareSerial HC12Serial(3, 2); // tx,rx --> GOES TO TX,RX OF DEVICE

void setup() {
  Serial.begin(9600);
  //Serial.begin(4800);
  Serial.setTimeout(20);
  pinMode(ATpin, OUTPUT);
  digitalWrite(ATpin, HIGH); // normal mode (Alternate between High and Low, Low mode is for programming, normal mode is for functioning)
  //digitalWrite(ATpin, LOW); // AT Mode
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LOW);
  HC12Serial.begin(9600);
  HC12Serial.setTimeout(20);
}

void loop() {
  bool atm=false;
  String aton="atmodeon";
  String atoff="atmodeoff";
  if (Serial.available() > 0) { // Read sent TO HC-12
    String input = Serial.readString();
    if (input==aton){
      digitalWrite(ATpin,LOW);
      Serial.println("Modo AT activado");
      //atm=true;
    }
    delay(2000);
    if (input==atoff ){
      digitalWrite(ATpin,HIGH);
      Serial.println("Modo AT desactivado");
      //atm=false;
    }
    HC12Serial.println(input);
  }
  if (HC12Serial.available() > 0) { // send data FROM HC-12
    String input = HC12Serial.readString();
    Serial.println(input);
  }
  delay(1000);
}
