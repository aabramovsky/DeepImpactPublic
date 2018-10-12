#include "StdAfx.h"
#include "luaObject.h"
#include "IScreenObject.h"

namespace Game
{

  GameScene* luaObject::gpGameScene = NULL;

  luaObject::luaObject()
  {
  }

  luaObject::luaObject( IObject* pObject ) : m_objectPtr(pObject)
  {
  }

  luaObject::~luaObject()
  {
  }

  const char* luaObject::type()
  {
    return m_objectPtr->ClassName();
  }

  int luaObject::id()
  {
    return m_objectPtr->id();
  }

  int luaObject::TeamId()
  {
    return m_objectPtr->TeamId();
  }

  int luaObject::BehaviorId()
  {
    return m_objectPtr->BehaviorId();
  }

  void luaObject::SetBehaviorId( int val )
  {
    m_objectPtr->SetBehaviorId(val);
  }

  float luaObject::GetX()
  {
    float x,y;
    m_objectPtr.get<IScreenObject>()->GetXY(x, y);

    return x;
  }

  float luaObject::GetY()
  {
    float x,y;
    m_objectPtr.get<IScreenObject>()->GetXY(x, y);

    return y;
  }

  void luaObject::MoveTo( float hotX, float hotY )
  {
    m_objectPtr.get<IScreenObject>()->MoveTo(hotX, hotY);
  }

  luaSettings luaObject::GetSettings()
  {
    return luaSettings( m_objectPtr->GetSettings() );
  }

  void luaObject::SetSettings( const luaSettings& s )
  {
    m_objectPtr->SetSettings( *s.activeSettings() );
  }

  void luaObject::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
          .beginClass <luaObject> ("Object")
          .addFunction ("type", &luaObject::type)
          .addFunction ("id", &luaObject::id)
          .addFunction ("TeamId", &luaObject::TeamId)
          .addFunction ("BehaviorId", &luaObject::BehaviorId)
          .addFunction ("SetBehaviorId", &luaObject::SetBehaviorId)
          .addFunction ("GetX", &luaObject::GetX)
          .addFunction ("GetY", &luaObject::GetY)
          .addFunction ("MoveTo", &luaObject::MoveTo)
          .addFunction ("GetSettings", &luaObject::GetSettings)
          .addFunction ("SetSettings", &luaObject::SetSettings)
        .endClass ()      
      .endNamespace ();
  }


}//namespace Game