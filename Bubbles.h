#pragma once

#include "AnimatedObject.h"

namespace Game
{


class Bubbles : public AnimatedObject
{
public:
  Bubbles(GameScene* pScene);
  virtual ~Bubbles(void);

  virtual void Init();
  virtual void Loop(float dt);
};


}//namespace Game