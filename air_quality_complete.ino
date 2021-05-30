/****
   Data Capture Technologies
   Project : Air-Wynk - an aindoor air quality monitoring system
   Student Name : Ereena Bagga
   Student ID : 2010993040
 ****/

/* Blynk is a platform with iOS and Android apps used to control Arduino.
   I have used the platform since it supports easy communication with our projects and can be used to get data from multiple sensors.
   References:
    Downloads, docs, tutorials: http://www.blynk.cc
    Blynk community:            http://community.blynk.cc
*/
/* Libraries and Constant Definitions for the communication of ESP8266 Wifi module with the Blynk app. */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

/* Library to work with DHT22 Temperature Sensor. */
#include <DHT.h>
/* Defining the digital pin 4 of the Arduino board as DHTPIN to get input data from the DHT22 sensor.*/
#define DHTPIN 4
/* Defining the type of DHT sensor as DHT22*/
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* Libraries to work with Liquid Crystal Screen. */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

/* Time library helps in real time kepping without the support of any Real Time Clock.*/
#include <TimeLib.h>
time_t currentTime;

/* Arduino Pin Definitions.*/
#define AIRSENSORPIN A0
#define REDLEDPIN 12
#define YELLOWLEDPIN 13
#define GREENLEDPIN 2
#define BUZZERPIN 7
#define FANPIN 8

float humidity;
float temperature;
float airQuality;

/* Authorization ticket from the Blink app.*/
char auth[] = "hrFzI7xB64Uz-QUHEEdbbysV98eJUwuf";
/* WiFi credentials.*/
char ssid[] = "Ereena";
char pass[] = "mainukipatahove";

/* List used for designing different custom characters to be displayed on the LCD Screen.*/
byte bell[] = {
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000,
  B00000
};

byte leftBorder[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte rightBorder[] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};

byte degree[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

/* Method used to display the initial setup on the LCD screen.*/
void initialDisplayLed()
{
  // Create custom characters
  lcd.createChar(0, bell);
  lcd.createChar(1, leftBorder);
  lcd.createChar(2, rightBorder);

  // Display the custom characters on the screen
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(15, 0);
  lcd.write(byte(2));
  lcd.setCursor(15, 1);
  lcd.write(byte(2));

  // Display the name of the project on the screen
  lcd.setCursor(4, 0);
  lcd.print("Air Wynk");
  lcd.setCursor(7, 1);
  lcd.write(byte(0));

  // Give a delay for 8 seconds
  delay(8000);
}

/* Method used to scroll the entire display on the LCD to the left, right, and back to the centre - used to create a good screen display.*/
void scrollDisplayLed()
{
  // scroll 13 positions (string length) to the left to move the entire string offscreen
  for (int positionCounter = 0; positionCounter < 13; positionCounter++)
  {
    lcd.scrollDisplayLeft();
    delay(150);
  }

  // scroll scroll 29 positions (string length + display length) to the right to move the entire string offscreen
  for (int positionCounter = 0; positionCounter < 29; positionCounter++)
  {
    lcd.scrollDisplayRight();
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left to move the entire string back to the centre
  for (int positionCounter = 0; positionCounter < 16; positionCounter++)
  {
    lcd.scrollDisplayLeft();
    delay(150);
  }
}

/* Method used to read temperature and humidity readings from the DHT22 Temperature Sensor.*/
void tempReading()
{
  // Reading temperature and humidity values
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Ensuring clean data since nan values are avoided
  if (!isnan(humidity) && !isnan(temperature))
  {
    // Displaying data on serial monitor
    Serial.print("\nDATA,DATE,TIME,");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");

    // Displaying data on LCD
    lcd.createChar(3, degree);
    lcd.clear();
    lcd.home();
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.setCursor(11, 0);
    lcd.write(byte(3));
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }
  // Method call to scroll the entire display on the LCD
  scrollDisplayLed();

  // Delay given for 20 seconds
  delay(20000);
}

/* Method used to read air quality index (AQI) from the MQ135 Air Sensor.*/
void airQualityReading()
{
  // Reading AQI values
  airQuality = analogRead(AIRSENSORPIN);

  // Ensuring clean data since nan values are avoided
  if (!isnan(airQuality))
  {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Air Quality");

    if (airQuality > 0 && airQuality <= 50)
    {
      // If AQI is good, then turn the green led on and show the respective values on lcd and serial monitor.
      digitalWrite(REDLEDPIN, LOW);
      digitalWrite(YELLOWLEDPIN, LOW);
      digitalWrite(GREENLEDPIN, HIGH);
      digitalWrite(FANPIN, LOW);
      Serial.print("Good,");
      Serial.print(airQuality);
      lcd.setCursor(6, 1);
      lcd.print("Good");
    }
    else if (airQuality > 50 && airQuality <= 100)
    {
      // If AQI is moderate, then turn the yellow led on and show the respective values on lcd and serial monitor.
      digitalWrite(REDLEDPIN, LOW);
      digitalWrite(YELLOWLEDPIN, HIGH);
      digitalWrite(GREENLEDPIN, LOW);
      digitalWrite(FANPIN, LOW);
      Serial.print("Moderate,");
      Serial.print(airQuality);
      lcd.setCursor(4, 1);
      lcd.print("Moderate");
    }
    else if (airQuality > 100 && airQuality <= 200)
    {
      // If AQI is unhealthy, then turn the yellow led on and show the respective values on lcd and serial monitor.
      digitalWrite(REDLEDPIN, LOW);
      digitalWrite(YELLOWLEDPIN, HIGH);
      digitalWrite(GREENLEDPIN, LOW);
      digitalWrite(FANPIN, LOW);
      Serial.print("Unhealthy,");
      Serial.print(airQuality);
      lcd.setCursor(3, 1);
      lcd.print("Unhealthy");
    }
    else if (airQuality > 200 && airQuality <= 300)
    {
      // If AQI is very unhealthy, then turn the red led, buzzer, and the fan on and show the respective values on lcd and serial monitor.
      digitalWrite(REDLEDPIN, HIGH);
      digitalWrite(YELLOWLEDPIN, LOW);
      digitalWrite(GREENLEDPIN, LOW);
      digitalWrite(BUZZERPIN, HIGH);
      digitalWrite(FANPIN, HIGH);
      Serial.print("Very Unhealthy,");
      Serial.print(airQuality);
      lcd.setCursor(1, 1);
      lcd.print("Very Unhealthy");
    }
    else if (airQuality > 300)
    {
      // If AQI is hazardous, then turn the red led, buzzer, and the fan on and show the respective values on lcd and serial monitor.
      digitalWrite(REDLEDPIN, HIGH);
      digitalWrite(YELLOWLEDPIN, LOW);
      digitalWrite(GREENLEDPIN, LOW);
      digitalWrite(BUZZERPIN, HIGH);
      digitalWrite(FANPIN, HIGH);
      Serial.print("Hazardous,");
      Serial.print(airQuality);
      lcd.setCursor(3, 1);
      lcd.print("Hazardous");
    }
  }
  // Method call to scroll the entire display on the LCD
  scrollDisplayLed();

  // Delay given for 20 seconds
  delay(20000);
}

/* Custom Air Quality Monitor settings based on the data collection. Method used to turn the fan on from 4:00 PM to 7:00 PM
   as the air quality mostly becomes unhealthy in the given time interval.
*/
void customAirQualityDisplay()
{
  while (16 < hour(currentTime) < 19)
  {
    tempReading();
    airQualityReading();
    digitalWrite(FANPIN, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Time To Open");
    lcd.setCursor(2, 1);
    lcd.print("Your Windows");
    scrollDisplayLed();
    delay(20000);
  }
}

/* Method used to send sensor data to the Blynk app.*/
void sendSensorData()
{
  if (isnan(humidity) || isnan(temperature) || isnan(airQuality)) {
    return;
  }
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V7, airQuality)
}

void setup()
{
  // put your setup code here, to run once:
  // Communication with the Blynk app.
  Blynk.begin(auth, ssid, pass);
  // Serial communication.
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  // Set time when only when you start the Air Wynk
  setTime(22, 00, 00, 23, 5, 2021);

  // Set mode of different pins as input or output.
  pinMode(DHTPIN, INPUT);
  pinMode(AIRSENSORPIN, INPUT);
  pinMode(REDLEDPIN, OUTPUT);
  pinMode(YELLOWLEDPIN, OUTPUT);
  pinMode(GREENLEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  lcd.backlight();
  Serial.print("LABEL, Date, Time,Temperature (in Celsius), Humidity (in %RH), Air Quality , Air Quality Index (in PPM)");
  initialDisplayLed(); // Method call to display the intial settings on the LCD.
  timer.setInterval(1000L, sendSensorData); // Set the time interval (1 second) at which readings will be sent to the Blynk app
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  currentTime = now();
  customAirQualityDisplay();
  tempReading();
  airQualityReading();
  timer.run();
  digitalWrite(BUZZERPIN, LOW);
}
