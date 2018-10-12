#include "StdAfx.h"
#include "RegSubMoveState.h"
#include "ContextObject.h"
#include <stdlib.h>
#include "Framework.h"
#include "LuaScript.h"
#include "lua\luaObjectState.h"
#include "Controller.h"

namespace Game
{

RegSubMoveState::RegSubMoveState(void)
{
   m_texId = texRegSubMove;
}

RegSubMoveState::~RegSubMoveState(void)
{
}

bool RegSubMoveState::Init()
{
  SetOrientation(uoRight);

  m_dynamicTraits.xSpeedMax = 60.0f; // pix/sec
  m_dynamicTraits.ySpeedMax = 20.0f;

  m_dynamicTraits.xAccel = 20.0f; // 2pix/sec
  m_dynamicTraits.yAccel = 20.0f; // 2pix/sec

  m_dynamicTraits.xDecel = 60.0f; // 2pix/sec
  m_dynamicTraits.yDecel = 30.0f; // 2pix/sec

  m_lua.Init();
  m_lua.DoScript("regular_submarine.lua");
  m_lua.Call("luaInitMoveState", luaObjectState(this));

  LoadResources();

  m_ptrController.set( new Controller(Parent()) );
  m_ptrController->Init(this);

  return true;
}

void RegSubMoveState::Loop( float dt )
{
  //ATLTRACE("x=%f, y=%f\n", m_hotX, m_hotY);

  m_anm.update(dt);

  LoopController(dt);

  LoopAllSlots(dt);
  FireAllSlots();
  
  //m_lua.Call("luaLoop", luaObjectState(this), dt);
}

void RegSubMoveState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  if( !Parent()->IsIntersectingWith(pDamagingObject) )
    return;

  m_health -= fDamage;
  if(m_health <= 0.0f)
  {
    Parent()->SwitchState(stateId(), 2);
  }
}


void RegSubMoveState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .deriveClass <RegSubMoveState, ObjectState> ("RegSubMoveState")
      .endClass ()      
    .endNamespace ();
}

}//namespace Game