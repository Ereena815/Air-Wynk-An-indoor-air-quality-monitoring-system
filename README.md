# Air-Wynk: An indoor air quality monitoring system

# About the project
The project is a technical investigation into analyzing patterns of air quality index from
MQ135 Air Sensor as well as temperature and humidity from DHT22 Temperature Sensor and
interfacing with a mobile application named Blynk to monitor these patterns. I hypothesize
that the AQI data can be used to analyze air quality patterns over some days, and based on
these patterns we can judge the different time intervals of a day where the air quality
reaches unsafe levels.

From the air quality data I have collected over a time interval of 5 days, it has been seen
that the air quality in my room is moderate most of the times (51-100 PPM) and becomes
unhealthy mostly in the afternoon-evening time (101-200 PPM). Therefore, through this
judgement, I have programmed the Arduino UNO microcontroller being used in my project
to switch on the fan from 4:00 PM to 7:00 PM every day as the air quality becomes unhealthy
mostly in the given time span. I have a control over the fan through the mobile application
as well and I can always switch it off whenever it’s not required.

# Important resources
1. Detailed Report: https://drive.google.com/file/d/1HjVtQ4BiTvb0nVMT0dGSRWdoF6QwkqjF/view?usp=sharing
2. Project Poster: https://drive.google.com/file/d/1Do1Pjo7l1A8obdit-4XpIRWmRWpJ_A4g/view?usp=sharing
3. Project Pitch: https://youtu.be/A1gTYSM9yiQ
