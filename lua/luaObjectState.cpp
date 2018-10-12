#include "StdAfx.h"
#include "luaObjectState.h"
#include "ObjectState2.h"

namespace Game
{


luaObjectState::luaObjectState( ObjectState* pState ) : m_pState(pState)
{  
}

luaObjectState::~luaObjectState()
{
}

void luaObjectState::LoadResources()
{
  m_pState->LoadResources();
}

void luaObjectState::AddWeaponSlot( int slotId, luaWeaponSlot& slot )
{
  m_pState->AddWeaponSlot2(slotId, slot.m_slotPtr);
}

Game::luaGameScene luaObjectState::Scene()
{
  return luaGameScene(m_pState->Scene());
}

void luaObjectState::SetDynamicTraits( luaDynamicTraits& traits )
{
  m_pState->SetDynamicTraits(traits.m_traits);
}

void luaObjectState::SetHealthStatus( float health, float maxHealth )
{
  m_pState->SetHealthStatus(health, maxHealth);
}

Game::luaSettings luaObjectState::GetObjectSettings()
{
  return luaSettings( m_pState->Parent()->GetSettings() );
}

void luaObjectState::SetObjectSettings( const luaSettings& s )
{
  m_pState->Parent()->SetSettings( *s.activeSettings() );
}

void luaObjectState::MoveTo( float hotX, float hotY )
{
  m_pState->MoveTo(hotX, hotY);
}

luaRect2d luaObjectState::GetSize()
{
  ge_rectF rc;
  m_pState->GetSize(rc);
  
  return luaRect2d(rc);
}

void luaObjectState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("game")
      .beginClass <luaObjectState> ("ObjectState")
        .addFunction ("AddWeaponSlot", &luaObjectState::AddWeaponSlot)    
        .addFunction ("Scene", &luaObjectState::Scene)    
        .addFunction ("SetDynamicTraits", &luaObjectState::SetDynamicTraits)    
        .addFunction ("SetHealthStatus", &luaObjectState::SetHealthStatus)    
        .addFunction ("GetObjectSettings", &luaObjectState::GetObjectSettings)    
        .addFunction ("SetObjectSettings", &luaObjectState::SetObjectSettings)    
        .addFunction ("MoveTo", &luaObjectState::MoveTo)            
        .addFunction ("GetSize", &luaObjectState::GetSize)
        .addFunction ("LoadResources", &luaObjectState::LoadResources)            
      .endClass ()      
    .endNamespace ();
}


}//namespace Game