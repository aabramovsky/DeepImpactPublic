#pragma once
#include "ObjectState2.h"


namespace Game
{


class AtomSubBottomState : public ObjectState
{
public:
  AtomSubBottomState(void);
  virtual ~AtomSubBottomState(void);

  virtual bool Init();
  virtual void Loop(float dt);
};


}//namespace Game

