#include "StdAfx.h"
#include "RocketOnboardState.h"

namespace Game
{


RocketOnboardState::RocketOnboardState(void)
{
}

RocketOnboardState::~RocketOnboardState(void)
{
}

bool RocketOnboardState::Init()
{
  SetOrientation(uoRight);
  resources().MakeAnimation(texRocketFly, SGet("scale", 1.0f), m_anm); 


  return true;
}

void RocketOnboardState::Loop( float dt )
{
}

void RocketOnboardState::Render()
{

}


}//namespace Game