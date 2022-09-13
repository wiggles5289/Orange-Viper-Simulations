//Scrolling Indicator for F-16 FFI for DCS BIOS using generic 128x32 SSD1306 !!!WITH I2C!!! display(tested on 0.91" screen from Amazon)
//Final code by JohnnyChicago and slightly modified/tested by Wiggles5289 on the DCS Forums
//Can work for Altimeter/Fuel QTY
//WIP on GitHub
//Updated for DCS 2.7 on 13SEP2022
//Working on making the font higher definition for future OLEDs with higher pixel density to eliminate the ugly screen door effect
//COMMENT OUT SECTIONS YOU DO NOT NEED AND READ THE COMMENTS BEFORE USE!
//REQUIRED FOR COMPILING: Adafruit SSD1306, Adafruit GFX, and dcs-bios-arduino library
//Updates Found at: https://github.com/wiggles5289/Orange-Viper-Simulations/blob/main/F16/FFI


//#define DCSBIOS_FOR_STM32

#ifdef DCSBIOS_FOR_STM32 //This sets Default Serial for STM32, will not compile with IRQ without an error
#define DCSBIOS_DEFAULT_SERIAL
#ifndef DCSBIOS_FOR_STM32 //This sets IRQ Serial as the default option for Arduino Nano/Uno, its much faster than Default Serial
#define DCSBIOS_IRQ_SERIAL
#endif

#include "DcsBios.h"

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "characters.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

//Comment for barometric pressure
#define FUELFLOW

int updateInterval = 100; //the number of milliseconds between updates(17 for 60FPS, 41 for 24FPS, 100 for Default)

struct scrollDigit {
  int digit; //The digit that is displayed
  int y; // The vertical position of the digit
};

struct disp {
  Adafruit_SSD1306 display;
  int width;
  int numberOfDigits;
  scrollDigit digits[5];
};

#ifdef FUELFLOW
disp oled = {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 24, 5, {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
#else
disp oled = {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 16, 4, {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}};
#endif

void setup() {
  if (!oled.display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    for (;;); // Don't proceed, loop forever
  }

  DcsBios::setup();
}

void UpdateDisplay()
{
  oled.display.clearDisplay();
  for (int i = 0; i < oled.numberOfDigits; i++)
  {
    printScrollingDigit(oled.digits[i].digit, oled.width, oled.digits[i].y, i + 1, &oled);
  }
  //Clear the area below the the numbers if we are using the small font
  if (oled.width == 16)
  {
    oled.display.fillRect(0, 25, 67, 7, BLACK);
  }

  oled.display.display();
}

int YPos()
{
  return (oled.width + 9) * -1;
}

void printScrollingDigit(int digit, int width, int y, int pos, disp *oled)
{
  int x = (width * pos) - width + pos;
#ifdef FUELFLOW
  switch (digit)
  {
    case -1: oled->display.drawBitmap(x, y, c24_Empty, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_1, 24, 32, 1); break;
    case 1: oled->display.drawBitmap(x, y, c24_1, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_2, 24, 32, 1); break;
    case 2: oled->display.drawBitmap(x, y, c24_2, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_3, 24, 32, 1); break;
    case 3: oled->display.drawBitmap(x, y, c24_3, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_4, 24, 32, 1); break;
    case 4: oled->display.drawBitmap(x, y, c24_4, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_5, 24, 32, 1); break;
    case 5: oled->display.drawBitmap(x, y, c24_5, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_6, 24, 32, 1); break;
    case 6: oled->display.drawBitmap(x, y, c24_6, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_7, 24, 32, 1); break;
    case 7: oled->display.drawBitmap(x, y, c24_7, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_8, 24, 32, 1); break;
    case 8: oled->display.drawBitmap(x, y, c24_8, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_9, 24, 32, 1); break;
    case 9: oled->display.drawBitmap(x, y, c24_9, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_0, 24, 32, 1); break;
    default: oled->display.drawBitmap(x, y, c24_0, 24, 32, 1); oled->display.drawBitmap(x, y + 33, c24_1, 24, 32, 1); break;
  }
#else
  switch (digit)
  {
    case -1: oled->display.drawBitmap(x, y, c16_Empty, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_1, 16, 24, 1); break;
    case 1: oled->display.drawBitmap(x, y, c16_1, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_2, 16, 24, 1); break;
    case 2: oled->display.drawBitmap(x, y, c16_2, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_3, 16, 24, 1); break;
    case 3: oled->display.drawBitmap(x, y, c16_3, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_4, 16, 24, 1); break;
    case 4: oled->display.drawBitmap(x, y, c16_4, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_5, 16, 24, 1); break;
    case 5: oled->display.drawBitmap(x, y, c16_5, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_6, 16, 24, 1); break;
    case 6: oled->display.drawBitmap(x, y, c16_6, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_7, 16, 24, 1); break;
    case 7: oled->display.drawBitmap(x, y, c16_7, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_8, 16, 24, 1); break;
    case 8: oled->display.drawBitmap(x, y, c16_8, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_9, 16, 24, 1); break;
    case 9: oled->display.drawBitmap(x, y, c16_9, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_0, 16, 24, 1); break;
    default: oled->display.drawBitmap(x, y, c16_0, 16, 24, 1); oled->display.drawBitmap(x, y + 25, c16_1, 16, 24, 1); break;
  }
#endif
}

#ifdef FUELFLOW
void onFuelflowcounter10kChange(unsigned int newValue)
{
  if (newValue == 0)
  {
    oled.digits[0].digit = -1;
    oled.digits[0].y = 0;
  }
  else
  {
    unsigned int mappedValue = newValue / 6553;
    unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());

    oled.digits[0].digit = mappedValue;
    oled.digits[0].y = y;
  }
}

void onFuelflowcounter1kChange(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6553;
  unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());

  oled.digits[1].digit = mappedValue;
  oled.digits[1].y = y;
}

void onFuelflowcounter100Change(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6553;
  unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());

  oled.digits[2].digit = mappedValue;
  oled.digits[2].y = y;
}
DcsBios::IntegerBuffer fuelflowcounter100Buffer(0x44e8, 0xffff, 0, onFuelflowcounter100Change);
DcsBios::IntegerBuffer fuelflowcounter10kBuffer(0x44e4, 0xffff, 0, onFuelflowcounter10kChange);
DcsBios::IntegerBuffer fuelflowcounter1kBuffer(0x44e6, 0xffff, 0, onFuelflowcounter1kChange);

//#else

#endif

unsigned long time = 0;

void loop() {
  DcsBios::loop();

  time = millis();
  if (time % updateInterval == 0)
  {
    UpdateDisplay();
  }
}
