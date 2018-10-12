#pragma once

#include "ObjectState2.h"


namespace Game
{

class RocketFlyState : public ObjectState
{
public:
  RocketFlyState(void);
  virtual ~RocketFlyState(void);

  virtual bool Init();
  virtual void Loop(float dt);
  virtual void Render();

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );

  bool IsIntersectedShip();  

public:
  static void LuaRegister( LuaScript& lua );

private:
  float m_angle;
  b2Vec2 m_vSpeed;
};


}//namespace Game