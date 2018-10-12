#pragma once
#include "NormalBombOnShipState.h"
#include "NormalBombFallState.h"
#include "NormalBombSinkState.h"
#include "Weapon.h"

namespace Game
{

class NormalBomb : public Weapon
{
public:
  NormalBomb(GameScene* pScene);
  virtual ~NormalBomb(void);

  virtual void Init();
  virtual void SwitchState(int idFrom, int idTo);

  virtual void Fire();

private:  
  NormalBombOnShipState m_stOnShip;
  NormalBombFallState   m_stFall;
  NormalBombSinkState   m_stSink;
};


}//namespace Game