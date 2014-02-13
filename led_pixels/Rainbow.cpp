#include "Rainbow.h"

static uint32_t ColorWheel(double wheelPos);

Rainbow::Rainbow(LEDFader &fader, double duration, int rainbowStyle) 
: Actor(fader, duration)
, _rainbowStyle(rainbowStyle)
{
}

Rainbow::~Rainbow()
{
}

bool Rainbow::loop(unsigned long delta)
{
  if (_transitionElapsed < _transitionDuration) {
    _transitionElapsed += delta;

    // clamp
    if (_transitionElapsed > _transitionDuration) {
      _transitionElapsed = _transitionDuration;
    }

    const int rainbowSize = 10000; // number of milliseconds rainbow unfolds over
    const double rainbowLength = (_rainbowStyle & RAINBOW_STYLE) ? 0 : 0.3; // How far across time the last pixel is from the first.
    double frame = (long)_transitionElapsed % rainbowSize;
    double gridSize = _fader.gridWidth() * _fader.gridHeight();
    
    for (int y = 0; y < _fader.gridHeight(); y++) {
      for (int x = 0; x < _fader.gridWidth(); x++) {

        double wheelPos = frame / rainbowSize;
        double tail = rainbowLength * ((double)(y * _fader.gridWidth() + x) / gridSize);
        wheelPos += tail;
        if (wheelPos > 1)
          wheelPos -= 1;

        uint32_t c = ColorWheel(wheelPos);

        _fader.setPixelColor(x, y, c, 0.1);
      }
    }
    return false;
  }
  else {
    // Animation complete.
    return true;
  }
}
  
  
// Color wheel yellow->red->blue->green
// Wheel from http://blog.asmartbear.com/color-wheels.html
// wheelPos: ranges 0..1
static uint32_t ColorWheel(double wheelPos)
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

