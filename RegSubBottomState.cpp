#include "StdAfx.h"
#include "RegSubCrashState.h"
#include "Framework.h"

namespace Game
{


RegSubBottomState::RegSubBottomState(void)
{
}

RegSubBottomState::~RegSubBottomState(void)
{
}

bool RegSubBottomState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texRegSubBottom, SGet("scale", 1.0f), m_anm);
  m_anm.set_mode(gfx_animation::playForward);

  return true;
}

void RegSubBottomState::Loop( float dt )
{
  m_anm.update(dt);
}

}//namespace Game