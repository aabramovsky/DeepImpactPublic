#include "StdAfx.h"
#include "AtomSubBottomState.h"
#include "Framework.h"

namespace Game
{


AtomSubBottomState::AtomSubBottomState(void)
{
}

AtomSubBottomState::~AtomSubBottomState(void)
{
}

bool AtomSubBottomState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texAtomSubBottom, SGet("scale", 1.0f), m_anm);
  m_anm.set_mode(gfx_animation::playForward);

  return true;
}

void AtomSubBottomState::Loop( float dt )
{
  m_anm.update(dt);
}

}//namespace Game
