// Code for the ATtiny (Slave)
//
// Luis Ortiz - luislab.com
// July 3, 2016
// This program is designed to use an ATtiny45 (or compatible) microcontroller 
// to read the water level of a container using an ultrasonic range finder HC-SR04.
// - Readings will be pushed through I2C
// - alarmPin: HIGH if the water level exceeds a threshold.
// - alarmPin: HIGH if sensor readings not between min and max range (this may be
//             useful to detect problems with the sensor.


#define I2C_SLAVE_ADDRESS 0x74 // Address of the slave

#define echoPin 4 // Echo Pin
#define trigPin 3 // Trigger Pin
#define LEDPin 1  // Onboard LED Pin / alarmPin

// As safety method, all measurements will be constrained to min and max range.
#define maxRange 100 // Maximum range
#define minRange 2   // Minimum range

#define waterThreshold 25 // Critical Water Level

#include <TinyWireS.h> //https://github.com/rambo/TinyWire

uint8_t distance = 0;
long duration; // Duration used to calculate distance

long lastMicros = 0;

long microsONDelay = 10;
long microsOFFDelay = 50000;

int lastTrig;

void setup() {
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(sendDistance);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  lastTrig = LOW;
  lastMicros = micros();

  pinMode(LEDPin, OUTPUT); //PB1
  digitalWrite(LEDPin, LOW);
}

void loop() {
  if ( lastTrig == LOW ) {
    if ((micros() - lastMicros) >= microsOFFDelay) {
      digitalWrite(trigPin, HIGH);
      lastMicros = micros();
      lastTrig = HIGH;
    }
  }else if ( lastTrig == HIGH ) {
    if ((micros() - lastMicros) >= microsONDelay) {
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      lastMicros = micros();
      lastTrig = LOW;
  
      //Calculate the distance (in cm) based on the speed of sound.
      distance = constrain(duration/58.2, minRange, maxRange);
    
      if ( distance >= maxRange || distance <= minRange ) {
        digitalWrite(LEDPin, HIGH);
      }else {
        if ( distance >= waterThreshold )
          digitalWrite(LEDPin, HIGH);
        else
          digitalWrite(LEDPin, LOW);
      }
    }
  }

  TinyWireS_stop_check();
}

void sendDistance() {
  TinyWireS.send(distance);
}

