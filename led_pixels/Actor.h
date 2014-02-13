#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "LEDFader.h"

class Actor
{
  public:
  Actor(LEDFader &fader, double duration);
  
  // Returns true if this animation is complete.
  virtual bool loop(unsigned long delta) = 0;
  virtual ~Actor() {}
  
  protected:
    LEDFader &_fader;
    double _transitionDuration; // in millis
    double _transitionElapsed; // millis taken by transition so far
    
    int _gridWidth;
    int _gridHeight;
};

#endif

