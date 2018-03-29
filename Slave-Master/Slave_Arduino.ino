#include <Wire.h>
#include <Keypad.h>
const byte Rows = 4;
const byte Cols = 4;
char keys[Rows][Cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowsPin[Rows] = {9, 8, 7, 6}; // rows pin connect accordingly
byte colsPin[Cols] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowsPin, colsPin, Rows, Cols);
char input[65];// 32 for ssid 32 for password 1 for /0
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (Serial.available() > 0) {
    byte size = Serial.readBytesUntil('\n', input, 64);
    // the string should be adjusted to ssid,password,#####0
    input[size] = 0;
  }
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
char key = keypad.getKey();
if(key){
  Serial.println(key);
  }
}

void requestEvent() {
  Wire.write(input);
}
