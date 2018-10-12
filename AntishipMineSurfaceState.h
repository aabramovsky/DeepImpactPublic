#pragma once
#include "ObjectState2.h"

namespace Game
{

class AntishipMineSurfaceState : public ObjectState
{
public:
  AntishipMineSurfaceState(void);
  virtual ~AntishipMineSurfaceState(void);

  virtual bool Init();
  virtual void Loop(float dt);
 
  bool IsIntersectedShip();

private:
  float m_dtExplodeDelay;
};

} //namespace Game