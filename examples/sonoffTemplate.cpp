//#include "sonoff.h"
#include "button.h"

// TODO add ability to set MQTT server and hostname from wifiManager

// /*
//   NOTE SONOFF LEDs are opposite, HIGH for off and LOW for on
// */

// // Update these with values suitable for your network.
// IPAddress mqtt_server(192,168,1,200);
// #define HOSTNAME      "SONOFF"
// #define OTA_PASSWORD  "PASSWORD"      // Password to perform OTA updates

// #define STRIPLIGHT_CMD "cmd"
// #define STRIPLIGHT_STATE "state"

// sonoffChannel ch1(SO_CH1);

// void setupSONOFFS(){
//   /*
//     Static setup functions that define attributes of all SONOFF objects
//   */
//   sonoff.setHostname(HOSTNAME); // Defaults to SONOFF_XXXX
//   sonoff.setWifiManagerTimeout(120);
//   sonoff.attachWifiManager();  // Optional built in wifi handling, you can use your own
//   sonoff.setMqttServer(mqtt_server);   // Builtin PubSubClient with default port
//   sonoff.sonoffOTA(OTA_PASSWORD);  // Set up OTA with some defaults

//   ch1.setLedMaxBrightness(PWMRANGE/2);

//   ch1.setCommandTopic(STRIPLIGHT_CMD);
//   ch1.setStateTopic(STRIPLIGHT_STATE);
//   ch1.beginMqtt();

//   //ch1.setWemoName("Desk Lamp");
//   //ch1.beginWemo();

//   sonoff.addChannel(&ch1);
//   //Serial.println(ch1.getSettings());
// }

Button b(0);

volatile int x = 0;

button_callback_t c1 = []() {
  x = 1;
};

button_callback_t c2 = []() {
  x = 2;
};

void setup() {
  Serial.begin(115200); Serial.println("\r\n\r\n");
  //setupSONOFFS();
  b.pressCallback(c1);
  b.holdCallback(c2);
  
  configTime(5 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // GMT+5 = central
}

void loop() {
  Serial.println(x);
  delay(500);
  // sonoff.handle();  // Needed rn to make OTA work
  // ch1.handle();
  // ArduinoOTA.handle();
}
