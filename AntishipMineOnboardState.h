#pragma once
#include "ObjectState2.h"

namespace Game
{

class AntishipMineOnboardState : public ObjectState
{
public:
  AntishipMineOnboardState(void);
  virtual ~AntishipMineOnboardState(void);

  virtual bool Init();
  virtual void Loop( float dt );
  virtual void Render();
};


}//namespace Game