#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SparkFunHTU21D.h> // Temperature and Humidity Sensor
#include <Wire.h>
#include <EEPROM.h>

////////////////////////////////////
// Blynk Virtual Variable Mapping //
////////////////////////////////////
#define VIRTUAL_LED                    V1 //This LED will indicate when the watering system is on/off in the app.
#define VIRTUAL_TEMPERATURE_F          V5
#define VIRTUAL_TEMPERATURE_C          V6
#define VIRTUAL_HUMIDITY               V7

#define VIRTUAL_WATERING_TIME          V10
#define VIRTUAL_WATERING_INTERVAL      V13

#define VIRTUAL_UV_INDEX               V19
#define VIRTUAL_WATER_LEVEL            V22
#define VIRTUAL_MOISTURE_THRESHOLD     V25
#define VIRTUAL_WATER_LEVEL_THRESHOLD  V26

#define VIRTUAL_RUNTIME                V30

////////////////////
// Blynk Settings //
////////////////////
char BlynkAuth[] = "blynkAuthToken";
char WiFiNetwork[] = "SSID";
char WiFiPassword[] = "WiFiPassword";

///////////////////////
// Hardware Settings //
///////////////////////
#define WS2812_PIN 4 // Pin connected to WS2812 LED
#define BUTTON_PIN 0
#define LED_PIN    5
HTU21D thSense;

////////////////////////
// Sensors settings   //
////////////////////////
#define VEML6070_ADDRESS_WRITE (0x70 >> 1)     // Address of UV Sensor VEML6070 Write 0x38h

#define VEML6070_ADDRESS_READ_MSB (0x73 >> 1)  // Address of UV Sensor VEML6070 MSB 0x39h
#define VEML6070_ADDRESS_READ_LSB (0x70 >> 1)  // Address of UV Sensor VEML6070 LSB 0x38h

#define HCSR04_ADDRESS 0x74                    // Address of Ultrasonic Sensor HC-SR04 (Water Level)

//////////////////////////
// Hardware Definitions //
//////////////////////////
uint8_t selfTestResult = 0;

//////////////////////////
// Variable Definitions //
//////////////////////////
bool firstConnect = true;

bool notifyFlag = false;

uint8_t wireR[2] = {0, 0};
uint16_t uvIndex = 0;

uint8_t waterLevel = 0;
uint8_t waterLevelThreshold = 20;  //Low Water Level: anything above or equal to this number is considered low water level.
uint16_t moistureThreshold = 520;   //Plant(s) will be watered if soil moisture is below this value.
uint16_t moisture = 0;

long lastWateringTime;
long wateringInterval = 60 * 60 * 1000; //How often system checks if plant(s) require water
long wateringTime = 15 * 1000;          //The amount of time sprinkler will be watering each time -in milliseconds-.
bool watering = false;                  //Watering system is currently On/Off (true/false).

long lastMeasuringTime;
long measuringInterval = 5 * 1000;      // How often system reads some sensors values in milliseconds

WidgetLED led1(VIRTUAL_LED); // LED widget in Blynk App

void setup()
{
  // Initialize hardware
  Serial.begin(9600); // Serial
  pinMode(BUTTON_PIN, INPUT); // Button input
  pinMode(LED_PIN, OUTPUT); // LED output

  // Initialize Blynk
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
  while (Blynk.connect() == false) {
    // Wait until connected
  }

  /////////////////
  // Si7021 Test //
  /////////////////
  thSense.begin();
  if (scanI2C(0x40)) { // Si7021 is i2c address 0x40
    Serial.println("Si7021 test succeeded");
    selfTestResult |= (1 << 1);
  }else
    Serial.println("Si7021 test failed");

  Wire.beginTransmission(VEML6070_ADDRESS_WRITE);
  Wire.write(0b00000110); // 1T
  Wire.endTransmission();
  Serial.println("UV Sensor VEML6070 Initialized");

  lastWateringTime = millis() - wateringInterval + 20000; // system will start checking moisture 15 seconds after being turned on
  lastMeasuringTime = millis() - measuringInterval;
}

void loop() {
  if ( millis() - lastMeasuringTime >= measuringInterval ) { //Read sensor values that are required for decision making
    Wire.requestFrom(HCSR04_ADDRESS, 1);
    while ( Wire.available() )
      waterLevel = Wire.read();
    lastMeasuringTime = millis();
    //Serial.println("Water level:" + String(waterLevel));
  }

  if ( !watering ) {
    if ( millis() - lastWateringTime >= wateringInterval ) {
      Serial.println("Water level: " + String(waterLevel));
      Serial.println("UV Index: " + String(uvIndex));

      notifyFlag = false;                            //to avoid repeating notifications, reset flag after each watering interval.
      moisture = analogRead(A0);

      if ( moisture < moistureThreshold ) {
        Serial.println("Moisture reading (" + String(moisture) + ") below Moisture threshold (" + String(moistureThreshold) + ").");
        Serial.println("Watering plant(s) for " + String(wateringTime / 1000) + " seconds.");

        watering = true;
        led1.on(); // Turn the H2O LED on in the app
        digitalWrite(LED_PIN, HIGH);        
      }else {
        Serial.println("Soil moisture (" + String(moisture) + ") OK. Plant(s) do not need water at this time.");
      }
      lastWateringTime = millis();
    }
  }else { //Watering plant(s)
    if ( millis() - lastWateringTime >= wateringTime ) {
        Serial.println("Sprinkler system is Off.");
        watering = false;
        lastWateringTime = millis();
        led1.off(); // Turn the H2O LED off in the app
        digitalWrite(LED_PIN, LOW);        
    }      
  }

  if ( waterLevel >= waterLevelThreshold ) {
    if ( !notifyFlag ) {
      Blynk.notify("Water level is running low."); // Notify!
      Serial.println("Water level (" + String(waterLevel) + ") above threshold (" + String(waterLevelThreshold) + "). Water level is running low.");
      notifyFlag = true;
    }
  }

  // Execute Blynk.run() as often as possible during the loop
  Blynk.run();
}


// Check for a response from an I2C device
bool scanI2C(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write( (byte)0x00 );
  if (Wire.endTransmission() == 0)
    return true;
  return false;
}

// Value ranges are ignored once values are written
// Board runs hot, subtract an offset to try to compensate:
float tempCOffset = 0; //-8.33;
BLYNK_READ(VIRTUAL_TEMPERATURE_F) {
  float tempC = thSense.readTemperature(); // Read from the temperature sensor
  tempC += tempCOffset; // Add any offset
  float tempF = tempC * 9.0 / 5.0 + 32.0; // Convert to farenheit
  // Create a formatted string with 1 decimal point:
  Blynk.virtualWrite(VIRTUAL_TEMPERATURE_F, tempF); // Update Blynk virtual value
}

BLYNK_READ(VIRTUAL_TEMPERATURE_C) {
  float tempC = thSense.readTemperature(); // Read from the temperature sensor
  tempC += tempCOffset; // Add any offset
  Blynk.virtualWrite(VIRTUAL_TEMPERATURE_C, tempC); // Update Blynk virtual value
}

BLYNK_READ(VIRTUAL_HUMIDITY) {
  float humidity = thSense.readHumidity(); // Read from humidity sensor
  Blynk.virtualWrite(VIRTUAL_HUMIDITY, humidity); // Update Blynk virtual value
}

BLYNK_WRITE(TEMP_OFFSET_VIRTUAL) { // Very optional virtual to set the tempC offset

  tempCOffset = param.asInt();
}

BLYNK_WRITE(VIRTUAL_WATERING_TIME) {
  int inputThreshold = param.asInt();

  wateringTime = constrain(inputThreshold, 10, 30) * 1000; // converting to milliseconds
  notifyFlag = false;

  Serial.println("Watering time set to: " + String(wateringTime / 1000) + " seconds.");
}

BLYNK_WRITE(VIRTUAL_WATERING_INTERVAL) {
  int inputThreshold = param.asInt();

  wateringInterval = constrain(inputThreshold, 1, 5) * ( 60 * 60 * 1000 ); //converting to milliseconds
  notifyFlag = false;

  Serial.println("Watering interval set to every " + String(wateringInterval / (60 * 60 * 1000)) + " hour(s).");
}

BLYNK_READ(VIRTUAL_UV_INDEX) {
  wireR[0] = 0;
  wireR[1] = 0;  
  uvIndex = 0;

  Wire.requestFrom(VEML6070_ADDRESS_READ_MSB, 1);
  while ( Wire.available() )
    wireR[0] = Wire.read();

  Wire.requestFrom(VEML6070_ADDRESS_READ_LSB, 1);
  while ( Wire.available() )
    wireR[1] = Wire.read();

  uvIndex = wireR[0] << 8;
  uvIndex |= wireR[1];

  Blynk.virtualWrite(VIRTUAL_UV_INDEX, uvIndex); // Update Blynk virtual value
}

BLYNK_READ(VIRTUAL_WATER_LEVEL) { //this value is refreshed in the loop since it is used for decision making.
  
  Blynk.virtualWrite(VIRTUAL_WATER_LEVEL, waterLevel); // Update Blynk virtual value
}

BLYNK_WRITE(VIRTUAL_MOISTURE_THRESHOLD) {
  int inputThreshold = param.asInt();

  moistureThreshold = constrain(inputThreshold, 1, 1024);
  notifyFlag = false;

  Serial.println("Moisture threshold set to: " + String(moistureThreshold));
}

BLYNK_WRITE(VIRTUAL_WATER_LEVEL_THRESHOLD) {
  int inputThreshold = param.asInt();

  waterLevelThreshold = constrain(inputThreshold, 1, 255);
  notifyFlag = false;

  Serial.println("Water level threshold set to: " + String(waterLevelThreshold));
}

BLYNK_CONNECTED() {
  if ( firstConnect ) {
    // Two options here. Either sync values from phone to Blynk Board:
    //Blynk.syncAll(); // Uncomment to enable.
    // Or set phone variables to default values of the globals:
    Blynk.virtualWrite(VIRTUAL_MOISTURE_THRESHOLD, moistureThreshold);
    Blynk.virtualWrite(VIRTUAL_WATER_LEVEL_THRESHOLD, waterLevelThreshold);
    Blynk.virtualWrite(VIRTUAL_WATERING_TIME, wateringTime / 1000); //Converting from milliseconds to seconds.
    Blynk.virtualWrite(VIRTUAL_WATERING_INTERVAL, wateringInterval / ( 60 * 60 * 1000) ); //Converting from milliseconds to hours
    //Blynk.notify("Smart Modular Watering System Ready!"); // Notify!
    firstConnect = false;
  }
}