#include "StdAfx.h"
#include "AntishipMineOnboardState.h"
#include "IScreenObject.h"

namespace Game
{


AntishipMineOnboardState::AntishipMineOnboardState(void)
{
}

AntishipMineOnboardState::~AntishipMineOnboardState(void)
{
}

bool AntishipMineOnboardState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texNormalBombOnShip, SGet("scale", 1.0f), m_anm);
  

  return true;
}

void AntishipMineOnboardState::Loop( float dt )
{
}

void AntishipMineOnboardState::Render()
{

}


}//namespace Game