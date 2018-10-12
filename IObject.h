#pragma once
#include "LuaScript.h"

#include "RefCountedPtrBase.h"
#include "RefCounterImpl.h"
#include "Settings.h"
#include "gerect.h"
#include "Gfx\gfx_regions.h"


#define DEFINE_STANDARD_OPS( className ) \
  \
  virtual const char* ClassName() const \
  {\
  static const char* pname = #className;\
  return pname;\
  }\

namespace Game
{

typedef int ObjectId;

class GameScene;

class IObject : public RefCounterImpl
{
public:
  IObject(GameScene* pScene);
  virtual ~IObject(void);  

  DEFINE_STANDARD_OPS(IObject)

  ObjectId id() const { return m_objectId; }
  
  void SetScene(GameScene* pScene) { m_pScene = pScene; }
  GameScene* Scene() { return m_pScene; }

  int TeamId() const { return m_teamId; }
  void SetTeamId(int val) { m_teamId = val; }

  int BehaviorId() const { return m_behaviorId; }
  void SetBehaviorId(int val) { m_behaviorId = val; }

  Settings& GetSettings() { return m_settings; }
  const Settings& GetSettings() const { return m_settings; }
  void SetSettings(const Settings& s) { m_settings = s; }
  float SGet(const char* name, float defVal);
  float SGet(const char* name);
  void  SSet(const char* name, float val);

  virtual void Init() = 0;
  virtual void Loop(float dt) = 0;
  virtual void Render() = 0;

  virtual bool GetSize(ge_rectF& rcSize) = 0;
  virtual bool GetBoundingRect(ge_rectF& rc) = 0;
  virtual bool GetBoundRegion(gfx_region& region) = 0;
  virtual bool IsIntersectingWith(IObject* pOtherObj);

  virtual void Event_DamagedBy( Game::IObject* pDamagingObject, float fDamage ) {}

  bool CanTakeDamage() const { return m_bCanTakeDamage; }
  void SetCanTakeDamage(bool val) { m_bCanTakeDamage = val; }

public:
  static void LuaRegister(LuaScript& lua);

private:
  GameScene* m_pScene; 
  int m_teamId;
  ObjectId m_objectId;
  int m_behaviorId;

  bool m_bCanTakeDamage;

  Settings m_settings;
};

typedef RefCountedPtrBase<IObject> ObjectPtr;

}//Game
