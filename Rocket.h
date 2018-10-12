#pragma once

#include "Weapon.h"
#include "RocketOnboardState.h"
#include "RocketFlyState.h"

namespace Game
{


class Rocket : public Weapon
{
public:
  Rocket(GameScene* pScene);
  virtual ~Rocket(void);

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);
  virtual void Fire();

private:
  RocketOnboardState m_stOnBoard;
  RocketFlyState     m_stFly;
};


}//namespace Game
