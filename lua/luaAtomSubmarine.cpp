#include "StdAfx.h"
#include "luaAtomSubmarine.h"
#include "AtomSubmarine.h"

namespace Game
{
  luaAtomSubmarine::luaAtomSubmarine(const luaSettings& ls)
  {
    assert( m_objectPtr.isNull() );
    
    m_objectPtr = ObjectPtr(new Game::AtomSubmarine(gpGameScene));
    
    SetSettings(ls);

    m_objectPtr->Init();
  }

  luaAtomSubmarine::~luaAtomSubmarine()
  {

  }

  void luaAtomSubmarine::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("game")
        .deriveClass <luaAtomSubmarine, luaObject> ("AtomSubmarine")
          .addConstructor <void (*) (const luaSettings&)> ()
        .endClass ()      
      .endNamespace ();
  }

}//namespace Game