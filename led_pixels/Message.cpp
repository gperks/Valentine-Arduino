#include "Message.h"

extern byte* alphabetFrameFromChar(char letter);
extern uint32_t ColorWheel(double wheelPos);

Message::Message(LEDFader &fader, const char *message)
  : Actor(fader, 0)
  , _message(message)
  , _currentCharIndex(0)
  , _scrollIndex(0)
  , _pausing(false)
  , _blur(0.05)
  , _speedDelay(200)
  , _wheelPos(0)
{
  _messageLen = strlen(message);
  _black = Color(10, 0, 0);
  //_fader._logging = true;
}

Message::~Message()
{
  //_fader._logging = false;
}


void Message::setColumnBlack(int x)
{
  for (int y = 0; y < _fader.gridHeight(); y++) {
    _fader.setPixelColor(x, y, _black, _blur);
  }
}

// @param x the coord of the LED pixel
// @param charX the column within the frame
void Message::setColumnFromFrame(byte *frame, int x, int charX)
{
  double wheelPos = _wheelPos + ((x-2)*.01);
  if (wheelPos < 0)
    wheelPos += 1;
  else if (wheelPos > 1)
    wheelPos -= 1;
  uint32_t color = ColorWheel(wheelPos);

  charX = 4 - charX;
  for (int y = 0; y < _fader.gridHeight(); y++) {
    byte bit = (frame[y] >> charX) & 1;
    _fader.setPixelColor(x, y, bit ? color : _black, _blur);
  }
}

void Message::updateLEDs()
{
  for (int x = 0; x < _fader.gridWidth(); x++) {
    if (x < _scrollIndex) {
      byte *frame = alphabetFrameFromChar(_message[_currentCharIndex]);

      setColumnFromFrame(frame, x, x - _scrollIndex + 5);
    }
    else if (x == _scrollIndex) {
      // Single pixel space between letters.
      setColumnBlack(x);
    }
    else { // x > _scrollIndex
      // Next letter available, not at end of message?
      if (_currentCharIndex + 1 < _messageLen) {
        byte *frame = alphabetFrameFromChar(_message[_currentCharIndex + 1]);
        setColumnFromFrame(frame, x, x - _scrollIndex  - 1);
      }
      else {
        // EOM
        setColumnBlack(x);
      }
    }
  }

  //  _currentCharIndex = t_currentCharIndex;
  //  _scrollIndex = t_scrollIndex;
}


bool Message::loop(unsigned long delta)
{
  if (_pausing) {
    if (_transitionElapsed < _transitionDuration) {
      _transitionElapsed += delta;
    }
    else {
      _pausing = false;
    }

    return false;
  }
  else if (_scrollIndex > 0) {
    // Scroll one pixel
    _scrollIndex--;
    
    _wheelPos += 0.02;
    if (_wheelPos > 1)
      _wheelPos = 0;
      
    updateLEDs();

    _pausing = true;
    _transitionElapsed = 0;
    _transitionDuration = _speedDelay; // milliseconds for pause, sets speed.
  }
  else if (_currentCharIndex + 1 < _messageLen) {
    // Onto next letter
    _scrollIndex = _fader.gridWidth() + 1; // +1 for space between chars
    _currentCharIndex++;
    

      
    return false;
  }
  else {
    // Animation complete.
    return true;
  }
}

