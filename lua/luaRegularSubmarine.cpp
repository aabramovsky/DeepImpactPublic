#include "StdAfx.h"
#include "luaRegularSubmarine.h"
#include "RegularSubmarine.h"

namespace Game
{
  luaRegularSubmarine::luaRegularSubmarine( const luaSettings& ls )
  {
    assert( m_objectPtr.isNull() );

    m_objectPtr = ObjectPtr(new Game::RegularSubmarine(gpGameScene));

    SetSettings(ls);

    m_objectPtr->Init();
  }

  luaRegularSubmarine::~luaRegularSubmarine()
  {

  }

  void luaRegularSubmarine::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .deriveClass <luaRegularSubmarine, luaObject> ("RegularSubmarine")
          .addConstructor <void (*) (const luaSettings&)> ()
        .endClass ()      
      .endNamespace ();
  }

}//namespace Game