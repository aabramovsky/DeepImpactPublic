#pragma once

#include "ObjectState2.h"

namespace Game
{


class AtomSubCrashState : public ObjectState
{
public:
  AtomSubCrashState(void);
  virtual ~AtomSubCrashState(void);

  virtual bool Init();
  virtual void Loop(float dt);

  virtual bool GetBoundingRect(ge_rectF& rc) { return false; }

private:
  float m_speed;
};


}//namespace Game