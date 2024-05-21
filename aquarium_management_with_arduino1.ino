#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// Project 1 Constants
const int triggerPin = 2;
const int echoPin = 3;
const int containerHeight = 20; // Centimeters
const int lcdAddress = 0x20;  // I2C address of your LCD
const int buzzerPin = 8;
const int ledPin = 9;

// Project 2 Constants
#define PIN 13
#define NUMPIXELS   12
int sensorPin = A1;
int sensorValue = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Project 3 Constants
Servo myServo;
int servoPin = 7;
unsigned long previousMillis = 0;// Variable to store the last time the servo was moved
const long interval = 5L ;  // Interval in milliseconds (6 hours)

// Project 4 Constants
const int sensorPinTemp = A0;
const float minTemp = 20.0;
const float maxTemp = 28.0;

LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

void setup() {
  // Project 1 Setup
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Water Level");
  lcd.setCursor(4, 1);
  lcd.print("Monitor");
  delay(4000);

  // Project 2 Setup
  pinMode(sensorPin, INPUT);
  pixels.begin();

  // Project 3 Setup
  myServo.attach(servoPin);
 

  // Project 4 Setup
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Project 1 Loop
  long duration, distance, waterLevel;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  waterLevel = containerHeight - distance;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WaterLevel:");
  lcd.print(waterLevel);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  if (waterLevel < 5) {
    lcd.print("Low WaterLevel!");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(500);
  } else if (waterLevel > containerHeight) {
    lcd.print("Leakage Detected!");
  } else {
    lcd.print("NormalWaterLevel");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  delay(4000);

  // Project 2 Loop
  sensorValue = analogRead(sensorPin);
  if (sensorValue > 500) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }
  } else if (sensorValue > 300 && sensorValue < 500) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();
    }
  } else if (sensorValue < 300) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      pixels.show();
    }
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }

  // Project 3 Loop
  unsigned long currentMillis = millis();// Get the current time
 
  
  // Check if the specified interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    myServo.write(180);// Move the servo to a specific position as 90 degree for feed 
    delay(1000);
    myServo.write(0);//Move back to its orginal position
    delay(1000);
  }
   lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Last feed before");//Print the last time fish feeded
    lcd.setCursor(0, 1);
    lcd.print(currentMillis );
    lcd.setCursor(2, 1);
    lcd.print(" Milli secound");
    delay(4000);

  // Project 4 Loop
  int sensorValueTemp = analogRead(sensorPinTemp);
  float voltage = sensorValueTemp * (5.0 / 1023.0);
  float temperature = (voltage - 500.0) / 10.0;
  if (temperature < minTemp) {
    Serial.println("WARNING: Temperature too low!");
    lcd.setCursor(0, 0);
    lcd.print("*TEMP LOW!*");
  } else if (temperature > maxTemp) {
    Serial.println("WARNING: Temperature too high!");
    lcd.setCursor(0, 0);
    lcd.print("*TEMP HIGH!*");
  } else {
    Serial.println("Temperature OK.");
    lcd.setCursor(0, 0);
    lcd.print("      ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("°C");
  delay(1000);
}