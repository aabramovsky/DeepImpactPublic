#include "StdAfx.h"
#include "Bubbles.h"
#include "GameScene.h"

namespace Game
{


Bubbles::Bubbles(GameScene* pScene) : AnimatedObject(pScene)
{
}

Bubbles::~Bubbles(void)
{
}

void Bubbles::Init()
{
  SetOrientation(uoRight);

  resources().MakeAnimation(texBubbles, SGet("scale"), m_anm);
  
  m_anm.set_mode(gfx_animation::playForward);
}

void Bubbles::Loop( float dt )
{
  m_anm.update(dt);

  m_hotY -= dt*50.0f;
  
  const float fYMoveLimit = Scene()->GetWaterline() + 20.0f;
  if(m_hotY <= fYMoveLimit)
  {
    m_hotY = fYMoveLimit;
  }

  if( !m_anm.is_playing() )
    Scene()->LA_RemoveGameObject( ObjectPtr(this) );
}

}//namespace Game