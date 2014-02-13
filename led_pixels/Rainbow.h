#ifndef _RAINBOW_H_
#define _RAINBOW_H_

#include "Actor.h"

// Swirl a rainbow animation

//#ifndef WS2801_GRB
//#include "SPI.h"
//#include "Adafruit_WS2801.h"
//#endif

#define RAINBOW_FADE_IN (1 << 16)
#define RAINBOW_STYLE 0xff

class Rainbow : public Actor
{
  public:
    Rainbow(LEDFader &fader, double duration, int rainbowStyle);
    ~Rainbow();
    virtual bool loop(unsigned long delta);

  private:
    int _rainbowStyle;

};


#endif

