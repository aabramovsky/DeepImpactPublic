#pragma once

#include "Scene.h"
#include "GameScene.h"
#include "BenchmarkScene.h"
#include "DockScene.h"

#include "lineSeg2d.h"
#include "gepoint.h"
#include "ray2d.h"

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus\tstring.h>
extern log4cplus::Logger logger;

class Framework
{
public:
  static Framework& instance();

  void Run();

  void SwitchScene(Game::Scene* pCaller, int toScene);

  SDL_Renderer* Renderer() { return m_renderer; }
  SDL_Window* Window() { return m_window; }

  //input
  bool KeyPressed(int scanCode);

  // application settings
public:
  int Window_Width() const { return m_screenWidth; }
  int Window_Height() const { return m_screenHeight; }

protected:
  Framework(void);
  ~Framework(void);

  bool LoadSettings();

  bool GfxInitialize();
  bool GameInitialize();

  bool frameFunc(float dt);
  bool renderFunc();

  void LoopSceneSwitching(float dt);
  void RenderSceneSwitching();

  void MessagePump();

  void Cleanup();
  void DebugRender();

protected:
  static Framework g_instance;

  SDL_Window* m_window;
  SDL_Renderer* m_renderer;

  Game::Scene* m_pCurrentScene;
  Game::Scene* m_pNextScene;

  bool m_bSwitching;
  bool m_bHidingPrevious;
  float m_switchAlpha;

  Game::Scene* m_scenes[Game::ScnCount];
  Game::GameScene       m_scnGame;
  Game::BenchmarkScene  m_scnBenchmark;
  Game::DockScene       m_scnDock;

  // application settings
  int m_screenWidth;
  int m_screenHeight;

  //
  gfx_ttf_font m_fntFps;
  float m_fps;

  float m_framesCount;
  float m_deltaTime; 

  //debug draw support
public:
  std::vector<ge::lineSeg2d> dbgLines;
  void DebugDraw(const ge::point2d& pt);
  void DebugDraw(const  ge::point2d& ptS, const  ge::point2d& ptE);
  void DebugDraw(const ge::ray2d& ray);
};

#define framework() Framework::instance()
#define gfx_engine() Framework::instance().GfxEngine()
#define sdl_renderer() Framework::instance().Renderer()
#define sdl_window() Framework::instance().Window()
//#define log
#define DEBUG_DRAW Framework::instance().DebugDraw
