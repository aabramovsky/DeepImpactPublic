#include "StdAfx.h"
#include "AntishipMineSurfaceState.h"
#include "Framework.h"
#include "GameScene.h"
#include "Ship.h"

namespace Game
{


AntishipMineSurfaceState::AntishipMineSurfaceState(void)
{
  m_texId = texNormalBombOnShip;
}

AntishipMineSurfaceState::~AntishipMineSurfaceState(void)
{
}

bool AntishipMineSurfaceState::Init()
{
  m_dtExplodeDelay = SGet("surface_explosion_delay", 5.0f);  

  LoadResources();

  return true;
}

bool AntishipMineSurfaceState::IsIntersectedShip()
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

void AntishipMineSurfaceState::Loop( float dt )
{
  m_anm.update(dt);
  
  m_dtExplodeDelay -= dt;

  if( !Parent()->CanTakeDamage() ) // means that the bomb already exploded
    return;

  if(m_dtExplodeDelay <= 0.0f || IsIntersectedShip())
  {    
    Settings explSettings;
    GetSettings().CopyEntry<float>("surface_explosion_scale", "scale", explSettings);
    GetSettings().CopyEntry<float>("surface_explosion_damage", "damage", explSettings);
    GetSettings().CopyEntry<float>("surface_explosion_fps", "fps", explSettings);
    GetSettings().CopyEntry<std::string>("surface_explosion_type", "explosion_type", explSettings);
    GetSettings().CopyEntry<std::string>("surface_explosion_align", "explosion_align", explSettings);
    Parent<Weapon>()->Explode(explSettings);

    //Scene()->Event_AntishipMineSurfaceExploded( Parent(), explSettings );
  }
}

}
