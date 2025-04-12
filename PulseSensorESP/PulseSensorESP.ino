#include <PulseSensorPlayground.h>
#include <WiFi.h>
#include <esp_now.h>
#include "BPM.h"

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

//Wifi settings
uint8_t receiverMAC[] = {0x34, 0xb7, 0xda, 0xf6, 0x41, 0x98};
bpm bpmData;

void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {

  Serial.begin(115200);          // For Serial Monitor

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
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
}



void loop() {

 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
  int signal = analogRead(PulseWire);
  int myBPM = pulseSensor.getBeatsPerMinute();
  Serial.print(signal);
  Serial.print(",");
  Serial.println(myBPM);                      // Print the value inside of myBPM. 
  bpmData.value = myBPM;
  }

  //Send data
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&bpmData, sizeof(bpmData));
  if (result == ESP_OK) {
    Serial.println("Sent BPM: ");
    Serial.println(bpmData.value);
  } else {
    Serial.print("Send failed. Error code: ");
    Serial.println(result);
  }

  delay(100);                    // considered best practice in a simple sketch. (was 20)

}
