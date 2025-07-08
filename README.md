# SONOFF-SENSOR
The sonoff is a very interesting device because it has the possibility to install custom software. This way we can make use of another feature: We can connect sensors like f.i a BME 280. Now we can setup a thermostate or a hydrostate.  With this program installed on a Sonoff it is very easy to do that. 
The only thing you have to do is connect a sensor with 4 wires, and make some adjustments in the settings, done via the webinterface.
The software has some interesting features:
- actual sensor readings are displayed on the frontpage
- Simply to connect to your wifi
- data can be requested via http and mosquitto
- Fast asyc webserver with a intuitive webinterface.
- Smart timekeeping.
- A lot of system info on the webinterface.
- switch based on sensor readings, manual or via 4 timers 
- Temperature/humidity/pressure/light/motion/touch sensors can be connected.
- It can work with any digital sensor (sound/rain/tilt and more)
- 4 different ways to communicate with the device<br>
- a log of the events like 'time, switched-on, by who'<br>

I created this software some years ago, but recently i completely overhauled it. Its more mobile friendly now and the userinterface
has a lot of improvements. Moreover it is compiled with the latest ArduinoIDE version so that the firmware is up to date.

june 16 2025: There is a new version v11 available.
Download [sonoff_v11,ino.generic.bin](https://github.com/patience4711/SONOFF-SENSOR/blob/main/sonoff-v11.ino.generic.bin)

![frontpage](https://github.com/user-attachments/assets/80ca500e-1aaa-4897-a1f6-de6520d11f89)

This system uses an interesting technique to keep the frontpage up to date. When something hapens at the server (a switch, a sensor reading, the servers sends an event to the client. The client reacts with a data request to update values in the webpage. This way the webinterface does not have to refresh itself every 10 seconds.
