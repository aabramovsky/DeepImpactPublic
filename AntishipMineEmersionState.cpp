#include "StdAfx.h"
#include "AntishipMineEmersionState.h"
#include "Framework.h"
#include "GameScene.h"

namespace Game
{


AntishipMineEmersionState::AntishipMineEmersionState(void)
{
  m_texId = texNormalBombSink;
}

AntishipMineEmersionState::~AntishipMineEmersionState(void)
{
}

bool AntishipMineEmersionState::Init()
{
  m_lua.Init();
  m_lua.DoScript("antiship_mine.lua"); 
 
  m_speed = SGet("emersion_speed", 1.0f);

  LoadResources();

  return true;
}

void AntishipMineEmersionState::Loop( float dt )
{
  m_anm.update(dt);

  m_lua.Call("luaLoopEmersionState", this, dt);
  
  if(m_hotY <= Scene()->GetWaterline())
    Parent()->SwitchState(2, 3);
}

void AntishipMineEmersionState::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  if( !Parent()->IsIntersectingWith(pDamagingObject) )
    return;

  //Scene()->Event_AntishipMineDeepExploded();

  Settings explSettings;
  GetSettings().CopyEntry<float>("deep_explosion_scale", "scale", explSettings);
  GetSettings().CopyEntry<float>("deep_explosion_damage", "damage", explSettings);
  GetSettings().CopyEntry<float>("deep_explosion_fps", "fps", explSettings);
  GetSettings().CopyEntry<std::string>("deep_explosion_type", "explosion_type", explSettings);
  GetSettings().CopyEntry<std::string>("deep_explosion_align", "explosion_align", explSettings);
  Parent<Weapon>()->Explode(explSettings);
}


void AntishipMineEmersionState::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .deriveClass <AntishipMineEmersionState, ObjectState> ("AntishipMineEmersionState")
        .addData ("m_speed", &AntishipMineEmersionState::m_speed)
      .endClass ()      
    .endNamespace ();
}


}


