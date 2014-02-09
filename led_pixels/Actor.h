#ifndef _ACTOR_H_
#define _ACTOR_H_

class Actor
{
  public:
  virtual void loop(unsigned long delta) = 0;
  virtual ~Actor() {}
};

#endif

