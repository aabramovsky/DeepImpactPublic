#include "StdAfx.h"
#include "GameScene.h"
#include "BombSplash.h"
#include "Explosions.h"
#include <algorithm>
#include "Bubbles.h"
#include "Framework.h"
#include "AntishipMineExplosion.h"
#include "AntishipMine.h"
#include <stdio.h>
#include "Rocket.h"
#include "Utility.h"
#include "Gfx\gfx_ttf_font.h"
#include <iosfwd>
#include "lua\DebugDrawSettings.h"
#include "lua\luaObject.h"
#include "lua\luaGameScene.h"
#include "GameStatus.h"

namespace Game
{


GameScene::GameScene(void)
{
  luaObject::gpGameScene = this;
}

GameScene::~GameScene(void)
{ 
}

void GameScene::LoadResources()
{
  m_screenWidth = framework().Window_Width();
  m_screenHeight = framework().Window_Height();

  LuaScript settingsScript;
  settingsScript.DoScript( "settings.lua" );

  m_waterline = settingsScript.GetValue<float>("waterline");
  m_bottomline = settingsScript.GetValue<float>("bottomline");

  resources().MakeAnimation(texReefBgAnim, m_anmBackground);
  resources().MakeAnimation(texReefWaterline, m_anmWaterline);
  
  m_numWave = 1;
}

void GameScene::SetUp()
{
  ShipUpgradeStatus::instance().numRegSubsDestroyed = 0;

  SetWaveActive(true);

  m_lua.Init();  
  m_lua.DoScript("submarine_creation.lua");

  CreateShip(); 

  m_lua.Call("luaCreateInitialWave", luaGameScene(this));

  LA_ProcessQueue(); // let the first added objects to appear on the screen
}

void GameScene::TearDown()
{
  mTeams.clear();
  mAllObjects.clear();
  LA_ClearActionsQueue();
  m_shipPtr.detach();
  m_lua.Cleanup();
}

void GameScene::Loop( float dt )
{
  m_anmBackground.update(dt);
  m_anmWaterline.set_frame( m_anmBackground.get_frame() + 1 );

  if( !IsWaveActive() )
  {
    SwitchToScene(ScnDock);
  }
  else
  {
    m_lua.Call("luaGameLoop", luaGameScene(this), dt);

    LoopGameObjects(dt);

    LA_ProcessQueue();
  }
}

void GameScene::Render()
{
  m_anmBackground.render(0.f,0.f, m_screenWidth, m_screenHeight);

  ObjectsCollection::iterator it = mAllObjects.begin();
  while(it != mAllObjects.end())
  {
    it->second->Render();
    ++it;
  }

  std::stringstream strStream;
  strStream << "total objects:" << mAllObjects.size() << "; effects:" << mTeams[-1].size() << "; team:" << mTeams[0].size();
  SDL_Color clr = {0,0,255,0};
  m_fntObjects.draw_solid(0, 0, strStream.str().c_str(), 26, clr);

  ////m_anmWaterline.get()->RenderStretch(0, GetWaterline(), m_screenWidth, GetWaterline() + m_screenHeight * 8 / 240.0f  );

  if(DbgSettings().dd_draw_waterline)
  {
    SDL_SetRenderDrawColor(sdl_renderer(), 255, 0, 0 ,0);
    SDL_RenderDrawLine(sdl_renderer(), 0, GetWaterline(), ScreenWidth(), GetWaterline());
  }

  if(DbgSettings().dd_draw_bottomline)
  {
    SDL_SetRenderDrawColor(sdl_renderer(), 255, 0, 0 ,0);
    SDL_RenderDrawLine(sdl_renderer(), 0, GetBottomline(), ScreenWidth(), GetBottomline());
  }
}

void GameScene::CreateBubblesAt( float hotX, float hotY )
{
  ObjectPtr pBubbles( new Game::Bubbles(this) );
  
  pBubbles->SSet("scale", 1.0f);

  pBubbles.get<Bubbles>()->MoveTo(hotX, hotY-10);

  LA_AddEffectObject(pBubbles);
}


void GameScene::LA_AddGameObject( ObjectPtr& objectPtr )
{  
  LoopAction la = { elaAddTeamObject, objectPtr };
  LA_PushLoopAction(la);
}

void GameScene::LA_AddEffectObject(ObjectPtr& objectPtr)
{
  objectPtr->Init();
  LoopAction la = { elaAddEffect, objectPtr };
  LA_PushLoopAction(la);
}

void GameScene::LA_RemoveGameObject( ObjectPtr& objectPtr )
{
  LoopAction la = { elaRemoveTeamObject, objectPtr };
  LA_PushLoopAction(la);
}


void GameScene::Event_BombPassedWaterline( float bombX )
{
  ObjectPtr pSplash( new BombSplash(this) );

  ge_rectF rc;
  m_shipPtr->GetBoundingRect(rc);

  pSplash.get<BombSplash>()->MoveTo(bombX, rc.bottom() - 10);
  
  LA_AddEffectObject(pSplash);
}

void GameScene::Event_BombExploded( Game::IScreenObject* pBomb, float scale, float dmgPerSec )
{
  Settings explSettings;
  explSettings.Set("scale", scale);
  explSettings.Set("damage", dmgPerSec);

  ObjectPtr pExpl( new NormalBombExplosion(this) );
  pExpl->SetSettings(explSettings);
  pExpl.get<NormalBombExplosion>()->AlignToObject(pBomb);
  pExpl->Init();

  LA_AddGameObject(pExpl);
  LA_RemoveGameObject( ObjectPtr(pBomb) );
}

void GameScene::Event_AntishipMineSurfaceExploded(Game::IScreenObject* pBomb, const Settings& explSettings)
{
}

void GameScene::Event_AntishipMineDeepExploded(Game::IScreenObject* pBomb, float scale, float dmgPerSec)
{
}

void GameScene::Event_RocketExploded(Game::IScreenObject* /*pRocket*/)
{
}

void GameScene::Event_AreaDamage( Game::IObject* pDamagingObject, float fDamage )
{
  ObjectsCollection& targetTeam = GetTeamObjects(0);
  
  ObjectsCollection::iterator it = targetTeam.begin();
  while(it != targetTeam.end())
  {
    ObjectPtr& pObj = it->second;
    if(pObj != pDamagingObject && pObj->CanTakeDamage())
    {
      pObj->Event_DamagedBy(pDamagingObject, fDamage);
    }
    ++it;
  }
}

void GameScene::CheckIntersectionsWithObject( Game::IObject* pCaller, std::vector<ObjectPtr>& vecInts )
{
  ObjectsCollection& targetTeam = GetTeamObjects(0);

  ObjectsCollection::iterator it = targetTeam.begin();
  while(it != targetTeam.end())
  {
    ObjectPtr& pObj = it->second;
    if(pObj != pCaller)
    {
      if(pObj->IsIntersectingWith(pCaller))
        vecInts.push_back(pObj);
    }
    ++it;
  }
}

void GameScene::CheckIntersectionsWithBreakableObjects( Game::IObject* pCaller, std::vector<ObjectPtr>& vecInts )
{
  ObjectsCollection& targetTeam = GetTeamObjects(0);

  ObjectsCollection::iterator it = targetTeam.begin();
  while(it != targetTeam.end())
  {
    ObjectPtr& pObj = it->second;
    if(pObj != pCaller)
    {
      if(pObj->IsIntersectingWith(pCaller))
        vecInts.push_back(pObj);
    }
    ++it;
  }
}

void GameScene::AddSubmarine( ObjectPtr& subPtr )
{
  LA_AddGameObject(subPtr);
}

void GameScene::Event_SubmarineDestroyed(Game::IObject* pDestroyed)
{
  if(dynamic_cast<RegularSubmarine*>(pDestroyed) != NULL)
  {
    ShipUpgradeStatus::instance().numRegSubsDestroyed++;
  }

  m_lua.Call("luaEvent_SubmarineDestroyed", luaGameScene(this), luaObject(pDestroyed));
}

float GameScene::GetBottomline() const
{
  return m_bottomline;
}

float GameScene::GetWaterline() const
{
  return m_waterline;
}

Game::ObjectsCollection& GameScene::GetTeamObjects( int teamId )
{
  return mTeams[teamId];
}

void GameScene::LA_PushLoopAction( LoopAction& la )
{
  mLoopActions.push_back(la);
}

void GameScene::LA_ClearActionsQueue()
{
  mLoopActions.clear();
}

void GameScene::LA_ProcessQueue()
{
  LoopActionArray::iterator it = mLoopActions.begin();
  while (it != mLoopActions.end())
  {
    LA_DoAction(*it);
    ++it;
  }

  LA_ClearActionsQueue();
}


void GameScene::LA_DoAction( LoopAction& la )
{
  ObjectPtr& actionObject = la.mObjPtr;
  ObjectId actionObjectId = actionObject->id();

  switch(la.mType)
  {
  case elaAddEffect:
    {
      mAllObjects[actionObjectId] = actionObject;
      
      ObjectsCollection& effects = GetTeamObjects(-1);
      effects[actionObjectId] = actionObject;
    }
    break;
  case elaAddTeamObject:
    {
      mAllObjects[actionObjectId] = actionObject;

      ObjectsCollection& team = GetTeamObjects(0);
      team[actionObjectId] = actionObject;
    }
    break;
  case elaRemoveTeamObject:
    {
      mAllObjects.erase(actionObjectId);
      
      ObjectsCollection& team = GetTeamObjects(0);
      team.erase(actionObjectId);

      ObjectsCollection& effects = GetTeamObjects(-1);
      effects.erase(actionObjectId);
    }
    break;
  }
}

void GameScene::LoopGameObjects( float dt )
{
  assert(mAllObjects.size() < 1000);

  ObjectsCollection::iterator it = mAllObjects.begin();
  while(it != mAllObjects.end())
  {
    it->second->Loop(dt);
    ++it;
  }
}

void GameScene::SetWaveActive( bool val )
{
  m_isWaveActive = val;

  if(val == false)
  {
    LA_ClearActionsQueue();
  }
}

void GameScene::CreateShip()
{
  m_shipPtr.detach();

  m_shipPtr = ObjectPtr(new Ship(this));
  m_shipPtr->Init();

  LA_AddGameObject( m_shipPtr );
}

}