#pragma once
#include "ObjectState2.h"


namespace Game
{


class RegSubFallState : public ObjectState
{
public:
  RegSubFallState(void);
  virtual ~RegSubFallState(void);

  virtual bool Init();
  virtual void Loop(float dt);
virtual bool GetBoundingRect(ge_rectF& rc) { return false; }
private:
  float m_speed;
};


}//namespace Game