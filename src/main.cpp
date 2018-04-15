#include <Arduino.h>
/*#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>
*/

#include <FastLED.h>

constexpr size_t blanks = 5;
constexpr size_t patternSize = 8*(6+blanks);
constexpr size_t stripSize = 35;

constexpr size_t ledArraySize = patternSize + stripSize - 1;
constexpr uint8_t stripPin = 2;

CRGB colors[ledArraySize];

void createColorGradiante(CRGB *&pPixel, byte hue)
{
    for (int i=255; i>=0; i-=51, pPixel++) {
        *pPixel = CHSV(hue, 255, byte(i));
    }
    for (size_t i=0; i<blanks; i++, pPixel++) {
        *pPixel = CRGB(0,0,0);
    }
}

void createRainbow(CRGB *pPixel)
{
    for (byte hue = 0 ; hue < 128 ; hue += 32) {
        createColorGradiante(pPixel, hue);
        createColorGradiante(pPixel, hue + 128);
    }
}

#define COLOR_ORDER RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.

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
    FastLED.addLeds<NEOPIXEL, stripPin>(colors, int(stripSize));
}

size_t currentStartPoint = 0;

void loop() {
    FastLED[0].show(colors + currentStartPoint, stripSize, 255);
    currentStartPoint++;
    if (currentStartPoint >= patternSize) {
        currentStartPoint = 0;
    }
    delay(150);
}
