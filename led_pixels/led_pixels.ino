
#include "SPI.h"
#include "Adafruit_WS2801.h"

#include "frames.h"
#include "LEDFader.h"

/**

  Designed to work with the Adafruit RGB Pixels.
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

*/

uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

#define GRIDSIZE 5 // We have a 5x5 LED array


unsigned long previousMillis = 0;
unsigned long frameDelayMillis = 5; // time between frames

// Set the first variable to the NUMBER of pixels.
Adafruit_WS2801 strip = Adafruit_WS2801(GRIDSIZE*GRIDSIZE, dataPin, clockPin);
LEDFader Fader(strip, GRIDSIZE, GRIDSIZE);

typedef void (*AnimationEndedCallback)();
typedef void (*CurrentAnimationFP)(unsigned long);
CurrentAnimationFP CurrentAnimation;


double transitionDuration; // in millis
AnimationEndedCallback transitionCompleteCallback;
double transitionElapsed; // millis taken by transition so far


/**
 Transition to a frame of colors.
 @param duration in seconds
 */
void transitionToFrame(byte* frame, uint32_t color)
{
  for (int y = 0; y < GRIDSIZE; y++) {
    byte b = frame[y];
    for (int x = 0; x < GRIDSIZE; x++) {
      byte bit = b & (1 << x);
      Fader.setPixelColor(x, y, bit ? color : Color(0, 0, 0), 0.2);
    }
  }
}

void transitionToRedFrame(byte* frame)
{
  transitionToFrame(frame, Color(128, 0, 0));
}

void transitionToColorFrame(byte* frame, uint32_t color)
{
  transitionToFrame(frame, color);
}


////////////
// Pause animation

void pause_animation(unsigned long delta)
{
  if (transitionElapsed < transitionDuration) {
    transitionElapsed += delta;

    //    // clamp  - no need in this simple pause.
    //    if (transitionElapsed > transitionDuration)
    //      transitionElapsed = transitionDuration;
  }
  else {
    transitionCompleteCallback();
  }
}

// duration in seconds
void pause(double duration, void (*completionCallback)())
{
  CurrentAnimation = &pause_animation;
  transitionElapsed = 0;
  transitionDuration = duration * 1000; // convert to millis
  transitionCompleteCallback = completionCallback;
}

////////////
// Swirl a rainbow animation

// Color wheel yellow->red->blue->green
// Wheel from http://blog.asmartbear.com/color-wheels.html
// wheelPos: ranges 0..1
uint32_t ColorWheel(double wheelPos)
{
  const double MaxBrightness = 100;
  byte r, g, b;
  if (wheelPos < 0) {
    // Shouldn't get here
    Serial.println("wheelPos < 0");
    r = g = b = 0;
  }
  else if (wheelPos < 0.25) {
    // Yellow to red
    double factor = MaxBrightness * 4 * wheelPos;
    r = MaxBrightness;
    g = MaxBrightness - factor;
    b = 0;
  }
  else if (wheelPos < 0.5) {
    // Red to Blue
    double factor = MaxBrightness * 4 * (wheelPos - 0.25);
    r = MaxBrightness - factor;
    g = 0;
    b = factor;
  }
  else if (wheelPos < 0.75) {
    // Blue to green
    double factor = MaxBrightness * 4 * (wheelPos - 0.5);
    r = 0;
    g = factor;
    b = MaxBrightness - factor;
  }
  else if (wheelPos <= 1) {
    // Green to yellow
    double factor = MaxBrightness * 4 * (wheelPos - 0.75);
    r = factor;
    g = MaxBrightness;
    b = 0;
  }
  else {
    // Shouldn't get here
    Serial.println("wheelPos > 1");
    r = g = b = 0;
  }

  //  char buf[100];
  //  char fbuf[20];
  //  dtostrf(wheelPos, 5, 2, fbuf);
  //  sprintf(buf, "%s: %d, %d, %d", fbuf, (int)r, (int)g, (int)b);
  //  Serial.println(buf);


  return Color(r, g, b);
}

#define RAINBOW_FADE_IN (1 << 16)
#define RAINBOW_STYLE 0xff
int rainbowStyle;

void rainbow_animation(unsigned long delta)
{
  if (transitionElapsed < transitionDuration) {
    transitionElapsed += delta;

    // clamp
    if (transitionElapsed > transitionDuration) {
      transitionElapsed = transitionDuration;
    }

    const int rainbowSize = 10000; // number of milliseconds rainbow unfolds over
    const double rainbowLength = (rainbowStyle & RAINBOW_STYLE) ? 0 : 0.3; // How far across time the last pixel is from the first.
    double frame = (long)transitionElapsed % rainbowSize;

    for (int y = 0; y < GRIDSIZE; y++) {
      for (int x = 0; x < GRIDSIZE; x++) {

        double wheelPos = frame / rainbowSize;
        double tail = rainbowLength * ((double)(y * GRIDSIZE + x) / (double)(GRIDSIZE * GRIDSIZE));
        wheelPos += tail;
        if (wheelPos > 1)
          wheelPos -= 1;

        uint32_t c = ColorWheel(wheelPos);

        Fader.setPixelColor(x, y, c, 0.1);
      }
    }
  }
  else {
    transitionCompleteCallback();
  }
}

// duration in seconds
void rainbow(double duration, void (*completionCallback)(), int style)
{
  // Configure the rainbow
  CurrentAnimation = &rainbow_animation;
  transitionElapsed = 0;
  transitionDuration = duration * 1000; // convert to millis
  transitionCompleteCallback = completionCallback;
  rainbowStyle = style;
}

///////

void allOff()
{
  transitionToFrame(AllDark, Color(0, 0, 128));

  //    for (int y=0;y<GRIDSIZE;y++) {
  //      for (int x=0;x<GRIDSIZE;x++) {
  //        strip.setPixelColor(y * GRIDSIZE + x, 0);
  //      }
  //    }
  //
  //    strip.show();
}


void allOffBitByBit(double duration)
{
  double time = 0;
  for (int y = 0; y < GRIDSIZE; y++) {
    for (int x = 0; x < GRIDSIZE; x++) {
      Fader.setPixelColor(x, y, Color(0, 0, 0), time);
      time += duration / (GRIDSIZE * GRIDSIZE); // fade over 2s
    }
  }
}

/////////////


int animationFrame;

// This array defines the series of animations we'll do.
typedef void(*AnimationFunction)(double duration, uint32_t param);
struct Animation
{
  AnimationFunction func;
  double duration; // seonds
  uint32_t param;
};

Animation animations[] = {
  // Beating heart
  { animateToBigHeart, 0.2 },
  { pause, 3 },
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},

  { allOffBitByBit, 1 },

  // "I <heart> U"
  { animateToLetterI, 0.2 },
  { pause, 1.5 },
  { animateToBigHeart, 0.2},
  { pause, 1 },
  { animateToLetterU, 0.2 },
  { pause, 1.5 },

  { allOffBitByBit, 1 },

  // Beating heart
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},
  { animateToSmallHeart, 0.4 },
  { animateToBigHeart, 0.5},

  { allOffBitByBit, 2 },
//  { allOff, 0.5 },
  { pause, 0.5 },

  // Color swirls
  { rainbowAnimation, 1, 1 },
  { rainbowAnimation, 1, 0 },

  { allOffBitByBit, 2 },
  //{ pause, 0.5 },
};

void setup() {

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

  //  Serial.begin(19200);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for Leonardo only
  //  }


  animationFrame = 0;
  nextAnimation();
}

// Starts the next animation.
void nextAnimation()
{
  //  Serial.print("Proceeding to animation: ");
  //  Serial.println(animationFrame);
  //
  // Fetch the current animation, invoke it.
  Animation a = animations[animationFrame++];
  a.func(a.duration, a.param);

  int countAnimations = sizeof(animations) / sizeof(struct Animation);

  if (animationFrame >= countAnimations) {
    animationFrame = 0;
  }
}

void animateToSmallHeart(double duration, uint32_t param)
{
  transitionToFrame(SmallHeart, Color(200, 0, 0));
  pause(duration, &nextAnimation);
}

void animateToBigHeart(double duration, uint32_t param)
{
  transitionToRedFrame(BigHeart);
  pause(duration, &nextAnimation);
}

void animateToLetterI(double duration, uint32_t param)
{
  transitionToFrame(FontI, Color(0, 0, 150));
  pause(duration, &nextAnimation);
}

void animateToLetterU(double duration, uint32_t param)
{
  transitionToFrame(FontU, Color(0, 0, 150));
  pause(duration, &nextAnimation);
}

void pause(double duration, uint32_t param)
{
  pause(duration, &nextAnimation);
}

void rainbowAnimation(double duration, uint32_t param)
{
  int style = param;
  rainbow(duration, &nextAnimation, style);
}

void allOff(double duration, uint32_t param)
{
  allOff();
  pause(duration, &nextAnimation);
}

void allOffBitByBit(double duration, uint32_t param)
{
  allOffBitByBit(duration);
  pause(duration, &nextAnimation);
}

//////////////

void loop() {

  unsigned long currentMillis = millis();

  unsigned long delta = currentMillis - previousMillis;

  if (delta > frameDelayMillis) {
    previousMillis = currentMillis;
    Fader.loop(delta);
    CurrentAnimation(delta);
  }
}




