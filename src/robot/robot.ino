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

#define telemetry_ratio 64
#define timeout 20  // 5 * 1000 / 250, safe factor * 1 second / frequency of update

int motorL_val;
int motorR_val;
int motorS_val;

// Ground station vals
bool is_timeout = false;

uint8_t gsAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };  // find the mac of the s2
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
int led_val = 0;  // Chg for prod

esp_now_peer_info_t peerInfo;

bool new_data = false;  //data block

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(ESP_NOW_SEND_SUCCESS ? "Sent telemetry" : "Failed sending telemetry");  // look at perfomance
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incoming, int len) {
  memcpy(&gsVals, incoming, sizeof(gsVals));
  new_data = true;
};

void setup() {
  Serial.begin(115200);

  ledcSetup(0, 50, 16);
  ledcSetup(1, 50, 16);
  ledcSetup(2, 50, 16);

  ledcSetup(3, 50, 16);

  ledcAttachPin(motorL_pin, motorL_channel);
  ledcAttachPin(motorR_pin, motorR_channel);
  ledcAttachPin(motorS_pin, motorS_channel);

  ledcAttachPin(ledPin, led_channel);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init fail.");  // look at perfomance
    return;
  };

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  memcpy(peerInfo.peer_addr, gsAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    // look at perfomance
    return;
  };

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

uint8_t i = 0;  //telemetry increment

void loop() {
  i++;

  unsigned long start = millis();
  while (!new_data) {
    if (millis() - start >= timeout) {
      is_timeout = true;
      break;
    }
  };

  if (is_timeout) {
    ledcWrite(motorL_channel, 0);
    ledcWrite(motorR_channel, 0);
    ledcWrite(motorS_channel, 0);

    Serial.println("Timeout");  // look at perfomance
  } else {
    getAxes();
    ledcWrite(motorL_channel, motorL_val);
    ledcWrite(motorR_channel, motorR_val);
    ledcWrite(motorS_channel, motorS_val);

    ledcWrite(led_channel, led_val);
  }

  new_data = false;

  if (i >= telemetry_ratio) {
    esp_err_t result = esp_now_send(gsAddress, (uint8_t *)&telemetry, sizeof(telemetry));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    } else {
      Serial.println("Error sending the data");
    }

    i = 0;
  }
}

void getAxes() {
  motorL_val = gsVals.motorL_val;
  motorR_val = gsVals.motorR_val;
  motorS_val = gsVals.motorS_val;
}