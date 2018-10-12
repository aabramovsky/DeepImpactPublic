#include "StdAfx.h"
#include "RegularSubmarine.h"
#include "GameScene.h"
#include "Framework.h"

namespace Game
{


RegularSubmarine::RegularSubmarine(GameScene* pScene) : ContextObject(pScene)
{
}

RegularSubmarine::~RegularSubmarine(void)
{
}

void RegularSubmarine::Init()
{
  AddState(1, &m_stMove);
  AddState(2, &m_stCrash);
  AddState(3, &m_stFall);
  AddState(4, &m_stBottom);  
  
  setCurrentState( &m_stMove );
}

void RegularSubmarine::SwitchState( int idFrom, int idTo )
{
  // align states animations
  if(idFrom == 1)
  {
    float x,y;
    m_stMove.GetXY(x, y);
    y -= 22.0f;
    m_stCrash.MoveTo(x,y);

    setCurrentState(&m_stCrash);

    Scene()->Event_SubmarineDestroyed(this);
  }
  else if(idFrom == 2)
  {
    float x,y;
    m_stCrash.GetXY(x, y);
    m_stFall.MoveTo(x,y);

    setCurrentState(&m_stFall);
  }
  else if(idFrom == 3)
  {
    float x,y;
    m_stFall.GetXY(x, y);
    m_stBottom.MoveTo(x,y);

    setCurrentState(&m_stBottom);
  }
}

}//namespace Game