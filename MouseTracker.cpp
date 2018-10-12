#include "StdAfx.h"
#include "Scene.h"
#include "Framework.h"
#include "MouseTracker.h"

MouseState::MouseState()
{
  m_lastX = 0;
  m_lastY = 0;

  m_mbStates[mbLeft] = false;
  m_mbStates[mbRight] = false;
  m_mbStates[mbMiddle] = false;
}

bool MouseState::IsPressed( MouseButton button ) const
{
  ButtonStates::const_iterator it = m_mbStates.find(button);
  if(it == m_mbStates.end())
    return false;

  return it->second;
}

void MouseState::UpdateMouseState( Game::Scene* pNotifyScene )
{
  int x, y;
  Uint32 mstate = SDL_GetMouseState( &x, &y );

  if(x != m_lastX || y != m_lastY)
  {
    m_lastX = x;
    m_lastY = y;
    pNotifyScene->OnMouseMotion(x, y);
  }

  UpdateMouseButtonState(pNotifyScene, mbLeft, mstate & SDL_BUTTON_LMASK, x, y);
  UpdateMouseButtonState(pNotifyScene, mbRight, mstate & SDL_BUTTON_RMASK, x, y);
  UpdateMouseButtonState(pNotifyScene, mbMiddle, mstate & SDL_BUTTON_MMASK, x, y);
}

void MouseState::UpdateMouseButtonState( Game::Scene* pNotifyScene, MouseButton button, bool bPressed, int x, int y )
{
  if(bPressed)
  {
    if(m_mbStates[button] == false)
    {
      m_mbStates[button] = true;
      pNotifyScene->OnMouseButtonDown(button, x, y);
    }
  }
  else
  {
    if(m_mbStates[button] == true)
    {
      m_mbStates[button] = false;
      pNotifyScene->OnMouseButtonUp(button, x, y);
    }
  }
}
