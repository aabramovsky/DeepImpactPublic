#include "StdAfx.h"
#include "AtomSubMoveState.h"
#include "ContextObject.h"
#include <stdlib.h>
#include "Framework.h"
#include "lua\luaObjectState.h"

namespace Game
{

AtomSubMoveState::AtomSubMoveState(void)
{
  m_texId = texAtomSubMove;
}

AtomSubMoveState::~AtomSubMoveState(void)
{
}

bool AtomSubMoveState::Init()
{
  m_dynamicTraits.xSpeedMax = 60.0f; // pix/sec
  m_dynamicTraits.ySpeedMax = 20.0f;
  
  m_dynamicTraits.xAccel = 20.0f; // 2pix/sec
  m_dynamicTraits.yAccel = 20.0f; // 2pix/sec

  m_dynamicTraits.xDecel = 60.0f; // 2pix/sec
  m_dynamicTraits.yDecel = 30.0f; // 2pix/sec

  m_ptrController.set( new Controller(Parent()) );
  m_ptrController->Init(this);

  m_lua.Init();
  m_lua.DoScript("atom_submarine.lua");
  m_lua.Call("luaInitMoveState", luaObjectState(this));  

  LoadResources();

  return true;
}

void AtomSubMoveState::Loop( float dt )
{
  m_anm.update(dt);

  //m_lua.Call("luaLoop", this, dt);
  
  LoopController(dt);

  LoopAllSlots(dt);
  FireAllSlots();
}

void AtomSubMoveState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  if( !Parent()->IsIntersectingWith(pDamagingObject) )
    return;

  m_health -= fDamage;
  if(m_health <= 0.0f)
  {
    Parent()->SwitchState(stateId(), 2);
  }
}


}//namespace Game