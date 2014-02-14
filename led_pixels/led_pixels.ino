
#include "SPI.h"
#include "Adafruit_WS2801.h"

#include "frames.h"
#include "LEDFader.h"
#include "Message.h"
#include "Pause.h"
#include "Rainbow.h"


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

Actor *CurrentActor;

typedef void (*AnimationEndedCallback)();
typedef void (*CurrentAnimationFP)(unsigned long);
CurrentAnimationFP CurrentAnimation;


double transitionDuration; // in millis
AnimationEndedCallback transitionCompleteCallback;
double transitionElapsed; // millis taken by transition so far


byte* alphabetFrameFromChar(char letter)
{
  if (letter == 32) {
//    Serial.println("space");
    return AllDark;
  }
  else 
    return Alphabet[letter - 'A'];
}
    

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
      time += duration / (GRIDSIZE * GRIDSIZE); // fade over duration
    }
  }
}

void allOffRowByRow(double duration)
{
  double time = 0.1;
  for (int y = 0; y < GRIDSIZE; y++) {
      time += duration / GRIDSIZE;
    for (int x = 0; x < GRIDSIZE; x++) {
      Fader.setPixelColor(x, y, Color(0, 0, 0), time);
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
//  { pause, 3 },
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//
//  { allOffBitByBit, 1 },
//
//  // "I <heart> U"
//  { animateToLetterI, 0.2 },
//  { pause, 1.5 },
//  { animateToBigHeart, 0.2},
//  { pause, 1 },
//  { animateToLetterU, 0.2 },
//  { pause, 1.5 },
//
//  { allOffRowByRow, 2 },
//
//  // Beating heart
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},
//  { animateToSmallHeart, 0.4 },
//  { animateToBigHeart, 0.5},

  { beMineMessage },

  { allOffBitByBit, 2 },
//  { allOff, 0.5 },
  { pause, 0.5 },

  // Color swirls
//  { rainbowAnimation, 15, 1 },
//  { allOffRowByRow, 1 },
  { rainbowAnimation, 3, 0 },

  { allOffRowByRow, 2 },
  //{ pause, 0.5 },
};

void setup() {

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

  Serial.begin(19200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

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

// Helper function as so many things include a pause.
void pause(double duration)
{
  CurrentActor = new Pause(Fader, duration);
}

void animateToSmallHeart(double duration, uint32_t param)
{
  transitionToFrame(SmallHeart, Color(200, 0, 0));
  pause(duration);
}

void animateToBigHeart(double duration, uint32_t param)
{
  transitionToRedFrame(BigHeart);
  pause(duration);
}

void animateToLetterI(double duration, uint32_t param)
{
  transitionToFrame(FontI, Color(0, 0, 150));
  pause(duration);
}

void animateToLetterU(double duration, uint32_t param)
{
  transitionToFrame(FontU, Color(0, 0, 150));
  pause(duration);
}

void pause(double duration, uint32_t param)
{
  pause(duration);
}

void rainbowAnimation(double duration, uint32_t param)
{
  int style = param;
  Rainbow *bow = new Rainbow(Fader, duration, style);
  CurrentActor = bow;
}

void allOff(double duration, uint32_t param)
{
  allOff();
  pause(duration);
}

void allOffBitByBit(double duration, uint32_t param)
{
  allOffBitByBit(duration);
  pause(duration);
}

void allOffRowByRow(double duration, uint32_t param)
{
  allOffRowByRow(duration);
  pause(duration);
}

void beMineMessage(double duration, uint32_t param)
{
  Message *msg = new Message(Fader, " BE MY VALENTINE");
  CurrentActor = msg;
}

//////////////

void loop() {

  unsigned long currentMillis = millis();

  unsigned long delta = currentMillis - previousMillis;

  if (delta > frameDelayMillis) {
    previousMillis = currentMillis;
    Fader.loop(delta); // render frame to leds
    if (CurrentActor) {
      bool rc = CurrentActor->loop(delta);
      // actor completed?
      if (rc) {
        delete CurrentActor;
        CurrentActor = 0;
        nextAnimation();
      }
    }
    else 
      CurrentAnimation(delta); // create next frame
  }
}




