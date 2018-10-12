#include "StdAfx.h"
#include "Explosions.h"
#include "GameScene.h"
#include "Framework.h"

namespace Game
{


  Explosion::Explosion( TextureId texExpl, GameScene* pScene ) : AnimatedObject(pScene)
  {
    m_texId = texExpl;
  }

  Explosion::~Explosion()
  {

  }

  void Explosion::Init()
  {
    SetOrientation(uoRight);
    InitAnimation(m_texId);
    m_anm.set_fps( SGet("fps", m_anm.fps()) );
    m_anm.set_mode(gfx_animation::playForward);
  }

  void Explosion::Loop( float dt )
  {
    m_anm.update(dt);

    Scene()->Event_AreaDamage( this, SGet("damage") * dt );

    if( !m_anm.is_playing() )
    {
      if(m_hotY > Scene()->GetWaterline())
        Scene()->CreateBubblesAt(m_hotX, m_hotY);

      Scene()->LA_RemoveGameObject( ObjectPtr(this) );
    }
  }

}//namespace Game
