#pragma once
#include "ObjectState2.h"


namespace Game
{


class AtomSubFallState : public ObjectState
{
public:
  AtomSubFallState(void);
  virtual ~AtomSubFallState(void);

  virtual bool Init();
  virtual void Loop(float dt);
virtual bool GetBoundingRect(ge_rectF& rc) { return false; }
private:
  float m_speed;
};


}//namespace Game
