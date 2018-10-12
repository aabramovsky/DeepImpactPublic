#pragma once
#include "ObjectState2.h"
#include "LuaScript.h"
#include "Controller.h"

namespace Game
{


class AtomSubMoveState : public ObjectState
{
public:
  AtomSubMoveState();
  virtual ~AtomSubMoveState();

  virtual bool Init();
  virtual void Loop(float dt);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );
};

}//namespace Game