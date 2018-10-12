#pragma once
#include "ObjectState2.h"
#include "LuaScript.h"

namespace Game
{

class RegSubMoveState : public ObjectState
{
public:
  RegSubMoveState();
  virtual ~RegSubMoveState();

  virtual bool Init();
  virtual void Loop(float dt);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );

  static void LuaRegister(LuaScript& lua);

private:
  LuaScript m_lua;
};

}//namespace Game
