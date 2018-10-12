#include "StdAfx.h"
#include "ContextObject.h"
#include "ObjectState2.h"

namespace Game
{


ContextObject::ContextObject(GameScene* pScene) : IScreenObject(pScene)
{
}

ContextObject::~ContextObject(void)
{
}

void ContextObject::AddState( int id, ObjectState* pState )
{
  pState->SetParent(this);
  pState->Init();
  pState->setStateId(id);

  m_states[id] = pState;
}

void ContextObject::Loop( float dt )
{
  m_pCurrentState->Loop(dt);
}

void ContextObject::Render()
{
  m_pCurrentState->Render();
}

void ContextObject::GetXY( float& hotX, float& hotY )
{
  m_pCurrentState->GetXY(hotX, hotY);
}

void ContextObject::MoveTo( float hotX, float hotY )
{
  m_pCurrentState->MoveTo(hotX, hotY);
}

void ContextObject::GetTrajectory( Trajectory& tr )
{
  currentState()->GetTrajectory(tr);
}

bool ContextObject::GetSize( ge_rectF& rcSize )
{
  return currentState()->GetSize(rcSize);
}

bool ContextObject::GetBoundingRect( ge_rectF& rc )
{
  return currentState()->GetBoundingRect( rc );
}


bool ContextObject::GetBoundRegion( gfx_region& region )
{
  return currentState()->GetBoundRegion( region );
}

void ContextObject::Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage )
{
  currentState()->Event_DamagedBy(pDamagingObject, fDamage);
}

void ContextObject::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
        .deriveClass <ContextObject, IObject> ("ContextObject")
        .addFunction ("GetXY", &ContextObject::GetXY)
        .addFunction ("MoveTo", &ContextObject::MoveTo)
      .endClass ()      
    .endNamespace ();
}


}//namespace Game