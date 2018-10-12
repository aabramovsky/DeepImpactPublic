#include "StdAfx.h"
#include "luaGameScene.h"
#include "GameScene.h"

namespace Game
{


  luaGameScene::luaGameScene( GameScene* pScene ) : m_pScene(pScene)
  {
  }

  luaGameScene::~luaGameScene()
  {
  }

  float luaGameScene::GetWaterline()
  {
    return m_pScene->GetWaterline();
  }

  float luaGameScene::GetBottomline()
  {
    return m_pScene->GetWaterline();
  }

  void luaGameScene::AddSubmarine( luaObject* pluaSub )
  {
    m_pScene->AddSubmarine(pluaSub->m_objectPtr);
  }

  void luaGameScene::ObjectiveCompleted()
  {
    m_pScene->SetWaveActive(false);
  }

  void luaGameScene::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .beginClass <luaGameScene> ("GameScene")
          .addFunction ("ObjectiveCompleted", &luaGameScene::ObjectiveCompleted)
          .addFunction ("AddSubmarine", &luaGameScene::AddSubmarine)
          .addFunction ("GetWaterline", &luaGameScene::GetWaterline)
          .addFunction ("GetBottomline", &luaGameScene::GetBottomline)
        .endClass ()      
      .endNamespace ();
  }


}