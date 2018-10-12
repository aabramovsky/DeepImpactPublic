#include "StdAfx.h"
#include "AntishipMine.h"
#include "GameScene.h"
#include "Framework.h"

namespace Game
{


AntishipMine::AntishipMine(GameScene* pScene)
  : Weapon(pScene)
{
}

AntishipMine::~AntishipMine(void)
{
}

void AntishipMine::Init()
{
  AddState(1, &m_stOnBoard);
  AddState(2, &m_stEmersion);
  AddState(3, &m_stSurface);

  setCurrentState( &m_stOnBoard );
}

void AntishipMine::Fire()
{
  if(currentState() != &m_stOnBoard)
    return;

  SwitchState(1, 2);
}

void AntishipMine::SwitchState( int idFrom, int idTo )
{
  if(idTo == 2)
  {
    m_stEmersion.AlignTo(&m_stOnBoard);
    setCurrentState(&m_stEmersion);
  }

  if(idTo == 3)
  {
    m_stSurface.AlignTo(&m_stEmersion);
    setCurrentState(&m_stSurface);
  }
}

}//namespace Game
