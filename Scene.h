#pragma once
#include "LuaScript.h"
#include "MouseTracker.h"

namespace Game
{

enum SceneId
{
  ScnGame = 0,  
  ScnDock = 1,
  ScnCount
};

class Scene
{
public:
  Scene(void);
  virtual ~Scene(void);

  virtual void LoadResources() = 0;

  virtual void SetUp() = 0;
  virtual void TearDown() = 0;

  virtual void Loop(float dt) = 0;
  virtual void Render() = 0;

  void SwitchToScene(int scnId);

  virtual void UpdateMouseState();
  virtual void OnMouseMotion(int x, int y) {};
  virtual void OnMouseButtonDown(MouseButton button, int x, int y) {};
  virtual void OnMouseButtonUp(MouseButton button, int x, int y) {};

public:
  static void LuaRegister(LuaScript& lua);

public:
  float m_screenWidth;
  float m_screenHeight;
 
  // mouse state tracker
  static MouseState g_mouseState;
};

}