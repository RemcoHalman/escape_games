#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// 25 Eifeltoren
// 24 Hart
// 20 Ringen
// 20 Huis
// 18 Ziektebed
// 18 Graf
// 19 Traan
// 34 Mes

#define PIXEL_COUNT 175
#define PIXEL_PIN 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Color Setup for LED strip
uint32_t RED = strip.Color(255, 0, 0);   // Red
uint32_t GREEN = strip.Color(0, 255, 0); // Green
uint32_t BLUE = strip.Color(0, 0, 255);  // Blue
uint32_t ORANGERED = strip.Color(210, 105, 30);
uint32_t MIDNIGHTBLUE = strip.Color(25, 25, 112);
uint32_t MINTCREAM = strip.Color(255, 245, 255);
uint32_t SILVER = strip.Color(100, 150, 100);
uint32_t PALEVIOLETRED = strip.Color(147, 219, 112);
uint32_t SEAGREEN = strip.Color(87, 46, 139);
uint32_t YELLOW = strip.Color(255, 215, 0);
uint32_t BLACK = strip.Color(0, 0, 0);

const int eiffelToren = 0;
const int hart = 25;
const int ringen = 49;
const int huis = 69;
const int ziekteBed = 89;
const int graf = 107;
const int traan = 125;
const int mes = 144;

// 25 Eifeltoren
// 24 Hart
// 20 Ringen
// 20 Huis
// 18 Ziektebed
// 18 Graf
// 19 Traan
// 34 Mes

void setup()
{
  strip.begin();
  strip.clear();
  strip.setBrightness(19);
  strip.show();
}
uint8_t brightness = 255;

void colorFade(uint32_t COLOR, int STARTLED, int LICHTCOUNT, int BRIGHTNESS, int wait) // optional addind brightness
{
  for (int LED = 0; LED < LICHTCOUNT;)
  { //  For each pixel in strip...
    strip.fill(COLOR, STARTLED, LICHTCOUNT);
    strip.show(); //  Update strip to match
    // delay(wait);  //  Pause for a moment, int wait
    for (int i = 1; i < BRIGHTNESS; i++)
    {
      strip.setBrightness(i);
      strip.show();
      delay(wait);
    }
    for (int i = BRIGHTNESS; i > 1; i--)
    {
      strip.setBrightness(i);
      strip.show();
      delay(wait);
    }
    break;
  }
}

uint32_t loopCount = 0;

void loop()
{
  // Color, StartLED, LED's to fill, Brightness, Delay
  while (loopCount < 5)
  {
    colorFade(RED, eiffelToren, 25, 7, 140);
    colorFade(ORANGERED, hart, 24, 7, 140);
    colorFade(BLUE, ringen, 20, 7, 140);
    colorFade(MINTCREAM, huis, 20, 7, 140);
    colorFade(BLACK, eiffelToren, 25, 7, 140);
    colorFade(YELLOW, ziekteBed, 18, 7, 140);
    colorFade(BLACK, hart, 24, 7, 140);
    colorFade(PALEVIOLETRED, graf, 18, 7, 140);
    colorFade(BLACK, ringen, 20, 7, 140);
    colorFade(PALEVIOLETRED, traan, 19, 7, 140);
    colorFade(BLACK, huis, 20, 7, 140);
    colorFade(RED, mes, 34, 7, 140);
    colorFade(BLACK, ziekteBed, 18, 7, 140);
    colorFade(BLACK, graf, 18, 7, 140);
    colorFade(BLACK, traan, 19, 7, 140);
    colorFade(BLACK, mes, 34, 7, 140);
    loopCount++;
  }
}

////////////////  OLD CODE ///////////////

// for (int i = 10; i < 100; i++)
// {
//   strip.setBrightness(i);
//   strip.show();
//   delay(15);
// }
// for (int i = 100; i > 10; i--)
// {
//   strip.setBrightness(i);
//   strip.show();
//   delay(15);
// }

// strip.setPixelColor(hart, RED);
// strip.setPixelColor(ringen, YELLOW);
// strip.setPixelColor(huis, GREEN);
// strip.setPixelColor(ziekteBed, PALEVIOLETRED);
// strip.setPixelColor(graf, SILVER);
// strip.setPixelColor(traan, MIDNIGHTBLUE);
// strip.setPixelColor(mes, BLUE);
// delay(500);
// strip.show();
