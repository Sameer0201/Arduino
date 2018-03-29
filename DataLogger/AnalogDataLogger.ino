
/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
namespace { 
  const char * WIFISSID = ""; // Assign your WiFi SSID 
  const char * PASSWORD = ""; // Assign your WiFi password
  const char * TOKEN = ""; // Assign your Ubidots TOKEN
}

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/ 
void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  client.readData(); // Reads the command from the logger
  delay(1000);
}
