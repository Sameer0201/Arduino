#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>

#include <Adafruit_GFX.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File myFile;
String toMatch = "aloo";
String bufferToFill;
char* ssid = "";
char* password = "";
char input[65];
void setup() {
  Wire.begin();
  Serial.begin(9600);

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  //to write
  myFile = SD.open("password.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("Aloo");
    myFile.close();
  }
  else {
    Serial.println("Error opening and writing file");
  }

  //read File
  myFile = SD.open("password.txt");
  if(myFile){
    while(myFile.available()){
      //bufferToFill = myFile.read();
      Serial.print(myFile.read()); 
      }
    }
}

void loop() {
  Wire.requestFrom(8, 65);
  int i = 0;
  while (Wire.available()) {
    char c = Wire.read();
    input[i++] = c;
    Serial.print(c);
  }
  delay(1000);
  ssid = strtok(input, ",");
  password = strtok(input, ",");
  Serial.println(ssid);
  Serial.println(password);
}
