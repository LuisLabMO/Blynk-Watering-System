# Smart Modular Watering System using Blynk

I like plants, I like to be eco-friendly, love electronics, but how can I fit those things together? enters the **Smart Modular Watering System**.

[![Smart Modular Watering System using Blynk - Youtube](http://luislab.com/luislab/wp-content/uploads/2016/07/SmartModularWateringSystem_youtube.jpg)](https://www.youtube.com/watch?v=UPALH_MEa6o)

Using the [SparkFun ESP8266 Blynk Board](https://www.sparkfun.com/products/13794) with the [Blynk app](http://www.blynk.cc) you can build a system that monitors the level of the water reservoir and will water your plants automatically at the right moment.

> **It will help you save water!**

## Sensors data

Data collected from sensors is used to decide when is the right time to water the plants, to send alerts when the water reservoir is low and more:

* **Soil moisture sensor**: accurately measures the actual soil moisture.
* An **ultrasonic distance sensor** will measure the level of the water reservoir with good reliability.
* **UV sensor** data may help deciding whether your plant needs more shadow or direct sunlight.
* **Temperature** and **humidity** sensors can also help deciding about critical conditions for your plants.

## Operation - The Blynk App

The Blynk app can be used to monitor in real time the data collected by the sensors:
* Plot the UV index.
* Monitor the water level, temperature, humidity, soil moisture in real time.
* Check the status of the irrigation system (On/Off) - **H2O Led**.

Some parameters of the system can be modified by using the Blynk app too:
* **Water level threshold**: if the water level is above¹ this number, the app will send a warning.
* **Soil moisture threshold**: the system will turn On the irrigation if the current soil moisture is below this number.
* **Watering interval _(in hours)_**: how often the system will irrigate -if conditions require it-.
* **Watering time _(in seconds)_**: duration of the irrigation when activated.

> ¹Please note that the water level sensor measures the distance between the top of the container and the surface of the water. So a higher number means a lower water level.

## Make it Modular, Make it Smarter

The **Smart Modular Watering System** has a very simple implementation, but there is lot of room for improvement. Some other features can be added to the **Smart Modular Watering System** with some more work, these are just a few examples:

* The **UV sensor** data can also be used to decide at what times of the day to stop watering. *Maybe stop watering at night?*
* The data collected can be analyzed to predict the water usage. e.g.: *How much water your plants will need before your next trip.*
* Add a solar panel, charge your batteries with it and show the world that you care about the Environment!.

## Repository Contents

* [/3D_Models](https://github.com/LuisLabMO/Blynk-Watering-System/tree/master/3D_Models) - SketchUp and **.STL** files
* [/ATtiny45_HC-SR04_to_I2C_Source_Code](https://github.com/LuisLabMO/Blynk-Watering-System/tree/master/ATtiny45_HC-SR04_to_I2C_Source_Code) - Source code for the ATtiny45 used to convert the data from ultrasonic distance sensor to I2C.
* [/Blynk_Board_Source_Code](https://github.com/LuisLabMO/Blynk-Watering-System/tree/master/Blynk_Board_Source_Code) - Source code for the [SparkFun ESP8266 Blynk Board](https://www.sparkfun.com/products/13794)
* [/Hardware](https://github.com/LuisLabMO/Blynk-Watering-System/tree/master/Hardware) - Eagle design files: **.BRD**, **.SCH**

## More Information

For more information visit the project page on [Hackster.io](https://www.hackster.io/lmortizg/smart-modular-watering-system-b571ec)
