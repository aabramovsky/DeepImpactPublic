#include "StdAfx.h"
#include "Framework.h"
#include "SDL.h"
#include "SDL_Image.h"
#include "Gfx\gfx_sprite.h"
#include "Gfx\gfx_animation.h"
#include "Utility.h"
#include "Gfx\gfx_ttf_font.h"
#include "lua\DebugDrawSettings.h"
#include "Scene.h"
#include <log4cplus/logger.h>

log4cplus::Logger logger;
Framework Framework::g_instance;

Framework::Framework(void)
{
  m_pCurrentScene = NULL;
  m_bSwitching = false;
  m_bHidingPrevious = true;
  m_switchAlpha = 0.0f;

  m_screenWidth = 640;
  m_screenHeight = 480;

  m_framesCount = 0;
  m_deltaTime = 0;
}

Framework::~Framework(void)
{
}

Framework& Framework::instance()
{
  return g_instance;
}

bool Framework::LoadSettings()
{
  LuaScript settingsScript;
  settingsScript.DoScript( "settings.lua" );

  m_screenWidth = settingsScript.GetValue<int>("window_width");
  m_screenHeight = settingsScript.GetValue<int>("window_height");  

  LOG4CPLUS_TRACE(logger, "Framework::LoadSettings success");
  return true;
}

void Framework::Run()
{
  LOG4CPLUS_TRACE(logger, "Entering Framework::Run");

  if( !LoadSettings() )
    return;

  DbgSettings().Load();

  LOG4CPLUS_TRACE(logger, "DbgSettings().Load() success");

  if (!GfxInitialize())
  {
    LOG4CPLUS_ERROR(logger, "GfxInitialize failed");
    return;
  }

  LOG4CPLUS_TRACE(logger, "GfxInitialize success");

  if (!GameInitialize())
  {
    LOG4CPLUS_ERROR(logger, "GameInitialize failed");
    return;
  }

  LOG4CPLUS_TRACE(logger, "GameInitialize success");

  MessagePump();

  LOG4CPLUS_TRACE(logger, "MessagePump exited");

  // TEST TEST: RELEASE SDL RESOURCES!!
  //GameEnd();
  Cleanup();

  LOG4CPLUS_TRACE(logger, "Exiting Framework::Run");
}

void Framework::MessagePump()
{
	SDL_Event event;
	bool quit = false;
	Uint32 t0 = 0;
	Uint32 dt = 0;

	while(!quit)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				quit=true;

			SDL_PumpEvents();
		}

		do { dt=SDL_GetTicks() - t0; } while(dt < 1);
    t0 = SDL_GetTicks();

		float fdt = (float)dt / 1000.0f;
		frameFunc(fdt);
		renderFunc();
	}
}

bool Framework::GfxInitialize()
{
  LOG4CPLUS_TRACE(logger, "Entering Framework::GfxInitialize");
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
		return false;

  LOG4CPLUS_TRACE(logger, "SDL_Init success");

	SDL_DisplayMode displayMode;
	int request = SDL_GetDesktopDisplayMode(0,&displayMode);

  LOG4CPLUS_TRACE(logger, "SDL_GetDesktopDisplayMode success");
  LOG4CPLUS_INFO(logger,
    std::endl << LOG4CPLUS_TEXT("format=") << std::hex << displayMode.format << std::dec << std::endl
    << LOG4CPLUS_TEXT("w=") << displayMode.w << std::endl
    << LOG4CPLUS_TEXT("h=") << displayMode.h << std::endl
    << LOG4CPLUS_TEXT("refresh_rate=") << displayMode.refresh_rate << std::endl);

	//Create window
	m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN );
	if( m_window == NULL )
		return false;

  LOG4CPLUS_TRACE(logger, "SDL_CreateWindow success");

	// ------------- TEST TEST ---------------------------------
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
  if (m_renderer == NULL)
    return false;

  LOG4CPLUS_TRACE(logger, "SDL_CreateRenderer success");
  return true;
}

bool Framework::GameInitialize()
{
  resources().Init(m_renderer);

  m_scnGame.LoadResources();
  m_scnDock.LoadResources();
  m_scnBenchmark.LoadResources();

  m_scenes[Game::ScnGame] = &m_scnGame;
  m_scenes[Game::ScnDock] = &m_scnDock;  

  m_pCurrentScene = &m_scnGame;
  //m_pCurrentScene = &m_scnDock;
  //m_pCurrentScene = &m_scnBenchmark;

  m_pCurrentScene->SetUp();  

  return true;
}

bool Framework::frameFunc(float dt)
{
  m_pCurrentScene->UpdateMouseState();

  if( m_bSwitching )
  {
    LoopSceneSwitching(dt);
  }
  else
  {
    m_pCurrentScene->Loop(dt);      
  }

  m_framesCount++;
  m_deltaTime += dt;
  if(m_deltaTime > 0.2f)
  {
    m_fps = m_framesCount / m_deltaTime;
    
    m_deltaTime = 0;
    m_framesCount = 0;
  }
  

  return false;
}

bool Framework::renderFunc()
{
  SDL_RenderClear(m_renderer);
  
  if( m_bSwitching )
  {
    RenderSceneSwitching();
  }
  else
  {
    m_pCurrentScene->Render();
  }

  DebugRender();

  std::stringstream strStream;
  strStream << "fps:" << m_fps;
  SDL_Color clr = {0,255,0,0};
  m_fntFps.draw_solid(m_screenWidth-150, 0, strStream.str().c_str(), 26, clr);

  SDL_RenderPresent(m_renderer); 

  return false;
}

void Framework::SwitchScene( Game::Scene* pCaller, int toScene )
{
  m_switchAlpha = 0.0f;

  m_bSwitching = true;
  m_bHidingPrevious = true;

  m_pNextScene = m_scenes[toScene];
}

void Framework::LoopSceneSwitching( float dt )
{
  static const float g_switchSpeed = 350.0f;
  if( m_bHidingPrevious )
  {
    limited_increment(m_switchAlpha, dt * g_switchSpeed, 255.0f);
    if(m_switchAlpha >= 244.9f)
    {
      m_bHidingPrevious = false;
      
      m_pCurrentScene->TearDown();
      m_pNextScene->SetUp();
    }
  }
  else
  {
    limited_decrement(m_switchAlpha, dt * g_switchSpeed, 0.0f);
    if(m_switchAlpha <= 0.1f) 
    {
      m_bSwitching = false;
      m_pCurrentScene = m_pNextScene;
    }
  }
}

void Framework::RenderSceneSwitching()
{
  if( m_bHidingPrevious )
  {
    m_pCurrentScene->Render();
  }
  else
  {
    m_pNextScene->Render();
  }

  SDL_SetRenderDrawBlendMode(sdl_renderer(), SDL_BLENDMODE_BLEND);
  
  SDL_Rect rcDst = {0, 0, m_screenWidth, m_screenHeight};
  SDL_SetRenderDrawColor(sdl_renderer(), 0, 0, 0, (Uint8)m_switchAlpha);
  SDL_RenderFillRect(sdl_renderer(), &rcDst);
}

void Framework::Cleanup()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Framework::DebugRender()
{
  SDL_SetRenderDrawColor(sdl_renderer(), 0, 255, 0 ,0);

  std::vector<ge::lineSeg2d>::iterator it = dbgLines.begin();
  while (it != dbgLines.end())
  {
    ge::lineSeg2d& ls2d = *it;
    SDL_RenderDrawLine(sdl_renderer(), ls2d.m_ptS.x(), ls2d.m_ptS.y(), ls2d.m_ptE.x(), ls2d.m_ptE.y());

    ++it;
  } 

  dbgLines.clear();
}

bool Framework::KeyPressed( int scanCode )
{
  const Uint8* pkeyboard = SDL_GetKeyboardState(NULL);
  return (pkeyboard[scanCode] != 0);
}

void Framework::DebugDraw( const ge::point2d& pt )
{

}

void Framework::DebugDraw( const ge::point2d& ptS, const ge::point2d& ptE )
{
  dbgLines.push_back(ge::lineSeg2d(ptS, ptE));
}

void Framework::DebugDraw( const ge::ray2d& ray )
{
  ge::point2d ptS = ray.pointOnLine();
  ge::point2d ptE = ptS + ray.direction() * 1000.0;

  dbgLines.push_back(ge::lineSeg2d(ptS, ptE));
}

