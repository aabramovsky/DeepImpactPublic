#include "StdAfx.h"
#include "RegSubCrashState.h"
#include "Framework.h"

namespace Game
{


RegSubFallState::RegSubFallState(void)
{
}

RegSubFallState::~RegSubFallState(void)
{
}

bool RegSubFallState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texRegSubFall,  SGet("scale", 1.0f), m_anm);
  m_speed = SGet("sink_speed", 0.5f);

  return true;
}

void RegSubFallState::Loop( float dt )
{
  m_anm.update(dt);
  m_hotY += m_speed;

  if(m_hotY >= Scene()->GetBottomline() - 30)
  {
    Parent()->SwitchState(stateId(), 4);
  }
}


}//namespace Game
