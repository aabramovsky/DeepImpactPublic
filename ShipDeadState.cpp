#include "StdAfx.h"
#include "ShipDeadState.h"

#include "StdAfx.h"
#include "ShipSailState.h"
#include "GameScene.h"
#include "Framework.h"
#include "Utility.h"

namespace Game
{

ShipDeadState::ShipDeadState(void)
{
}

ShipDeadState::~ShipDeadState(void)
{
}

bool ShipDeadState::Init()
{
  SetOrientation( uoRight );

  resources().MakeAnimation(texNormalBombExplosion, m_anm);
  resources().MakeAnimation(texShipHull, SGet("scale", 1.0f), m_shipHull);

  return true;
}

void ShipDeadState::Loop( float dt )
{
  m_shipHull.update(dt);
  m_anm.update(dt);

  if( framework().KeyPressed(SDL_SCANCODE_SPACE) )
    Parent()->SwitchState(2, 1);
}

void ShipDeadState::Render()
{
  bool isLeft = (Orientation() == uoLeft);

  m_shipHull.set_flipped_h(isLeft);
  m_shipHull.render(m_hotX,m_hotY);
  m_anm.render(m_hotX,m_hotY);
}


}//namespace Game