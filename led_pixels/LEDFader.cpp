#include "LEDFader.h"

LEDFader::LEDFader(Adafruit_WS2801 &strip, int GridWidth, int GridHeight) 
: _strip(strip)
{
  _gridWidth = GridWidth;
  _gridHeight = GridHeight;
  int numPixels = GridWidth * GridHeight;
  _targetColors = (uint32_t*)calloc(numPixels, sizeof(uint32_t));
  _startColors = (uint32_t*)calloc(numPixels, sizeof(uint32_t));
  _transitionDuration = (double*)calloc(numPixels, sizeof(double));
  _transitionElapsed = (double*)calloc(numPixels, sizeof(double));
}

LEDFader::~LEDFader()
{
  free(_transitionElapsed);
  free(_transitionDuration);
  free(_startColors);
  free(_targetColors);
}

void LEDFader::setPixelColor(int x, int y, uint32_t color, double duration)
{
  int pixel = y * _gridWidth + x;
  _startColors[pixel] = _strip.getPixelColor(pixel);
  _targetColors[pixel] = color;
  _transitionDuration[pixel] = duration * 1000; // convert to millis
  _transitionElapsed[pixel] = 0;
}
  

void LEDFader::loop(unsigned long delta)
{
  for (int y = 0; y < _gridHeight; y++) {
    for (int x = 0; x < _gridWidth; x++) {
      int pixel = y * _gridWidth + x;
      
      if (_transitionElapsed[pixel] < _transitionDuration[pixel]) {
        _transitionElapsed[pixel] += delta;

        // clamp
        if (_transitionElapsed[pixel] > _transitionDuration[pixel])
          _transitionElapsed[pixel] = _transitionDuration[pixel];

        // How far through the animation are we?
        double factor =  _transitionElapsed[pixel] / _transitionDuration[pixel];
  
        uint32_t originalColor = _startColors[pixel];
      
        byte r = (originalColor >> 16) & 255;
        byte g = (originalColor >> 8 ) & 255;
        byte b = (originalColor      ) & 255;
  
        uint32_t targetColor = _targetColors[pixel];
        byte targetR = (targetColor >> 16) & 255;
        byte targetG = (targetColor >> 8 ) & 255;
        byte targetB = (targetColor      ) & 255;
  
        byte newR = r + ((targetR - r) * factor);
        byte newG = g + ((targetG - g) * factor);
        byte newB = b + ((targetB - b) * factor);
  
        _strip.setPixelColor(pixel, Color(newR, newG, newB));
      }
      else if ((_transitionDuration[pixel] == 0) && (_transitionElapsed[pixel] == 0)) {
        // No duration specified. Jump to target color immediately.
        _strip.setPixelColor(pixel, _targetColors[pixel]);
        _transitionElapsed[pixel] = 1; // so we don't try to set the color here again.
      }
    }
  }

  _strip.show();
}


// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  return c;
}


