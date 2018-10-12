#include "StdAfx.h"
#include "NormalBombSinkState.h"
#include "GameScene.h"
#include "RegularSubmarine.h"
#include "AtomSubmarine.h"
#include "Framework.h"
#include "AntishipMine.h"

namespace Game
{


NormalBombSinkState::NormalBombSinkState(void)
{
  m_texId = texNormalBombSink;
}

NormalBombSinkState::~NormalBombSinkState(void)
{
}

bool NormalBombSinkState::Init()
{
  LoadResources();
  m_speed = SGet("speed", 50.0f);

  return true;
}

void NormalBombSinkState::Loop( float dt )
{
  m_hotY += dt * m_speed;
  m_anm.update(dt);

  if( !Parent()->CanTakeDamage() )
    return;

  if( CheckIfBombHitEnemy() ||
      m_hotY >= Scene()->GetBottomline() )
  {
    Parent<Weapon>()->Explode();
  }
}

void NormalBombSinkState::GetTrajectory( Trajectory& tr )
{
  tr.bValid = true;
  tr.ptPos = ge::point2d(m_hotX, m_hotY);
  tr.vSpeed = ge::vector2d(0.0, m_speed);
}

bool NormalBombSinkState::CheckIfBombHitEnemy()
{
  std::vector<ObjectPtr> vecObjects; // TEST TEST постоянное выделение крупного объекта. можно в статик сделать или членом класса
  Scene()->CheckIntersectionsWithBreakableObjects( Parent(), vecObjects );

  std::vector<ObjectPtr>::iterator it = vecObjects.begin();
  while(it != vecObjects.end() )
  {
    ObjectPtr& pObj = *it;
    if(pObj.get<RegularSubmarine>() != NULL)
      return true;

    if(pObj.get<AtomSubmarine>() != NULL)
      return true;

    if(pObj.get<AntishipMine>() != NULL)
      return true;


    ++it;
  }
  
  return false;
}


}//namespace Game
