#include "StdAfx.h"
#include "ObjectState2.h"
#include "ContextObject.h"
#include "GameScene.h"
#include "Weapon.h"
#include "lua\DebugDrawSettings.h"
#include "vector2d.h"

namespace Game
{

//std::auto_ptr<hgeFont> ObjectState::m_fontPtr;

ObjectState::ObjectState( void ) 
  : m_hotX(0), m_hotY(0), m_health(0), m_maxHealth(0), m_texId(__texCount)
{
}

bool ObjectState::Init()
{
  LoadResources();
  return true;
}

void ObjectState::LoopController( float dt )
{
  if(m_ptrController.isNull())
    return;

  m_ptrController->Loop(dt);

  float vx = m_vSpeed.x;
  float vy = m_vSpeed.y;

  float dSpeedAcX = m_dynamicTraits.xAccel * dt;
  float dSpeedDcX = m_dynamicTraits.xDecel * dt;

  if( m_ptrController->Left() )
  {
    abs_limited_increment(vx, -dSpeedAcX, m_dynamicTraits.xSpeedMax);
  }
  else if( m_ptrController->Right() )
  {
    abs_limited_increment(vx, dSpeedAcX, m_dynamicTraits.xSpeedMax);
  }
  else if( !m_ptrController->Left() && !m_ptrController->Right() )
  {
    abs_tozero(vx, dSpeedDcX);
  }

  float dSpeedAcY = m_dynamicTraits.yAccel * dt;
  float dSpeedDcY = m_dynamicTraits.yDecel * dt;

  if( m_ptrController->Up() )
  {
    abs_limited_increment(vy, -dSpeedAcY, m_dynamicTraits.ySpeedMax);
  }
  else if( m_ptrController->Down() )
  {
    abs_limited_increment(vy, dSpeedAcY, m_dynamicTraits.ySpeedMax);
  }
  else if( !m_ptrController->Up() && !m_ptrController->Down() )
  {
    abs_tozero(vy, dSpeedDcY);
  }

  m_vSpeed = ge::vector2d(vx, vy);

  if(m_vSpeed.x < 0)
  {
    SetOrientation(uoLeft);
  }
  else if(m_vSpeed.x > 0)
  {
    SetOrientation(uoRight);
  }

  m_hotX += m_vSpeed.x*dt;
  m_hotY += m_vSpeed.y*dt;
}

bool ObjectState::LoadResources()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(m_texId, SGet("scale", 1.0f), m_anm);

  return true;
}

void ObjectState::Render()
{
  bool isLeft = (Orientation() == uoLeft);

  m_anm.set_flipped_h( isLeft );

  m_anm.render(m_hotX, m_hotY);

  if(DbgSettings().dd_submarines_life)
    DrawHealth();
}

void ObjectState::DrawHealth()
{
  if(Parent<Weapon>() != NULL)
    return;

  std::stringstream strStream;
  strStream << m_health << "/" << m_maxHealth;

  SDL_Color clr = {0,0,255,0};
  m_fntHealth.draw_solid(m_hotX, m_hotY, gfx_ttf_font::hCenter, gfx_ttf_font::vCenter, strStream.str().c_str(), 18, clr);
}

void ObjectState::GetXY( float& hotX, float& hotY )
{
  hotX = m_hotX; hotY = m_hotY;
}

void ObjectState::MoveTo( float hotX, float hotY )
{
  m_hotX = hotX; m_hotY = hotY;
}

bool ObjectState::GetSize( ge_rectF& rcSize )
{
  rcSize = m_anm.frame_box(m_hotX, m_hotY);
  return true;
}

bool ObjectState::GetBoundingRect( ge_rectF& rc )
{
  rc = m_anm.bounding_box(m_hotX, m_hotY);
  return true;
}

bool ObjectState::GetBoundRegion( gfx_region& region )
{
  m_anm.bounding_region(m_hotX, m_hotY, region);
  return true;
}

void ObjectState::AlignTo( ObjectState* pOtherState, float dx, float dy )
{
  float x = 0.f, y = 0.f;
  pOtherState->GetXY(x, y);
  MoveTo(x + dx, y + dy);
}

void ObjectState::GetTrajectory( Trajectory& tr )
{
  tr.bValid = true;
  tr.ptPos = ge::point2d(m_hotX, m_hotY);
  tr.vSpeed = m_vSpeed;
}

WeaponSlot& ObjectState::AddWeaponSlot( int weaponType, int slotId, float reloadDelay, float dx, float dy )
{
  WeaponSlotsMap::iterator it = m_slots.find(slotId);
  if(it != m_slots.end())
    return *(it->second);

  WeaponSlotPtr slotPtr( new WeaponSlot() );  
  slotPtr->Init(weaponType, reloadDelay, dx, dy, Scene(), Parent());

  m_slots[slotId] = slotPtr;
  return *slotPtr;
}

WeaponSlot& ObjectState::AddWeaponSlot2( int slotId, WeaponSlotPtr& slotPtr )
{
  slotPtr->AttachTo(this);
  m_slots[slotId] = slotPtr;
  return *slotPtr;
}

WeaponSlot& ObjectState::GetWeaponSlot( int slotId )
{
  return *(m_slots[slotId]);
}

void ObjectState::RemoveWeaponSlot( int slotId )
{
  m_slots.erase(slotId);
}

void ObjectState::LoopAllSlots( float dt )
{
  WeaponSlotsMap::iterator it = m_slots.begin();
  while(it != m_slots.end())
  {
    WeaponSlot& ws = *(it->second);
    ws.Loop(dt);

    ++it;
  }
}

void ObjectState::FireAllSlots()
{
  WeaponSlotsMap::iterator it = m_slots.begin();
  while(it != m_slots.end())
  {
    WeaponSlot& ws = *(it->second);
    ws.Fire();

    ++it;
  }
}


void ObjectState::SetHealthStatus( float health, float maxHealth )
{
  assert(health <= maxHealth);

  m_maxHealth = maxHealth;
  m_health = health;
}

bool ObjectState::IsOutOfScene( float dOutOffset )
{
  if(m_hotX < -dOutOffset
    || m_hotX > Scene()->ScreenWidth() + dOutOffset
    || m_hotY < -dOutOffset
    || m_hotY > Scene()->ScreenHeight() + dOutOffset)
  {
    return true;
  }

  return false;
}

void ObjectState::RemoveIfOutOfScene( float dOutOffset )
{
  if( IsOutOfScene(dOutOffset) )
    Scene()->LA_RemoveGameObject( ObjectPtr(Parent()) );
}

void ObjectState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
    .deriveClass <ObjectState, IObjectState> ("ObjectState")
    .addFunction ("AddWeaponSlot", &ObjectState::AddWeaponSlot)
    .addFunction ("AddWeaponSlot2", &ObjectState::AddWeaponSlot2)
    .addFunction ("GetWeaponSlot", &ObjectState::GetWeaponSlot)
    .addFunction ("RemoveWeaponSlot", &ObjectState::RemoveWeaponSlot)
    .addFunction ("LoopAllSlots", &ObjectState::LoopAllSlots)
    .addFunction ("FireAllSlots", &ObjectState::FireAllSlots)
    .addFunction ("SetMaximumHealth", &ObjectState::SetMaximumHealth)    
    .addData ("m_hotX", &ObjectState::m_hotX)
    .addData ("m_hotY", &ObjectState::m_hotY)
    .addData ("m_health", &ObjectState::m_health)
    .addData ("m_maxHealth", &ObjectState::m_maxHealth)
    .endClass ()      
    .endNamespace ();
}

}//namespace Game