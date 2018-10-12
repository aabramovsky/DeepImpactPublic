#include "StdAfx.h"
#include "ShipSailState.h"
#include "GameScene.h"
#include "Framework.h"
#include "Utility.h"
#include "lua\DebugDrawSettings.h"
#include "lua\luaObjectState.h"

namespace Game
{

ShipSailState::ShipSailState(void)
{
  m_acceleration = 0;
  m_maxSpeed = 0;
  m_vx = 0;
  m_bIdle = true;
  m_triggerDelay = 0;
  m_deceleration = 0;
}

ShipSailState::~ShipSailState(void)
{
}

bool ShipSailState::Init()
{
  SetOrientation( uoRight );

  m_dynamicTraits.xAccel = 10;
  m_dynamicTraits.xDecel = 10;
  m_dynamicTraits.xSpeedMax = 10; 

  m_bIdle = true;
  m_vx = 0;

  m_lua.Init();
  m_lua.DoScript("ship.lua");
  m_lua.Call("luaInitSailState", luaObjectState(this));
  
  m_triggerDelay = SGet("trigger_delay", 0.5f);
  
  return true;
}

bool ShipSailState::LoadResources()
{
  float scale = SGet("scale", 1.5f);
  resources().MakeAnimation(texShipHull, scale, m_shipHull);
  resources().MakeAnimation(texShipWavesIdle, scale, m_wavesIdle);
  resources().MakeAnimation(texShipWavesAccel, scale, m_wavesAccel);
  resources().MakeAnimation(texShipWavesFullSpeed, scale, m_wavesFullSpeed);

  return true;
}

void ShipSailState::Loop( float dt )
{
  m_shipHull.update(dt);
  m_wavesIdle.update(dt);
  m_wavesAccel.update(dt);
  m_wavesFullSpeed.update(dt);

  bool bPrevIdle = m_bIdle;

  bool bLeft = framework().KeyPressed(SDL_SCANCODE_LEFT);
  bool bRight = framework().KeyPressed(SDL_SCANCODE_RIGHT);

  float dSpeedAcX = m_dynamicTraits.xAccel * dt;
  float dSpeedDcX = m_dynamicTraits.xDecel * dt;

  if( bLeft )
  {
    m_bIdle = false;

    if(m_vx > 0)
      dSpeedAcX += dSpeedDcX; //plus water resistance

    abs_limited_increment(m_vx, -dSpeedAcX, m_dynamicTraits.xSpeedMax);
  }
  else if( bRight )
  {
    m_bIdle = false;

    if(m_vx < 0)
      dSpeedAcX += dSpeedDcX; //plus water resistance

    abs_limited_increment(m_vx, dSpeedAcX, m_dynamicTraits.xSpeedMax);
  }
  else if( !bLeft && !bRight )
  {
    abs_tozero(m_vx, dSpeedDcX);
  }
  
  m_hotX += m_vx;
  if(IsZero(m_vx, 1.0e-2))
    m_bIdle = true;

  if(m_vx < 0)
  {
    SetOrientation(uoLeft);
  }
  else if(m_vx > 0)
  {
    SetOrientation(uoRight);
  }

  if(bPrevIdle != m_bIdle)
    m_wavesAccel.set_frame(0);

  if( framework().KeyPressed(SDL_SCANCODE_DOWN) )
    FireBomb(dt);

  LoopAllSlots(dt);
}

void ShipSailState::Render()
{
  bool isLeft = (Orientation() == uoLeft);

  m_shipHull.set_flipped_h(isLeft);

  float idleWavesY = m_hotY + 22;
  float moveWavesY = m_hotY + 19;

  m_shipHull.render(m_hotX,m_hotY);

  if( m_bIdle )
  {
    m_wavesIdle.render( (isLeft ? m_hotX- 16 : m_hotX-22), idleWavesY);
  }
  else
  {
    if( m_vx < 2 )
    {
      m_wavesAccel.set_flipped_h(isLeft);
      m_wavesAccel.render( (isLeft ? m_hotX+27 : m_hotX-27), moveWavesY);
    }
    else
    {
      m_wavesFullSpeed.set_flipped_h(isLeft);
      m_wavesFullSpeed.render( (isLeft ? m_hotX+27 : m_hotX-27), moveWavesY);
    }    
  }  

  if(DbgSettings().dd_ship_life)
    DrawHealth();
}


void ShipSailState::FireBomb(float dt)
{
  static float triggerDelay = 500.5f;

  triggerDelay += dt;
  if(triggerDelay < m_triggerDelay)
    return;

  triggerDelay = 0.0f;

  if(GetWeaponSlot(1).Fire())
    return;

  if(GetWeaponSlot(2).Fire())
    return;
}

bool ShipSailState::GetSize( ge_rectF& rcSize )
{
  rcSize = m_shipHull.frame_box(m_hotX, m_hotY);
  return true;
}

bool ShipSailState::GetBoundingRect( ge_rectF& rc )
{
  rc = m_shipHull.bounding_box(m_hotX, m_hotY);
  return true;
}

bool ShipSailState::GetBoundRegion( gfx_region& region )
{
  m_shipHull.bounding_region(m_hotX, m_hotY, region);
  return true;
}

void ShipSailState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  if( !Parent()->IsIntersectingWith(pDamagingObject) )
    return;

  m_health -= fDamage;
  if(m_health <= 0.0f)
  {
    Parent()->SwitchState(stateId(), 2);
  }
}

void ShipSailState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .deriveClass <ShipSailState, ObjectState> ("ShipSailState")
        .addData ("m_acceleration", &ShipSailState::m_acceleration)
        .addData ("m_maxSpeed", &ShipSailState::m_maxSpeed)
        .addData ("m_triggerDelay", &ShipSailState::m_triggerDelay)
        .addData ("m_deceleration", &ShipSailState::m_deceleration)        
      .endClass ()      
    .endNamespace ();
}


}//namespace Game
