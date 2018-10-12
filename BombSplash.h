#pragma once


#include "AnimatedObject.h"
#include "GameScene.h"

namespace Game
{


class BombSplash : public AnimatedObject
{
public:
  BombSplash(GameScene* pScene);
  virtual ~BombSplash(void);

  virtual void Init();
  virtual void Loop(float dt);
};


}//namespace Game