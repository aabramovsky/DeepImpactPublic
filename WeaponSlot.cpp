#include "StdAfx.h"
#include "WeaponSlot.h"
#include "AntishipMine.h"
#include "GameScene.h"
#include "Weapon.h"
#include "Rocket.h"

namespace Game
{



  WeaponSlot::WeaponSlot() : m_pScene(NULL), m_pOwner(NULL)
  {
    m_dx = 0;
    m_dy = 0;
    m_reloadDt = 0;
    m_weaponType = 0;
    m_reloadDelay = 0;
  }

  WeaponSlot::~WeaponSlot( void )
  {

  }

  void WeaponSlot::Init( int weaponType, float reloadDelay, float dx, float dy, GameScene* pScene, IScreenObject* pOwner )
  {
    m_weaponType = weaponType;

    m_pScene = pScene;
    m_pOwner = pOwner;
    m_reloadDt = 1000.0f;

    m_dx = dx;
    m_dy = dy;

    m_reloadDelay = reloadDelay;
  }

  void WeaponSlot::SetWeapon( WeaponType weaponType )
  {
    m_weaponType = weaponType;
  }

  void WeaponSlot::SetReloadDelay( float delay )
  {
    m_reloadDelay = delay;
  }

  void WeaponSlot::SetOffset( float dx, float dy )
  {
    m_dx = dx;
    m_dy = dy;
  }

  void WeaponSlot::AttachTo( ObjectState* pState )
  {
    m_pOwner = pState->Parent();
    m_pScene = m_pOwner->Scene();
  }

  void WeaponSlot::Loop( float dt )
  {
    TryReload(dt);

    AlignSlotToShipPos();
  }

  void WeaponSlot::TryReload( float dt )
  {
    if( !m_weaponPtr.isNull() )
      return;

    m_reloadDt += dt;
    if(m_reloadDt < m_reloadDelay)
      return;

    m_reloadDt = 0;
    CreateWeapon();
  }

  void WeaponSlot::CreateWeapon()
  {
    assert( m_weaponPtr.isNull() );
    if( !m_weaponPtr.isNull() )
      return;

    switch(m_weaponType)
    {
    case WPN_NORMAL_BOMB: m_weaponPtr = WeaponPtr( new NormalBomb(m_pScene) );
      break;
    case WPN_ANTISHIP_MINE: m_weaponPtr = WeaponPtr( new AntishipMine(m_pScene) );
      break;
    case WPN_ROCKET: m_weaponPtr = WeaponPtr( new Rocket(m_pScene) );
      break;
    }

    assert( !m_weaponPtr.isNull() );
    if( m_weaponPtr.isNull() )
      return;

    m_weaponPtr->SetSettings(m_settings);

    ObjectPtr weaponObj( m_weaponPtr.get<Weapon>() );
    weaponObj->Init();
    m_pScene->LA_AddGameObject(weaponObj);
  }

  void WeaponSlot::AlignSlotToShipPos()
  {
    if( m_weaponPtr.isNull() )
      return;

    float x,y;
    m_pOwner->GetXY(x, y);
    (m_pOwner->Orientation() == uoRight) ? x += m_dx : x -= m_dx;

    m_weaponPtr->MoveTo(x, y + m_dy);
  }

  bool WeaponSlot::Fire()
  {
    if( m_weaponPtr.isNull() )
      return false;

    m_weaponPtr->Fire();
    m_weaponPtr.detach();
    m_reloadDt = 0;

    return true;
  }

  void WeaponSlot::ForceFire()
  {
    CreateWeapon();
    Fire();
  }


  Settings& WeaponSlot::GetSettings()
  {
    return m_settings;
  }


  Game::luaSettings WeaponSlot::luaGetSettings()
  {
    return luaSettings(GetSettings());
  }

  void WeaponSlot::LuaRegister( LuaScript& lua )
  {
    getGlobalNamespace (lua.State())
      .beginNamespace ("luabridge")
        .beginClass <WeaponSlot> ("WeaponSlot2")
          .addFunction ("Loop", &WeaponSlot::Loop)
          .addFunction ("Fire", &WeaponSlot::Fire)
          .addFunction ("ForceFire", &WeaponSlot::ForceFire)
          .addFunction ("GetSettings", &WeaponSlot::luaGetSettings)
        .endClass ()      
      .endNamespace ();
  }

}//namespace Game
