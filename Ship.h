#pragma once

#include "WeaponSlot.h"
#include "ContextObject.h"
#include "ShipSailState.h"
#include "ShipDeadState.h"

namespace Game
{

class Ship : public ContextObject
{
public:
  Ship(GameScene* pScene);
  virtual ~Ship();

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);

  float GetHealth() const;

private:
  ShipSailState m_stSail;
  ShipDeadState m_stDead;
};


}//Game