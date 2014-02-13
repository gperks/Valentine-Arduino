#include "Actor.h"

Actor::Actor(LEDFader &fader, double duration)
: _fader(fader)
, _transitionDuration(duration*1000)
, _transitionElapsed(0)
{
}

