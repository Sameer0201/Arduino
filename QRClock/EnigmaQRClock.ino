
// A QR Clock for the Arduino UNO + Seeed Touchscreen LCD platform
// by @arduinoenigma
// uses a modified @chooftech QR library and a modified Xark display driver
// update times of a second are possible
// http://ch00ftech.com/2013/04/23/optimizing-the-qr-clock/
// http://ch00ftech.com/wp-content/uploads/2012/10/main1.c
//
// to set without resetting, open a command prompt (cmd.exe) and type:
// echo "r1;h15;m22;s25;u10;d;x;" > comX:
// where X is the serial port number for the clock
//
// to set the time, open a serial monitor and type the following command
// i0;r1;h15;m22;s25;u10;d;x;
// i0 sets the normal color scheme, i1 inverts the colors.
// r1 sets rotation so plug is to the right. Any rotation is fine, setting is for purists
// h15 sets hours to 15
// m22 sets minutes to 22
// s10 sets seconds to 25
// u10 sets update rate to 10 seconds, 1,5,10,15,20,30,60 are valid
// d shows the time
// x starts the clock now
// if a command with a two digit parameter is sent with two digits, the semicolon can be omitted
// the following are valid strings
// r1h15m22s25u10dx
// r1h9;m5;s25u1;dx
//
//

/* type the lines below this one in a file called setqrclock.cmd
@echo off

rem change the line below with the serial port number for your arduino
rem include only the numeric part of the port.
set EnigmaQRPort=9

rem code from: http://stackoverflow.com/questions/4248220/how-can-i-retrieve-the-seconds-part-of-time-in-cmd

For /f "tokens=1-4 delims=/:." %%a in ("%TIME%") do (
    SET HH24=%%a
    SET MI=%%b
    SET SS=%%c
    SET FF=%%d
)

set EnigmaQRTime=i1;r2;h%HH24%;m%MI%;s%SS%;u1;x
echo %EnigmaQRTime% >> com%EnigmaQRPort%:
echo %EnigmaQRTime%
*/

#define TFTSeed
//#define TFTAdafruit

#include <SPI.h>
#include <FastTftILI9341.h>      //https://github.com/arduinoenigma/FastSeedTFTv2
#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>

#include <Adafruit_GFX.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
Adafruit_TFTLCD Tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//raw input string from user GLOBAL VAR to generate();
char input[17];

//GLOBAL VAR to generate() and printcode();
unsigned char outputmatrix[56] = {128, 63, 192, 247, 247, 137, 254, 34, 209, 95, 36, 250, 139, 124, 127, 31, 160, 10, 248, 255, 255, 191, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 251, 255, 255, 253, 63, 224, 255, 247, 253, 255, 162, 255, 95, 244, 255, 139, 254, 127, 223, 255, 15, 248, 255, 255};
unsigned long previousQR[21];

unsigned long ul_LastGenerated = 0UL;
unsigned long ul_UpdateMillis = 0UL;

byte hours = 0;
byte mins = 0;
byte secs = 0;
byte ampm = 0;
byte update = 1;
byte rotation = 0;
byte invert = 0;

byte xo = 15;
byte yo = 55;


void buildtimestring()
{
  byte index = 0;
  byte displayhours = hours;

  if (ampm)
  {
    if (hours == 0)
    {
      displayhours = 12;
    }
    if (hours > 12)
    {
      displayhours = hours - 12;
    }
  }

  input[index++] = (displayhours / 10) + '0';
  input[index++] = (displayhours - (displayhours / 10) * 10) + '0';
  input[index++] = ':';
  input[index++] = (mins / 10) + '0';
  input[index++] = (mins - (mins / 10) * 10) + '0';

  if (update != 60)
  {
    input[index++] = ':';
    input[index++] = (secs / 10) + '0';
    input[index++] = (secs - (secs / 10) * 10) + '0';
  }

  if (ampm)
  {
    input[index++] = ' ';

    if (hours < 12)
    {
      input[index++] = 'A';
    }
    else
    {
      input[index++] = 'P';
    }

    input[index++] = 'M';
  }

  input[index++] = ';';

  for (byte i = index; i < 17; i++)
  {
    input[i] = 0;
  }
}

void advancetime()
{
  secs += update;
  if (secs >= 60)
  {
    secs = secs - 60;
    mins++;
    if (mins == 60)
    {
      mins = 0;
      hours++;
      if (hours == 24)
      {
        mins = 0;
        hours = 0;
      }
    }
  }
}

void clearScreen()
{
  uint16_t color = 0;

  clearPrevQR();

  if (invert)
  {
    color = WHITE;
  }
  else
  {
    color = BLACK;
  }

  Tft.fillScreen(color);
}

void clearPrevQR()
{
  for (byte i = 0; i < 21; i++)
  {
    previousQR[i] = 0;
  }
}

void setup() {
  Serial.begin(9600);
  Tft.reset();
  uint16_t identifier = Tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }
 
  Tft.begin(identifier);

  Serial.println(F("valid init string is: i0;r2;h15;m22;s25;u1;d;x;"));

  ul_UpdateMillis = update * (unsigned long)1000;

  buildtimestring();
  generate();
  ul_LastGenerated = millis();

  clearScreen();
  printcode(&outputmatrix[0]);

  advancetime();
  buildtimestring();
  generate();
}


void loop() {

  unsigned long ul_CurrentMillis;
  ul_CurrentMillis = millis();

  SerialFSM();

  if ((ul_CurrentMillis - ul_LastGenerated) >= ul_UpdateMillis)
  {
    ul_LastGenerated += ul_UpdateMillis;
    printcode(&outputmatrix[0]);
    advancetime();
    buildtimestring();
    generate();
  }

}
