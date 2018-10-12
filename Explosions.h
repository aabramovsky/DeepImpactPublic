#pragma once

#include "AnimatedObject.h"

namespace Game
{

class Explosion : public AnimatedObject
{
public:
  Explosion(TextureId texExpl, GameScene* pScene);
  virtual ~Explosion();

  virtual void Init();
  virtual void Loop(float dt);
};


class NormalBombExplosion : public Explosion
{
public:
  NormalBombExplosion(GameScene* pScene) 
    : Explosion(texNormalBombExplosion, pScene)
  {}
};

class IonExplosion : public Explosion
{
public:
  IonExplosion(GameScene* pScene) 
    : Explosion(texIonExplosion, pScene)
  {}
};

class Shockwave : public Explosion
{
public:
  Shockwave(GameScene* pScene) 
    : Explosion(texShockwave, pScene)
  {}
};


}//namespace Game