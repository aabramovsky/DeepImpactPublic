#include "StdAfx.h"
#include "Ship.h"
#include "Framework.h"
#include "Utility.h"
#include <atlbase.h>
#include <atltrace.h>

namespace Game
{


Ship::Ship(GameScene* pScene) 
  : ContextObject(pScene)
{
}

Ship::~Ship()
{
}

void Ship::Init()
{
  AddState(1, &m_stSail);
  AddState(2, &m_stDead);

  setCurrentState( &m_stSail );
}

void Ship::SwitchState( int idFrom, int idTo )
{
  // align states animations
  if(idFrom == 1)
  {
    m_stDead.AlignTo(&m_stSail);
    setCurrentState(&m_stDead);

    //Scene()->Event_SubmarineDestroyed(this);
  }
  else if(idFrom == 2)
  {
    m_stSail.SetMaximumHealth();
    m_stSail.AlignTo(&m_stDead);
    setCurrentState(&m_stSail);
  }
}

float Ship::GetHealth() const
{
  if(currentState() != &m_stSail)
    return 0.0f;

  return m_stSail.Health();

}

}//Game