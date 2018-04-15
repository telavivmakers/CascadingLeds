#include <Arduino.h>
#include <FastLED.h>

#define STRIPTYPE NEOPIXEL

constexpr bool backwards = false; // change to true if the leds are connected from the top
constexpr size_t blanks = 5;
constexpr uint8_t stripPin = 2;
constexpr byte intesity = 255;


constexpr size_t patternSize = 8*(6+blanks);
constexpr size_t stripSize = 35;

constexpr size_t ledArraySize = patternSize + stripSize - 1;


CRGB colors[ledArraySize];

void createColorGradiante(CRGB *&pPixel, byte hue)
{
    constexpr byte step = backwards ? 51 : -51;
    byte val = backwards ? 0 : 255; 
    for (size_t i=0; i < 6; i++, val+=step, pPixel++) {
        *pPixel = CHSV(hue, 255, val);
    }
    for (size_t i=0; i<blanks; i++, pPixel++) {
        *pPixel = CRGB(0,0,0);
    }
}

void createRainbow(CRGB *pPixel)
{
    for (byte hue = 0, i=0 ; i<8 ; i++, hue += (32*5)) {
        createColorGradiante(pPixel, hue);
        //createColorGradiante(pPixel, hue + 128);
    }
}


void setup()
{
    createRainbow(colors);
    ptrdiff_t ledsLeft = ledArraySize - patternSize;
    CRGB *currentLed = colors + patternSize;
    do {
        memcpy(currentLed, colors, min(patternSize,size_t(ledsLeft))*sizeof(CRGB));
        ledsLeft -= patternSize;
        currentLed += patternSize;
    } while (ledsLeft > 0);
    FastLED.addLeds<STRIPTYPE, stripPin>(colors, int(stripSize));
}

size_t currentStep = 0;

inline size_t startingPoint(size_t step)
{
    return backwards ? patternSize - 1 - step : step;
}

void loop() {
    FastLED[0].show(colors + startingPoint(currentStep), stripSize, intesity);
    currentStep++;
    if (currentStep >= patternSize) {
        currentStep = 0;
    }
    delay(150);
}
