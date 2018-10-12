#include "StdAfx.h"
#include "RegSubCrashState.h"
#include "Framework.h"

namespace Game
{


RegSubCrashState::RegSubCrashState(void)
{
}

RegSubCrashState::~RegSubCrashState(void)
{
}

bool RegSubCrashState::Init()
{
  SetOrientation(uoRight);

  resources().MakeAnimation(texRegSubCrash, SGet("scale", 1.0f), m_anm);
  m_anm.set_mode(gfx_animation::playForward);
  
  m_sinkSpeed = SGet("sink_speed", 0.5f);

  return true;
}

void RegSubCrashState::Loop( float dt )
{
  m_anm.update(dt);
  m_hotY += m_sinkSpeed;

  if(m_hotY > Scene()->GetBottomline() - 30)
    m_hotY = Scene()->GetBottomline() - 30;

  if(!m_anm.is_playing())
  {
    Parent()->SwitchState(stateId(), 3);
  }
}


}//namespace Game