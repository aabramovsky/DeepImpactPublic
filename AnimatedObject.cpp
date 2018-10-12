#include "StdAfx.h"
#include "AnimatedObject.h"

namespace Game
{


AnimatedObject::AnimatedObject(GameScene* pScene)
 : IScreenObject(pScene)
{
  m_hotX = 0;
  m_hotY = 0;
}

AnimatedObject::~AnimatedObject(void)
{
}

void AnimatedObject::Render()
{
  bool isLeft = (Orientation() == uoLeft);

  m_anm.set_flipped_h(isLeft);

  m_anm.render(m_hotX, m_hotY);
}

bool AnimatedObject::GetSize( ge_rectF& rcSize )
{
  rcSize = m_anm.frame_box(m_hotX, m_hotY);
  return true;
}

bool AnimatedObject::GetBoundingRect( ge_rectF& rc )
{
  rc = m_anm.bounding_box(m_hotX, m_hotY);
  return true;
}

bool AnimatedObject::GetBoundRegion( gfx_region& region )
{
  m_anm.bounding_region(m_hotX, m_hotY, region);
  return true;
}


void AnimatedObject::InitAnimation( TextureId texId )
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texId, SGet("scale", 1.3f), m_anm);
}

}//Game