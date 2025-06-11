#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 8); // CE , CSN
const byte address[6] = "00001";

int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 7;
int ena = 5;
int enb = 6;

unsigned long lastCommandTime = 0;
const unsigned long commandTimeout = 500; // 500ms timeout
String lastCommand = "stop";

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  analogWrite(ena, 126); // Half speed
  analogWrite(enb, 126);
}

void loop() {
  // Check if data is available
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println("Received: " + String(text));
    lastCommandTime = millis(); // ✅ Update time when command was received
    lastCommand = String(text);

    if (lastCommand == "forward") {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } 
    else if (lastCommand == "backward") {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    else if(lastCommand == "right") {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    else if(lastCommand == "left") {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
    else if (lastCommand == "stop") {
      stopMotors();
    }
  }

  // ✅ Always check timeout, even if no data
  if (millis() - lastCommandTime > commandTimeout && lastCommand != "stop") {
    Serial.println("Connection Lost. Stopping Motors.");
    stopMotors();
    lastCommand = "stop";
  }
}