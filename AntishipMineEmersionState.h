#pragma once
#include "ObjectState2.h"



namespace Game
{

class AntishipMineEmersionState : public ObjectState
{
public:
  AntishipMineEmersionState(void);
  virtual ~AntishipMineEmersionState(void);

  virtual bool Init();
  virtual void Loop(float dt);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );

public:
  static void LuaRegister( LuaScript& lua );

private:
  float m_speed;
};


}//namespace Game