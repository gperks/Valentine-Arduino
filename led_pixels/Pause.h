#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "Actor.h"

class Pause : public Actor
{
  public:
    Pause(LEDFader &fader, double duration);
    ~Pause();
    virtual bool loop(unsigned long delta);

};

#endif

