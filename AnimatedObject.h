#pragma once
#include "IObject.h"
#include "IScreenObject.h"
#include "Gfx\gfx_animation.h"
#include "ResourceManager.h"

namespace Game
{


class AnimatedObject : public IScreenObject
{
public:
  AnimatedObject(GameScene* pScene);
  virtual ~AnimatedObject(void);

  virtual void Init() = 0;
  virtual void Loop(float dt) = 0;
  virtual void Render();

  virtual void GetXY(float& hotX, float& hotY) { hotX = m_hotX; hotY = m_hotY; }
  virtual void MoveTo(float hotX, float hotY) { m_hotX = hotX; m_hotY = hotY; }

  virtual bool GetSize(ge_rectF& rcSize);
  virtual bool GetBoundingRect(ge_rectF& rc);
  virtual bool GetBoundRegion(gfx_region& region);

protected:
  void InitAnimation(TextureId texId);

protected:
  float m_hotX;
  float m_hotY;

  TextureId m_texId;
  gfx_animation m_anm;
};


}//Game
