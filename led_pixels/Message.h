#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Actor.h"

class Message : public Actor
{
  public:
    Message(LEDFader &fader, const char *message);
    ~Message();
    virtual bool loop(unsigned long delta);
  private:
    const char *_message;
    int _messageLen;
    int _currentCharIndex;
    int _scrollIndex; // How many pixels have we scrolled within the current letter.
    bool _pausing; // pausing during scroll
    
    uint32_t _black;
    double _blur; // how fast the LEDs fade
    double _speedDelay; // in milliseconds, delay between frames
    
    void updateLEDs();
    void setColumnBlack(int x);
    void setColumnFromFrame(byte *frame, int x, int charX) ;
};

#endif

