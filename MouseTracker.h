#pragma once
#include <map>
#include "SDL_mouse.h"


enum MouseButton
{
  mbNone,
  mbLeft,
  mbRight,
  mbMiddle
};

namespace Game
{
  class Scene;
}


struct MouseState
{
  typedef std::map<MouseButton, bool> ButtonStates;

  ButtonStates m_mbStates;
  int m_lastX;
  int m_lastY;

  MouseState();

  bool IsPressed(MouseButton button) const;

  void UpdateMouseState(Game::Scene* pNotifyScene);

  void UpdateMouseButtonState(Game::Scene* pNotifyScene, MouseButton button, bool bPressed, int x, int y);
};