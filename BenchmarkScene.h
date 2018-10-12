#pragma once
#include "Scene.h"

#include "Ship.h"
#include "RegularSubmarine.h"
#include <set>
#include "NormalBomb.h"
#include <vector>
#include "IObject.h"
#include "IScreenObject.h"
#include "AtomSubmarine.h"
#include "LuaScript.h"
#include <list>
#include "Gfx\gfx_animation.h"

namespace Game
{

class BenchmarkScene : public Scene
{
public:
  BenchmarkScene(void);
  virtual ~BenchmarkScene(void);

  virtual void LoadResources();
  virtual void SetUp();
  virtual void TearDown();
  virtual void Loop(float dt);
  virtual void Render(); 

private:
  gfx_animation m_anmBackground;
  gfx_animation m_anmWaterline;

  float m_waterline;
  float m_bottomline;

  std::list<gfx_animation*> m_objects;
};

} //namespace Game