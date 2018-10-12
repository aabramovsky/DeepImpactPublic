#pragma once
#include "ContextObject.h"

namespace Game
{

class Weapon : public ContextObject
{
public:
  Weapon(GameScene* pScene);
  virtual ~Weapon(void);

  virtual void Fire() = 0;
  virtual void Explode();
  virtual void Explode(const Settings& explS);
};

typedef RefCountedPtrBase<Weapon> WeaponPtr;

}//namespace Game
