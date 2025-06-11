#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <MPU9250_asukiaaa.h>

RF24 radio(8, 9);  // CE, CSN
const byte address[6] = "00001";
MPU9250_asukiaaa mySensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  mySensor.accelUpdate();  // Update accelerometer data

  // Print raw sensor data for debugging
  float x = mySensor.accelX();
  float y = mySensor.accelY();
  float z = mySensor.accelZ();
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.println(z);

  if (y > 0.3) {  // If Y is above threshold, send "forward" command
    const char text[] = "forward";
    radio.write(&text, sizeof(text));
    Serial.println("Sent: forward");
  }

  else if (y < -0.3 ) {
    const char text[] = "backward";
    radio.write(&text, sizeof(text));
    Serial.println("Sent: backward");
  }

  else if(x > 0.3) {
    const char text[] = "right";
    radio.write(&text, sizeof(text));
    Serial.println("Sent: right");
  }

  else if(x < -0.3){
    const char text[] = "left";
    radio.write(&text, sizeof(text));
    Serial.println("Sent: left");
  }

  else {
    const char text[] = "stop";
    radio.write(&text, sizeof(text));
    Serial.println("Sent: stop");
  }
  

  delay(100);  // Delay to make the output readable
}