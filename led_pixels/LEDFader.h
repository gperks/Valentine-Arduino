#ifndef _LEDFADER_H_
#define _LEDFADER_H_

#ifndef WS2801_GRB
#include "SPI.h"
#include "Adafruit_WS2801.h"
#endif


// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b);

// Transitions between the current LED state and the desired color.
class LEDFader
{
  public:
    LEDFader(Adafruit_WS2801 &strip, int GridWidth, int GridHeight);
    ~LEDFader();
    void loop(unsigned long delta);
    
    /**
     @param duration Fade time in seconds.
     */
    void setPixelColor(int x, int y, uint32_t color, double duration);

    int gridWidth() const {return _gridWidth; }
    int gridHeight() const {return _gridHeight; }
    bool _logging;
    
  private:
    Adafruit_WS2801 &_strip;
    int _gridWidth;
    int _gridHeight;
    
    // Render colors
    // TargetColors: the colors the LEDs are to end up in at the transition's end
    // transitionDuration: the desired duration of the transition
    
    int pixelFromCoords(int x, int y);
    
    uint32_t *_targetColors; // Per pixel
    uint32_t *_startColors; // Per pixel, the colors at the animation's start.
    double *_transitionDuration; // Per pixel, in millis
    double *_transitionElapsed; // Per pixel, millis taken by animation so far


  
};

#endif

