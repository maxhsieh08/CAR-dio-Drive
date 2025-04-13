#include <Timer.h>
#include <WiFi.h>
#include <esp_now.h>
#include "EFFORT.h"


Timer timer;

// Flags
bool timerStarted = false;
bool stopMotors = false;

//Wifi Related
effort receivedData;

//Ports/Connections
//Motor 1
#define ENA 21 //Speed control pin (0, 255)
#define IN1 20 //Backward
#define IN2 10 //Forward
//Motor 2
#define ENB 3 //Speed control pin (0, 255)
#define IN3 0 //Forward
#define IN4 1 //Backward

int speed;
double EffortLvl = 0;

void stopAllMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

 void onReceive(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  if (len == sizeof(receivedData))
  {
    memcpy(&receivedData, data, sizeof(receivedData));
    Serial.print("Received Effort: ");
    Serial.println(receivedData.value);
    EffortLvl = receivedData.value;

  } else {
    Serial.println("Invalid data size.");
  }
}

void setup() {
  Serial.begin(115200);

  //Setup Pins
  //Motor 1
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  //Motor 2
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Initial state (off)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  //Setup Wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  // Register receive callback with the correct function signature
  esp_now_register_recv_cb(onReceive);

  
}

void loop() {
  // if (timerStarted && timer.read() >= 30000 && !stopMotors) {
  //   stopAllMotors();
  //   stopMotors = true;
  //   Serial.println("30 seconds elapsed. Rover stopped.");
  // }

  // Skip motor control if time is up
  // if (stopMotors) return;

  if (EffortLvl < 0.1) {
    speed = 0;
  } else if (EffortLvl >= 0.1 && EffortLvl <= 0.35) {
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(100);
    speed = 80;
  } else if (EffortLvl > 0.35 && EffortLvl <= 0.6) {
    speed = 138.88;
  } else if (EffortLvl > 0.6){
    speed = 255;
  }

  Serial.println(EffortLvl);
  Serial.print("speed: ");
  Serial.println(speed);


  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  // Drive both motors forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(300);
}


