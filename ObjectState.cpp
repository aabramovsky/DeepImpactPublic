#include "StdAfx.h"
#include "ObjectState.h"
#include "ContextObject.h"
#include "GameScene.h"

namespace Game
{

void IObjectState::SetOrientation( int val )
{
  Parent()->SetOrientation(val > 0 ? uoRight : uoLeft);
}

int IObjectState::Orientation() const
{
  return (int)Parent()->Orientation();
}

void IObjectState::FlipOrientation()
{
  SetOrientation( Orientation() == uoLeft ? uoRight : uoLeft );
}

GameScene* IObjectState::Scene()
{
  return Parent()->Scene();
}

int IObjectState::WaveNumber()
{
  return Scene()->WaveNumber();
}

int IObjectState::BehaviorId()
{
  return Parent()->BehaviorId();
}

const Settings& IObjectState::GetSettings() const
{
  return Parent()->GetSettings();
}

float IObjectState::SGet( const char* name, float defVal )
{
  return Parent()->SGet(name, defVal);
}

void IObjectState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  
}

void IObjectState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .beginClass <IObjectState> ("IObjectState")
      .addFunction ("Scene", &IObjectState::Scene)
      .addFunction ("WaveNumber", &IObjectState::WaveNumber)
      .addFunction ("BehaviorId", &IObjectState::BehaviorId)
      .addFunction ("Orientation", &IObjectState::Orientation)
      .addFunction ("SetOrientation", &IObjectState::SetOrientation)
      .addFunction ("SGet", &IObjectState::SGet)
    .endClass ()      
  .endNamespace ();
}

}//namespace Game