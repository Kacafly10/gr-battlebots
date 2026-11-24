#include <esp_now.h>
#include <WiFi.h>

#define motorL_pin 0
#define motorR_pin 1
#define motorS_pin 21

#define ledPin 8

#define motorL_channel 0
#define motorR_channel 1
#define motorS_channel 2

#define led_channel 3

// Ground station vals
bool is_connected = true; //make sure to make false in prod!!!

uint8_t gsAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // find the mac of the s2
bool success;

typedef struct control_struct {
  int motorL_val;
  int motorR_val;
  int motorS_val;
} control_struct;

typedef struct telemetry_struct {
  bool recieved;
} telemetry_struct;

control_struct gsVals;
telemetry_struct telemetry;

//led?
int led_val = 0; // Chg for prod

esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t * mac, const uint8_t * incoming, int len) {
};

void setup() {
  ledcSetup(0, 50, 16);
  ledcSetup(1, 50, 16);
  ledcSetup(2, 50, 16);

  ledcSetup(4, 50, 16);

  ledcAttachPin(motorL_pin, motorL_channel);
  ledcAttachPin(motorR_pin, motorR_channel);
  ledcAttachPin(motorS_pin, motorS_channel);

  ledcAttachPin(ledPin, led_channel);
}

void loop() {
  if (!is_connected) { return; };

  ledcWrite(motorL_channel, gsVals.motorL_val);
  ledcWrite(motorR_channel, gsVals.motorR_val);
  ledcWrite(motorS_channel, gsVals.motorS_val);

  led_val += 256;
  ledcWrite(led_channel, led_val);
}