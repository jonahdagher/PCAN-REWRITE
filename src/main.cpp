#include <Arduino.h>
#include "driver/twai.h"
#include <unordered_map>
#include <vector>
#include <WiFi.h>
#include <WebServer.h>
#include <algorithm>

using namespace std;

#define TX_PIN 32
#define RX_PIN 33

WebServer server(80);

class LightSwitch {

//null means no change
private:
  int _state; // 0:off, 1:on, -1:null
  int _brightness; // Value betwen 1 - 100, -1:null

public:

  //default constructor
  LightSwitch(){
    _state = -1;
    _brightness = -1;
  }

  //state methods
  int getState(){
    return _state;
  }

  void setState(int state){
    _state = state;
  }

  int getBrightness(){
    return _brightness;
  }

  void setBrightness(int brightness){
    if (brightness == -1){
      _brightness = -1;
      return;
    }
    brightness = max(brightness, 0);
    brightness = min(brightness, 100);

    _brightness = brightness;
  }

  //TWAI

  twai_message_t toMessage(){
    twai_message_t msg = {};

    msg.extd = 1;
    msg.identifier = 0x100;
    msg.data_length_code = 2;

    msg.data[0] = _state;
    msg.data[1] = _brightness;

    return msg;
  }
};

void handleHome() {
  server.send(200, "text/html", "<h1>Home Page</h1>");
}

void handleTest() {
  
  String state = server.arg("state");
  String brightness = server.arg("brightness");
  Serial.println(state);
  Serial.println(brightness);
  server.send(200, "text/html", "State: " + state + " | Brightness: " + brightness);
}

void setup() {

  Serial.begin(115200);
  //TWAI setup
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
    (gpio_num_t)TX_PIN, 
    (gpio_num_t)RX_PIN, 
    TWAI_MODE_NO_ACK);

  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  //INSTALL
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK){
    Serial.println("Driver Install Success");
  }
  else{
    Serial.println("Driver Install Error");
  }

  //START
  if (twai_start() == ESP_OK){
    Serial.println("Driver Started");
  }
  else{
    Serial.println("Driver Failed to Start");
  }

  //CONNECT TO WIFI

  WiFi.begin("Resident Lyons Corner");
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connection Success");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  server.on("/", HTTP_GET, handleHome);
  server.on("/test", HTTP_GET, handleTest);
}


void loop() {
  server.handleClient();;
}

// put function definitions here:




