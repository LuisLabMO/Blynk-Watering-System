# Smart Modular Watering System using Blynk

I like plants, I like to be eco-friendly, love electronics, but how can I fit those things together? enters the **Smart Modular Watering System**.

By using the [SparkFun ESP8266 Blynk Board](https://www.sparkfun.com/products/13794) you can build a system that monitors the level of the water reservoir and will water your plants automatically at the right moment.

## Sensors

Data collected from sensors is being used to decide when is the right time to water the plants, and to send alerts when the water reservoir is low and more:

* **Soil moisture sensor**: accurately measures the actual soil moisture.
* An **ultrasonic distance sensor** can be used to measure the level of the water reservoir with good reliability.
* **UV sensor** data may help you decide whether your plant needs more shadow or direct sunlight.
* **Temperature** and **humidity** sensors can also help deciding about critical conditions for your plants.
* It will help you save water!. It will water the plants when is required and the water reservoir can be filled-up with rainwater.

## Make it Modular, Make it Smarter

The **Smart Modular Watering System** has a very simple implementation, but there is lot of room for improvement. Some other features can be added to the **Smart Modular Watering System** with some more work, below few examples:

* The **UV sensor** data can also be used to decide at what times of the day to stop watering. *Maybe stop watering at night?*
* The data collected can be analyzed to predict the water usage. e.g.: *How much water your plants will need before your next trip.*
* Add a solar panel, charge your batteries with it and show the world that you care about the Environment!.

## Repository Contents

* **3D_Models** - SketchUp and .STL files
* **/Hardware** - Eagle design files: ***.brd**, ***.sch**
* **/Blynk_Board_Source_Code** - Source code for the [SparkFun ESP8266 Blynk Board](https://www.sparkfun.com/products/13794)
* **/ATtiny45_HC-SR04_to_I2C_Source_Code** - Source code for the ATtiny45 used to convert the readings of the ultrasonic distance sensor to I2C.


