#pragma once
#include "ObjectState2.h"
#include "Gfx\gfx_animation.h"


namespace Game
{

class ShipDeadState : public ObjectState
{
public:
  ShipDeadState(void);
  virtual ~ShipDeadState(void);

  virtual bool Init();
  virtual void Loop(float dt);
  virtual void Render(); 

private:
  gfx_animation m_shipHull;
};


}//namespace Game