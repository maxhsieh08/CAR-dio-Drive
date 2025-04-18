#include <PulseSensorPlayground.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Timer.h>
#include "EFFORT.h"

//Constants
#define N 10
#define switchOutput 3
#define playerOne 10
#define playerTwo 20
#define winner1 21
#define winner2 1

//PulseSensor
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

//Timer
Timer timer;

//Wifi settings
uint8_t receiverMAC[] = {0x34, 0xb7, 0xda, 0xf6, 0x41, 0x98};
effort effortData;

//Heart Calculations
float maxHeartRate1;
float avgRestingRate1;
float maxHeartRate2;
float avgRestingRate2;

//Winning calculations
int count = 0;
double sumOfEffort = 0;
double averageEffort1 = 0;
double averageEffort2 = 0;

//Player info
//1
char sex1 = 'm';
int age1 = 20;
//2
char sex2 = 'm';
int age2 = 20;

void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void callibrate(int player) {
  Serial.println("Make sure sensor is attached. Callibrating resting heart rate (10 seconds).");
  delay(3000);
  int sumBPM = 0;
  int numOfSamples = 0;
  for (int i = 0; i < 100; i++) {
    if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
      int signal = analogRead(PulseWire);
      int newBPM = pulseSensor.getBeatsPerMinute();
      numOfSamples++;
      sumBPM = newBPM + sumBPM;
      Serial.println(newBPM);
    }
    delay(100);
  }
  Serial.print("Resting rate calculated as: " );
  if (player == 1) {
    avgRestingRate1 = sumBPM/numOfSamples;
    Serial.println(avgRestingRate1);
  } else if (player == 2) {
    avgRestingRate2 = sumBPM/numOfSamples;
    Serial.println(avgRestingRate2);
  }
  Serial.println();
  digitalWrite(winner1, HIGH);
  delay(1000);
  digitalWrite(winner1, LOW);
}

void setup() {

  Serial.begin(115200);          // For Serial Monitor

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("Started up successfully!\n");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  //Wifi Setup
  WiFi.mode(WIFI_STA);  // ESP-NOW requires STA mode
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  // Use same channel or 0
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }

  // Register the send callback
  esp_now_register_send_cb(onSend);

  //Pin setup
  pinMode(switchOutput, OUTPUT);
  pinMode(playerOne, INPUT);
  pinMode(playerTwo, INPUT);
  pinMode(winner1, OUTPUT);
  pinMode(winner2, OUTPUT);
  
  digitalWrite(switchOutput, HIGH);
  //digitalWrite(winner1, LOW);
  //digitalWrite(winner2, LOW);

  //Begin game and callibration
  // Serial.println("Calculating max heart rate. Please enter your age: ");
  // while (Serial.available() == 0) {
  //   // Wait for input
  // }
  // int userAge = Serial.readStringUntil('\n').toInt();
  // Serial.print("You selected: ");
  // Serial.println(userAge);
  // Serial.println();

  // Serial.println("Please enter your sex (m/f): ");
  // while (Serial.available() == 0) {
  //   // Wait for input
  // }
  // char sex = Serial.read();
  // Serial.print("You selected: ");

  // //Calculate max heart rate
  // if (sex == 'm') {
  //   Serial.println("Male");
  //   maxHeartRate = (203.7 / (1 + exp(0.033*(userAge-104.3))));
  // } else if (sex == 'f') {
  //   Serial.println("Female");
  //   maxHeartRate = (190.2 / (1 + exp(0.0453*(userAge-107.5))));
  // }
  // Serial.println();
  // delay(1000);
  // Serial.print("Max rate calculated as: ");
  // Serial.println(maxHeartRate);
  // Serial.println();

  //Calculate max heart rate for player 1
  if (sex1 == 'm') {
    Serial.println("Male");
    maxHeartRate1 = (203.7 / (1 + exp(0.033*(age1-104.3))));
  } else if (sex1 == 'f') {
    Serial.println("Female");
    maxHeartRate1 = (190.2 / (1 + exp(0.0453*(age1-107.5))));
  }
  //Calculate max heart rate for player 2
  if (sex2 == 'm') {
    Serial.println("Male");
    maxHeartRate2 = (203.7 / (1 + exp(0.033*(age2-104.3))));
  } else if (sex2 == 'f') {
    Serial.println("Female");
    maxHeartRate2 = (190.2 / (1 + exp(0.0453*(age2-107.5))));
  }

  callibrate(1);

  //Start timer
  timer.start();

  Serial.println(digitalRead(playerOne));

  while (digitalRead(playerOne) != 0) {
    if(timer.read() <= 10000) {
      if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
      //int signal = analogRead(PulseWire);
      int myBPM = pulseSensor.getBeatsPerMinute();

      double effort = 0.0;
      effort = (myBPM - avgRestingRate1) / (maxHeartRate1 - avgRestingRate1);
      if (effort < 0.0) {
        effort = 0.0;
      } else if (effort > 1.0) {
        effort = 1.0;
      }

      effortData.value = effort;
      }

    //Send data
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&effortData, sizeof(effortData));
    if (result == ESP_OK) {
      Serial.println("Sent Effort: ");
      count++;
      sumOfEffort += effortData.value;
      Serial.println(effortData.value);
    } else {
      Serial.print("Send failed. Error code: ");
      Serial.println(result);
    }

    delay(100);                    // considered best practice in a simple sketch. (was 20)
    } else {
      timer.stop();
      Serial.println("Player 1's turn ended");
      digitalWrite(winner2, HIGH);
      effortData.value = 0;
      esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&effortData, sizeof(effortData));
      if (result == ESP_OK) {
        Serial.println("Sent Effort: ");
        Serial.println(effortData.value);
      } else {
        Serial.print("Send failed. Error code: ");
        Serial.println(result);
      }
      delay(100);
    }
  }
  digitalWrite(winner2, LOW);

  averageEffort1 = sumOfEffort/count;
  sumOfEffort = 0;
  count = 0;
  Serial.println("Turn switched");
  callibrate(2);

  timer.start();
  while (digitalRead(playerTwo) != 0) {
    if(timer.read() <= 10000) {
      if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
      //int signal = analogRead(PulseWire);
      int myBPM = pulseSensor.getBeatsPerMinute();

      double effort = 0.0;
      effort = (myBPM - avgRestingRate1) / (maxHeartRate1 - avgRestingRate1);
      if (effort < 0.0) {
        effort = 0.0;
      } else if (effort > 1.0) {
        effort = 1.0;
      }

      effortData.value = effort;
      }

    //Send data
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&effortData, sizeof(effortData));
    if (result == ESP_OK) {
      Serial.println("Sent Effort: ");
      count++;
      sumOfEffort += effortData.value;
      Serial.println(effortData.value);
    } else {
      Serial.print("Send failed. Error code: ");
      Serial.println(result);
    }

    delay(100);                    // considered best practice in a simple sketch. (was 20)
    } else {
      Serial.println("Player 2's turn ended");
      digitalWrite(winner2, HIGH);
      effortData.value = 0;
      esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&effortData, sizeof(effortData));
      if (result == ESP_OK) {
        Serial.println("Sent Effort: ");
        Serial.println(effortData.value);
      } else {
        Serial.print("Send failed. Error code: ");
        Serial.println(result);
      }
      delay(100);
    }
  }
  effortData.value = 0;

  digitalWrite(switchOutput, LOW);
  digitalWrite(winner2, LOW);
  averageEffort2 = sumOfEffort/count;
  if (averageEffort1 > averageEffort2) {
    Serial.println("Player 1 wins!");
    digitalWrite(winner1, HIGH);
  } else if (averageEffort1 < averageEffort2) {
    Serial.println("Player 2 wins!");
    digitalWrite(winner2, HIGH);
  } else if (averageEffort1 == averageEffort2) {
    Serial.println("It's a tie!");
    digitalWrite(winner1, HIGH);
    digitalWrite(winner2, HIGH);
  }

  // Serial.print("Starting in...");
  // delay(1000);
  // Serial.print("3..");
  // delay(1000);
  // Serial.print("2..");
  // delay(1000);
  // Serial.print("1..");
  // delay(1000);
  // Serial.println("Start!");

}



void loop() {
  // if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
  //   //int signal = analogRead(PulseWire);
  //   int myBPM = pulseSensor.getBeatsPerMinute();

  //   double effort = 0.0;
  //   effort = (myBPM - avgRestingRate) / (maxHeartRate - avgRestingRate);
  //   if (effort < 0.0) {
  //     effort = 0.0;
  //   } else if (effort > 1.0) {
  //     effort = 1.0;
  //   }

  //   effortData.value = effort;
  // }

  // //Send data
  // esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&effortData, sizeof(effortData));
  // if (result == ESP_OK) {
  //   Serial.println("Sent Effort: ");
  //   Serial.println(effortData.value);
  // } else {
  //   Serial.print("Send failed. Error code: ");
  //   Serial.println(result);
  // }

  // delay(100);                    // considered best practice in a simple sketch. (was 20)
}
