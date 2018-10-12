#include "StdAfx.h"
#include "IScreenObject.h"

namespace Game
{


IScreenObject::IScreenObject(GameScene* pScene)
  : IObject(pScene)
{
}

IScreenObject::~IScreenObject(void)
{
}

void IScreenObject::AlignToObject( IScreenObject* objAlignTo, float dx, float dy )
{
  float x = 0.f, y = 0.f;
  objAlignTo->GetXY(x, y);
  MoveTo(x + dx, y + dy);
}


void IScreenObject::GetTrajectory( Trajectory& tr )
{
  tr.bValid = false;
}

}//Game