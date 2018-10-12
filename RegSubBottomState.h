#pragma once
#include "ObjectState2.h"


namespace Game
{


class RegSubBottomState : public ObjectState
{
public:
  RegSubBottomState(void);
  virtual ~RegSubBottomState(void);

  virtual bool Init();
  virtual void Loop(float dt);
};


}//namespace Game