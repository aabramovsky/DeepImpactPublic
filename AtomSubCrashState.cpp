#include "StdAfx.h"
#include "AtomSubCrashState.h"

#include "Framework.h"

namespace Game
{


AtomSubCrashState::AtomSubCrashState(void)
{
}

AtomSubCrashState::~AtomSubCrashState(void)
{
}

bool AtomSubCrashState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texAtomSubCrash, SGet("scale", 1.0f), m_anm);
  m_anm.set_mode(gfx_animation::playForward);

  m_hotY = 300.f;
  m_hotX = 300.f;

  m_speed = SGet("sink_speed", 0.3f);

  return true;
}

void AtomSubCrashState::Loop( float dt )
{
  m_anm.update(dt);
  m_hotY += m_speed;

  if(!m_anm.is_playing())
  {
    Parent()->SwitchState(stateId(), 3);
  }
}


}//namespace Game