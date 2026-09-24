#include <Arduino.h>
#include "driver/twai.h"
#include <unordered_map>
#include <vector>
#include <WiFi.h>

using namespace std;

#define TX_PIN 32
#define RX_PIN 33

  WiFiServer server(80);

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
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
   while (client.connected() && !client.available()){
    delay(1);
   }

    String test = "None";

   while (client.available()) {
    test = client.readStringUntil('\n');
   }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();                       // <-- blank line, required
    client.println("<!DOCTYPE html><html><body>");
    client.println("<h1>PCAN Light Control</h1>");
    client.println("<p>Hello from the ESP32</p>");
    client.print("<input>TEST</input>");
    client.println("</body></html>");

    client.stop();
  }
}

// put function definitions here:



