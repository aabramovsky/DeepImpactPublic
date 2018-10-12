#include "StdAfx.h"
#include "AtomSubFallState.h"
#include "RegSubCrashState.h"
#include "Framework.h"

namespace Game
{


AtomSubFallState::AtomSubFallState(void)
{
}

AtomSubFallState::~AtomSubFallState(void)
{
}

bool AtomSubFallState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texAtomSubFall, SGet("scale", 1.0f), m_anm);

  m_speed = SGet("sink_speed", 0.3f);

  return true;
}

void AtomSubFallState::Loop( float dt )
{
  m_anm.update(dt);
  m_hotY += m_speed;

  if(m_hotY >= Scene()->GetBottomline() - 80)
  {
    Parent()->SwitchState(stateId(), 4);
  }
}


}//namespace Game