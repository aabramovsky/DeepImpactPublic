#include "StdAfx.h"
#include "Rocket.h"
#include "GameScene.h"
#include "Framework.h"

namespace Game
{


Rocket::Rocket(GameScene* pScene)
  : Weapon(pScene)
{
}

Rocket::~Rocket(void)
{
}

void Rocket::Init()
{
  AddState(1, &m_stOnBoard);
  AddState(2, &m_stFly);

  setCurrentState( &m_stOnBoard );
}

void Rocket::Fire()
{
  if(currentState() != &m_stOnBoard)
    return;

  SwitchState(1, 2);
}

void Rocket::SwitchState( int idFrom, int idTo )
{
  if(idTo == 2)
  {
    m_stFly.AlignTo(&m_stOnBoard);
    setCurrentState(&m_stFly);
  }
}


}//namespace Game