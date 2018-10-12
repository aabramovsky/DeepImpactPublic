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
#include <map>
#include "Gfx\gfx_ttf_font.h"
#include "lua\luaObject.h"

namespace Game
{

typedef std::map<ObjectId, ObjectPtr> ObjectsCollection;
//typedef std::multimap<float, ObjectPtr> ZOrderedCollection;
typedef std::map<int, ObjectsCollection> TeamCollection;

enum LoopActionType
{
  elaAddEffect,
  elaAddTeamObject,
  elaRemoveTeamObject
};

struct LoopAction
{
  LoopActionType mType;
  ObjectPtr mObjPtr;
};

typedef std::vector<LoopAction> LoopActionArray;

class GameScene : public Scene
{
public:
  GameScene(void);
  virtual ~GameScene(void);

  virtual void LoadResources();
  virtual void SetUp();
  virtual void TearDown();

  virtual void Loop(float dt);

  void CreateShip();

  void LoopGameObjects( float dt );

  virtual void Render(); 

  template<typename TGameObject>
  ObjectPtr CreateGameObject()
  {
    ObjectPtr ptrObject( new TGameObject(this) );
    LA_AddGameObject(ptrObject);

    return ptrObject;
  }

  void CreateBubblesAt(float hotX,float hotY);  

  float GetWaterline() const;
  float GetBottomline() const;

  void CheckIntersectionsWithObject(Game::IObject* pCaller, std::vector<ObjectPtr>& vecInts);
  void CheckIntersectionsWithBreakableObjects(Game::IObject* pCaller, std::vector<ObjectPtr>& vecInts);

  void Event_BombPassedWaterline(float bombX);
  void Event_BombExploded(Game::IScreenObject* pBomb, float scale, float dmgPerSec);
  void Event_AntishipMineSurfaceExploded(Game::IScreenObject* pBomb, const Settings& explSettings);
  void Event_AntishipMineDeepExploded(Game::IScreenObject* pBomb, float scale, float dmgPerSec);
  void Event_RocketExploded(Game::IScreenObject* pRocket);
  void Event_AreaDamage(Game::IObject* pDamagingObject, float fDamage);
  void Event_SubmarineDestroyed(Game::IObject* pDestroyed);

  void AddSubmarine(ObjectPtr& subPtr);

  float ScreenWidth() const { return m_screenWidth; }
  float ScreenHeight() const { return m_screenHeight; }

  int WaveNumber() { return m_numWave; }
  bool IsWaveActive() const { return m_isWaveActive; }
  void SetWaveActive(bool val);

  ObjectPtr& GetShipPtr() { return m_shipPtr; }
  //GameObjects& GetBreakableObjects() { return m_breakableObjects; }

  ObjectsCollection& GetTeamObjects(int teamId);
  
  template<typename T>
  void GetObjectsOfType(int teamId, ObjectsCollection& rResult);
  
  // loop actions
  void LA_AddGameObject(ObjectPtr& objectPtr);
  void LA_AddEffectObject(ObjectPtr& objectPtr);
  void LA_RemoveGameObject(ObjectPtr& objectPtr);

  void LA_PushLoopAction(LoopAction& la);
  void LA_ClearActionsQueue();
  void LA_ProcessQueue();
  void LA_DoAction(LoopAction& la);

private:
  gfx_animation m_anmBackground;
  gfx_animation m_anmWaterline;
  
  TeamCollection mTeams;
  ObjectsCollection mAllObjects;

  LoopActionArray mLoopActions;

  float m_waterline;
  float m_bottomline;

  gfx_ttf_font m_fntObjects;

  int m_numWave;
  bool m_isWaveActive;

  ObjectPtr m_shipPtr;

  LuaScript m_lua;

  
};

template<typename T>
void GameScene::GetObjectsOfType(int teamId, ObjectsCollection& rResult)
{
  ObjectsCollection& teamObjects = GetTeamObjects(teamId);

  ObjectsCollection::iterator it = mAllObjects.begin();
  while(it != mAllObjects.end())
  {
    ObjectPtr& ptrObject = it->second;
    if( ptrObject.get<T>() != NULL )
    {
      rResult.insert(std::make_pair(it->first, it->second));
    }
    
    ++it;
  }
}

} //namespace Game