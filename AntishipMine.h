#pragma once

#include "AntishipMineOnboardState.h"
#include "AntishipMineEmersionState.h"
#include "AntishipMineSurfaceState.h"
#include "Weapon.h"

namespace Game
{


class AntishipMine : public Weapon
{
public:
  AntishipMine(GameScene* pScene);
  virtual ~AntishipMine(void);

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);
  virtual void Fire();

private:
  AntishipMineOnboardState m_stOnBoard;
  AntishipMineEmersionState m_stEmersion;
  AntishipMineSurfaceState  m_stSurface;
};


}//namespace Game
