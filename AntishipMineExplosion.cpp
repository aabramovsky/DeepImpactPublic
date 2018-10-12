#include "StdAfx.h"
#include "AntishipMineExplosion.h"
#include "GameScene.h"
#include "Framework.h"
#include "IObject.h"

namespace Game
{

AntishipMineExplosion::AntishipMineExplosion( GameScene* pScene ) : Explosion(texBombSplash, pScene)
{
}

void AntishipMineExplosion::Loop( float dt )
{
  m_anm.update(dt);

  if(m_anm.get_frame() == 1)
    Scene()->Event_AreaDamage( this, SGet("damage") * dt );

  if( !m_anm.is_playing() )
    Scene()->LA_RemoveGameObject( ObjectPtr(this) );
}

}//namespace Game
