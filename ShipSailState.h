#pragma once
#include "ObjectState2.h"
#include "Gfx\gfx_animation.h"

namespace Game
{

class ShipSailState : public ObjectState
{
public:
  ShipSailState(void);
  virtual ~ShipSailState(void);

  virtual bool Init();
  virtual bool LoadResources();
  virtual void Loop(float dt);
  virtual void Render();

  void FireBomb(float dt);

  virtual bool GetSize(ge_rectF& rcSize);
  virtual bool GetBoundingRect(ge_rectF& rc);
  virtual bool GetBoundRegion(gfx_region& region);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage );  

public:
  static void LuaRegister(LuaScript& lua);
  
private:
  gfx_animation m_shipHull;
  gfx_animation m_wavesIdle;
  gfx_animation m_wavesAccel;
  gfx_animation m_wavesFullSpeed;

  float m_acceleration;
  float m_deceleration;
  float m_maxSpeed;
  float m_vx;
  
  bool m_bIdle;
  
  float m_triggerDelay;
};


}//namespace Game