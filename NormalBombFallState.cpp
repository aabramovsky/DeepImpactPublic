#include "StdAfx.h"
#include "NormalBombFallState.h"
#include "NormalBomb.h"
#include "GameScene.h"


namespace Game
{


NormalBombFallState::NormalBombFallState(void)
{
  m_texId = texNormalBombOnShip;
}

NormalBombFallState::~NormalBombFallState(void)
{
}

void NormalBombFallState::Loop( float dt )
{
  m_hotY += dt * 60.0f;

  if( m_hotY > Parent()->Scene()->GetWaterline() )
  {
    Parent()->Scene()->Event_BombPassedWaterline(m_hotX);
    Parent()->SwitchState(stateId(), 3);
  }
}


}//namespace Game
