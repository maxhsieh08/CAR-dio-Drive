#include <WiFi.h>
#include <esp_now.h>
#include "BPM.h"

//Wifi Related
bpm receivedData;

//Ports/Connections
//Motor 1
#define ENA 21 //Speed control pin (0, 255)
#define IN1 20 //Backward
#define IN2 10 //Forward
//Motor 2
#define ENB 3 //Speed control pin (0, 255)
#define IN3 0 //Forward
#define IN4 1 //Backward

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(receivedData)) {
    memcpy(&receivedData, data, sizeof(receivedData));
    Serial.print("Received BPM: ");
    Serial.println(receivedData.value);
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

  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
}

void loop() {
  
}

