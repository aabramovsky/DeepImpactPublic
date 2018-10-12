#include "StdAfx.h"
#include "BombSplash.h"
#include "GameScene.h"

namespace Game
{


BombSplash::BombSplash(GameScene* pScene) : AnimatedObject(pScene)
{
}

BombSplash::~BombSplash(void)
{
}

void BombSplash::Init()
{
  SetOrientation(uoRight);

  resources().MakeAnimation(texBombSplash, SGet("scale", 0.6f), m_anm);
  
  m_anm.set_mode(gfx_animation::playForward);
}

void BombSplash::Loop( float dt )
{
  m_anm.update(dt);

  if( !m_anm.is_playing() )
    Scene()->LA_RemoveGameObject( ObjectPtr(this) );
}

}//namespace Game

