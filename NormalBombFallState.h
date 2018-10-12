#pragma once
#include "ObjectState2.h"



namespace Game
{

class NormalBombFallState : public ObjectState
{
public:
  NormalBombFallState(void);
  virtual ~NormalBombFallState(void);

  virtual void Loop(float dt);
};


}//namespace Game
