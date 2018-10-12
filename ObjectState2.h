#pragma once

#include "LuaScript.h"
#include <map>
#include "WeaponSlot.h"
#include "ObjectState.h"
#include "ResourceManager.h"
#include "Gfx\gfx_animation.h"
#include "Gfx\gfx_ttf_font.h"
#include "Controller.h"
#include "vector2d.h"

namespace Game
{

  class IObject;
  class GameScene;
  class ContextObject;

  //////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////
  struct DynamicTraits
  {
    DynamicTraits()
    {
      xSpeedMax = 0;
      ySpeedMax = 0;

      xAccel = 0;
      yAccel = 0;

      xDecel = 0;
      yDecel = 0;
    }

    float xSpeedMax;
    float ySpeedMax;

    float xAccel;
    float yAccel;

    float xDecel;
    float yDecel;

    ge::vector2d a() const
    {
      return ge::vector2d(xAccel, yAccel);
    }

    ge::vector2d vMax() const
    {
      return ge::vector2d(xSpeedMax, ySpeedMax);
    }
  };


  //////////////////////////////////////////////////////////////////////////
  // animation based object state
  //////////////////////////////////////////////////////////////////////////

  class ObjectState : public IObjectState
  {
  public:
    ObjectState(void);
    virtual ~ObjectState(void)
    {}

    virtual bool Init();
    virtual bool LoadResources();
    virtual void Render();

    virtual void LoopController(float dt);

    void DrawHealth();

    virtual void GetXY(float& hotX, float& hotY);
    virtual void MoveTo(float hotX, float hotY);
    void AlignTo(ObjectState* pOtherState, float dx = 0.0f, float dy = 0.0f);
    virtual void GetTrajectory(Trajectory& tr);

    virtual bool GetSize(ge_rectF& rcSize);
    virtual bool GetBoundingRect(ge_rectF& rc);
    virtual bool GetBoundRegion(gfx_region& region);

    WeaponSlot& AddWeaponSlot(int weaponType, int slotId, float reloadDelay, float dx, float dy );
    WeaponSlot& AddWeaponSlot2( int slotId, WeaponSlotPtr& slotPtr );
    WeaponSlot& GetWeaponSlot(int slotId);
    void RemoveWeaponSlot(int slotId);
    void LoopAllSlots(float dt);
    void FireAllSlots();

    float Health() const { return m_health; }
    void SetMaximumHealth() { m_health = m_maxHealth; }
    void SetHealthStatus(float health, float maxHealth);

    bool IsOutOfScene( float dOutOffset );
    void RemoveIfOutOfScene( float dOutOffset );

    const DynamicTraits& GetDynamicTraits() const { return m_dynamicTraits; }
    void SetDynamicTraits(const DynamicTraits& traits) { m_dynamicTraits = traits; }

    // controller test interface
  public:
    ge::vector2d GetSpeed() const { return m_vSpeed; }

  public:
    static void LuaRegister( LuaScript& lua );

  protected:
    float m_hotX;
    float m_hotY;

    float m_health;
    float m_maxHealth;

    typedef std::map<int, WeaponSlotPtr> WeaponSlotsMap;
    WeaponSlotsMap m_slots;

    TextureId m_texId;
    gfx_animation m_anm;

    gfx_ttf_font m_fntHealth;

    DynamicTraits m_dynamicTraits;

    ControllerPtr m_ptrController;
    ge::vector2d m_vSpeed;

    LuaScript m_lua;
  };


}//namespace Game