#pragma once
#include "ObjectState2.h"

namespace Game
{

class RocketOnboardState : public ObjectState
{
public:
  RocketOnboardState(void);
  virtual ~RocketOnboardState(void);

  virtual bool Init();
  virtual void Loop( float dt );
  virtual void Render();
};


}//namespace Game

