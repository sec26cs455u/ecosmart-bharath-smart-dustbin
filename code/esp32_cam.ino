#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  5
#define ECHO_PIN     6
#define MAX_DISTANCE 60 // 60cm bin height

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servoYellow, servoBlue, servoOrange, servoGreen;

int yellowPin = 9, bluePin = 10, orangePin = 11, greenPin = 3;

void setup() {
  Serial.begin(9600);
  servoYellow.attach(yellowPin);
  servoBlue.attach(bluePin);
  servoOrange.attach(orangePin);
  servoGreen.attach(greenPin);
  closeAll();
  Serial.println("EcoSmart Bharath Ready");
}

void loop() {
  // Wait for command from ESP32-CAM / AI Model via Serial
  if(Serial.available()){
    String wasteType = Serial.readStringUntil('\n');
    wasteType.trim();
    Serial.print("Classified: "); Serial.println(wasteType);
    
    if(wasteType == "Recyclable") openBin(servoYellow);
    else if(wasteType == "Biodegradable") openBin(servoBlue);
    else if(wasteType == "Non-Biodegradable") openBin(servoOrange);
    else if(wasteType == "E-Waste") openBin(servoGreen);
    else Serial.println("Unknown Type");
  }

  // Fill level check
  int distance = sonar.ping_cm();
  if(distance > 0){
    int fillLevel = 100 - (distance * 100 / MAX_DISTANCE);
    if(fillLevel > 75){
      Serial.print("FILL ALERT: "); Serial.print(fillLevel); Serial.println("%");
      // Send to Firebase via ESP32
    }
  }
  delay(1000);
}

void openBin(Servo &s){
  s.write(90); // Open 90 degree
  delay(5000); // Stay open 5 sec
  closeAll();
}

void closeAll(){
  servoYellow.write(0);
  servoBlue.write(0);
  servoOrange.write(0);
  servoGreen.write(0);
}