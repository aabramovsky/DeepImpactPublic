#include "StdAfx.h"
#include "Scene.h"
#include "Framework.h"

namespace Game
{
 
MouseState Scene::g_mouseState;

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::SwitchToScene( int scnId )
{
  framework().SwitchScene(this, scnId);
}

void Scene::UpdateMouseState()
{
  g_mouseState.UpdateMouseState(this);
}

void Scene::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .beginClass <Scene> ("Scene")
        .addFunction ("SwitchToScene", &Scene::SwitchToScene)
      .endClass ()      
    .endNamespace ();
}

}