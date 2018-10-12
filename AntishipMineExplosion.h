#pragma once

#include "AnimatedObject.h"
#include "Explosions.h"

namespace Game
{

class AntishipMineExplosion : public Explosion
{
public:
  AntishipMineExplosion(GameScene* pScene);

  virtual void Loop(float dt);
};

}//namespace Game
