#include "StdAfx.h"


#include "RocketFlyState.h"
#include "Framework.h"
#include "GameScene.h"
#include "Weapon.h"


namespace Game
{


RocketFlyState::RocketFlyState(void)
{
}

RocketFlyState::~RocketFlyState(void)
{
}

bool RocketFlyState::Init()
{
  m_lua.Init();
  m_lua.DoScript("rocket.lua"); 

  float pi2 = 3.14f / 2.0f;
  //m_angle = gfx_engine()->Random_Float(-pi2, pi2);

  m_vSpeed.Set(100, 0);

  //m_lua.Call("luaInitRocketFlyState", this);

  /*m_vSpeed.x = ::sin(m_angle);
  m_vSpeed.y = -::cos(m_angle);

  m_vSpeed *= 100.f;*/

  SetOrientation(uoRight);
  resources().MakeAnimation(texRocketFly, SGet("scale", 1.0f), m_anm);  

  return true;
}

void RocketFlyState::Loop( float dt )
{
//  m_anm.update(dt);
  m_anm.set_frame(0);

  //m_lua.Call("luaLoopRocketFlyState", this, dt);
  ObjectPtr& ptrShip = Scene()->GetShipPtr();
  Ship* pShip = ptrShip.get<Ship>();

  float x, y;
  pShip->GetXY(x, y);

  b2Vec2 vToShip(x - m_hotX, y - m_hotY);
  
  vToShip.Normalize();
  float speedMod = m_vSpeed.Normalize();
  
  b2Vec2 vShift = vToShip - m_vSpeed;
   
  vShift /= 30.0f;

  m_vSpeed += vShift;
  m_vSpeed.Normalize();
  m_vSpeed *= speedMod;

  //ATLTRACE("%.2f %.2f\n", m_vSpeed.x, m_vSpeed.y);

  m_hotX += m_vSpeed.x * dt;
  m_hotY += m_vSpeed.y * dt;  

  m_angle = m_vSpeed.angle() + bPI2;

  if( IsIntersectedShip() && Parent()->CanTakeDamage() )
  {
    Scene()->Event_RocketExploded( Parent() );
    Parent<Weapon>()->Explode();
  }

  RemoveIfOutOfScene(100.0f);
}

void RocketFlyState::Render()
{
  m_anm.render(m_hotX, m_hotY, m_angle);
  //gfx_engine()->Gfx_RenderLine(m_hotX, m_hotY, m_hotX + m_vSpeed.x, m_hotY + m_vSpeed.y);
}

void RocketFlyState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  if( !Parent()->IsIntersectingWith(pDamagingObject) )
    return;

  Scene()->Event_RocketExploded( Parent() );
  Parent<Weapon>()->Explode();
}

bool RocketFlyState::IsIntersectedShip()
{
  std::vector<ObjectPtr> vecObjects; // TEST TEST постоянное выделение крупного объекта. можно в статик сделать или членом класса
  Scene()->CheckIntersectionsWithBreakableObjects( Parent(), vecObjects );

  std::vector<ObjectPtr>::iterator it = vecObjects.begin();
  while(it != vecObjects.end() )
  {
    ObjectPtr& pObj = *it;
    if(pObj.get<Ship>() != NULL)
      return true;

    ++it;
  }

  return false;
}

void RocketFlyState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
    .deriveClass <RocketFlyState, ObjectState> ("RocketFlyState")
      .addData ("m_angle", &RocketFlyState::m_angle)
    .endClass ()      
    .endNamespace ();
}


}