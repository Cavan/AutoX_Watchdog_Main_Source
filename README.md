**Introduction**

This repo is for the AutoX Watchdog hardware unit the project is part of a Capstone project at Conestoga College's Software Engineering Technology (SET) program, the source code in this repo does the following.

* Monitors the PIR(passive infrared sensor) sensors
* Drives the servos to move in the direction of the triggered sensor
*  Captures the image once it's moved into position by the servos
* Saves the image onto a mini-SD card
* Sends a notification through serial communication to the sim900 that will then send a SMS text notification to the user that motion was detected and what direction the camera was aimed at, front, left, right, rear. 





**Wiring Diagram**

![https://inceptioncoding.com/autoxWiki/dokuwiki/lib/exe/fetch.php?media=autoxwatchdog:autoxwatchdog_version_3_bb.png](https://inceptioncoding.com/autoxWiki/dokuwiki/lib/exe/fetch.php?media=autoxwatchdog:autoxwatchdog_version_3_bb.png)



**Hardware Unit Assembled**

![IMG-20200410-015728](https://i.ibb.co/BTnBwWp/IMG-20200410-015728.jpg)