#include "StdAfx.h"
#include "NormalBomb.h"

namespace Game
{


NormalBomb::NormalBomb(GameScene* pScene)
  : Weapon(pScene)
{
}

NormalBomb::~NormalBomb(void)
{
}

void NormalBomb::Init()
{
  AddState(1, &m_stOnShip);
  AddState(2, &m_stFall);
  AddState(3, &m_stSink);

  
  setCurrentState( &m_stOnShip );
}

void NormalBomb::SwitchState( int idFrom, int idTo )
{
  if(idFrom == 1)
  {
    float x,y;
    m_stOnShip.GetXY(x, y);
    m_stFall.MoveTo(x,y);

    setCurrentState(&m_stFall);
  }
  else if(idFrom == 2)
  {
    float x,y;
    m_stFall.GetXY(x, y);
    m_stSink.MoveTo(x,y);

    setCurrentState(&m_stSink);
  }

}

void NormalBomb::Fire()
{
  SwitchState(1, 2);
}


}//namespace Game