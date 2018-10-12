#include "StdAfx.h"
#include "IObject.h"
#include "Framework.h"
#include "Explosions.h"
#include "BombSplash.h"
#include "RegSubCrashState.h"
#include "AtomSubFallState.h"
#include "RegSubFallState.h"
#include "Bubbles.h"
#include <assert.h>

int GetNextObjectId()
{
  static int g_nextObjectId = 1;
  return g_nextObjectId++;
}

namespace Game
{

IObject::IObject( GameScene* pScene ) 
  : m_pScene(pScene), m_teamId(-1), m_behaviorId(0), m_bCanTakeDamage(true)
{
  m_objectId = GetNextObjectId();
}


IObject::~IObject( void )
{}


bool IObject::IsIntersectingWith( IObject* pOtherObj )
{
  if(dynamic_cast<AtomSubmarine*>(this) != NULL)
  {
    int t = 0;
  }

  if(dynamic_cast<NormalBombExplosion*>(pOtherObj) != NULL
    && dynamic_cast<NormalBombExplosion*>(this)!= NULL)
  {
    return false;
  }

  if(dynamic_cast<Bubbles*>(pOtherObj) != NULL
    || dynamic_cast<Bubbles*>(this)!= NULL)
  {
    return false;
  }

  if(dynamic_cast<RegSubFallState*>(pOtherObj) != NULL
    || dynamic_cast<RegSubFallState*>(this)!= NULL)
  {
    return false;
  }

  if(dynamic_cast<AtomSubFallState*>(pOtherObj) != NULL
    || dynamic_cast<AtomSubFallState*>(this)!= NULL)
  {
    return false;
  }

  if(dynamic_cast<RegSubCrashState*>(pOtherObj) != NULL
    || dynamic_cast<RegSubCrashState*>(this) != NULL)
  {
    return false;
  }

  if(dynamic_cast<BombSplash*>(pOtherObj) != NULL
    || dynamic_cast<BombSplash*>(this)!= NULL)
  {
    return false;
  }

  ge_rectF rc1,rc2;
  if(!GetBoundingRect(rc1) || !pOtherObj->GetBoundingRect(rc2))
    return false;

  if(!rc1.intersect(rc2))
    return false;

  gfx_region rgn1, rgn2;
  GetBoundRegion(rgn1);
  pOtherObj->GetBoundRegion(rgn2);

  if(rgn1.IsEmpty() || rgn2.IsEmpty())
    return false;

  const gfx_region::RegionItem& itemsArr1 = rgn1.mItems;
  const gfx_region::RegionItem& itemsArr2 = rgn2.mItems;
  
  
  for(size_t i = 0; i < itemsArr1.size(); i++)
  {
    const ge_rectF& r1 = itemsArr1[i];
    for(size_t j = 0; j < itemsArr2.size(); j++)
    {
      const ge_rectF& r2 = itemsArr2[j];

      if((int)r1.top() != (int)r2.top())
        continue;
      
      if((int)r1.left() > (int)r2.right())
        continue;

      if((int)r1.right() < (int)r2.left())
        continue;

      return true;
    }
  }

  return false;
}

float IObject::SGet( const char* name, float defVal )
{
  return GetSettings().Get(name, defVal);
}

float IObject::SGet( const char* name )
{
  return SGet(name, 0.0f);
}

void IObject::SSet( const char* name, float val )
{
  m_settings.Set(name, val);
}

void IObject::LuaRegister( LuaScript& lua )
{
  getGlobalNamespace (lua.State())
    .beginNamespace ("luabridge")
      .beginClass <IObject> ("IObject")
        .addFunction ("id", &IObject::id)
      .endClass ()      
    .endNamespace ();
}


}//Game