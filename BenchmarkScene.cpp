#include "StdAfx.h"
#include "BenchmarkScene.h"
#include "Framework.h"
#include "Explosions.h"
#include "random.h"

namespace Game
{

BenchmarkScene::BenchmarkScene(void)
{
}

BenchmarkScene::~BenchmarkScene(void)
{
}


void BenchmarkScene::SetUp()
{
  m_screenWidth = framework().Window_Width();
  m_screenHeight = framework().Window_Height();

  LuaScript settingsScript;
  settingsScript.DoScript( "settings.lua" );

  m_waterline = settingsScript.GetValue<float>("waterline");
  m_bottomline = settingsScript.GetValue<float>("bottomline");

  resources().MakeAnimation(texReefBgAnim, m_anmBackground);
  resources().MakeAnimation(texReefWaterline, m_anmWaterline);

  for(int i = 0; i < 1000; i++)
  {
    gfx_animation* m_anm = new gfx_animation();
    
    resources().MakeAnimation(texNormalBombExplosion, random<float>(1, 3), *m_anm);

    m_objects.push_back(m_anm);
  }
}

void BenchmarkScene::TearDown()
{

}

void BenchmarkScene::Loop( float dt )
{
  m_anmBackground.update(dt);
  
  std::list<gfx_animation*>::iterator it = m_objects.begin();
  while(it != m_objects.end())
  {
    (*it)->update(dt);
    ++it;
  }
}

void BenchmarkScene::Render()
{
  m_anmBackground.render(0.0f, 0.0f, m_screenWidth, m_screenHeight);

  std::list<gfx_animation*>::iterator it = m_objects.begin();
  while(it != m_objects.end())
  {
    float x = random<float>(100.0f, m_screenWidth-100.0f);
    float y = random<float>(m_waterline, m_bottomline);

    gfx_animation* pAnm = *it;
    pAnm->render(x, y);
    
    ++it;
  }
}

void BenchmarkScene::LoadResources()
{

}

} //namespace Game

