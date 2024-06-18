//Device credentials
#define BLYNK_TEMPLATE_ID "************************"
#define BLYNK_TEMPLATE_NAME "Automated Irrigation System"
#define BLYNK_AUTH_TOKEN "************************"

char auth[] = "**********************"; // Enter your Auth token
char ssid[] = "**********************";    //Enter your WIFI name
char pass[] = "**********************";  //Enter your WIFI password

//Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Define component pins
#define sensor A0
#define led D1
#define waterPump D2
#define buzzer D3
#define trigPin D4
#define echoPin D5
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
bool Relay = 0;
int sensorState = 0;
int prevState = 0;
int value;
long duration;
float distance;
BlynkTimer timer;

void setup() {
  pinMode(waterPump, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(waterPump, HIGH);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

void waterOn() {
  digitalWrite(waterPump, LOW);
  digitalWrite(led, HIGH);
  Blynk.logEvent("Water Pump", "Pump Started!");
  Serial.println("Pump Started, Water Flowing.");
}

void waterOff() {
  digitalWrite(waterPump, HIGH);
  digitalWrite(led, LOW);
  Blynk.logEvent("Water Pump", "Pump Stopped!");
  Serial.println("Pump Stopped, Water Not Flowing.");
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Serial.print("Humidity Value: ");
  Serial.println(value);
  Serial.println("");
  Blynk.virtualWrite(V0, value);
  delay(1000);

  if(value <= 30) {
    waterOn();
  } else {
    waterOff();
    tone(buzzer, 4000, 50);
    delay(100);
    tone(buzzer, 4000, 50);
    delay(100);
    tone(buzzer, 4000, 50);
    delay(100);
    noTone(buzzer);
  }
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distance = duration * SOUND_VELOCITY / 2;
  
  // Prints the distance on the Serial Monitor
  if (distance <= 100) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    tone(buzzer, 400, 100);
    delay(10);
    Blynk.virtualWrite(V1, distance);
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    noTone(buzzer);
    Blynk.virtualWrite(V1, distance);
  }

  Blynk.run();  //Run the Blynk library
  timer.run();  //Run the Blynk timer
}