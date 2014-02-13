#include "Pause.h"

Pause::Pause(LEDFader &fader, double duration) 
: Actor(fader, duration)
{
}

Pause::~Pause()
{
}

bool Pause::loop(unsigned long delta)
{
  if (_transitionElapsed < _transitionDuration) {
    _transitionElapsed += delta;

    return false;
  }
  else {
    // Animation complete.
    return true;
  }
}
 
