#include "StdAfx.h"
#include "Weapon.h"
#include "ContextObject.h"
#include "GameScene.h"
#include "Explosions.h"
#include "AntishipMineExplosion.h"

namespace Game
{

Weapon::Weapon(GameScene* pScene) : ContextObject(pScene)
{
}

Weapon::~Weapon(void)
{
}

void Weapon::Explode()
{
  assert( CanTakeDamage() );

  Settings explSettings;
  GetSettings().CopyEntry<float>("explosion_scale", "scale", explSettings);
  GetSettings().CopyEntry<float>("explosion_damage", "damage", explSettings);
  GetSettings().CopyEntry<float>("explosion_fps", "fps", explSettings);
  GetSettings().CopyEntry<std::string>("explosion_type", "explosion_type", explSettings);
  GetSettings().CopyEntry<std::string>("explosion_align", "explosion_align", explSettings);

  Explode(explSettings);  
}

void Weapon::Explode( const Settings& explS )
{
  assert( CanTakeDamage() );

  ObjectPtr pExpl;

  std::string explType = explS.GetS("explosion_type", "ion_expl");
  if( explType == "ion_expl")
  {
    pExpl = ObjectPtr( new IonExplosion(Scene()) );
  }
  else if( explType == "shockwave_expl" )
  {
    pExpl = ObjectPtr( new Shockwave(Scene()) );
  }
  else if( explType == "antiship_expl" )
  {
    pExpl = ObjectPtr( new AntishipMineExplosion(Scene()) );
  }
  else
  {
    pExpl = ObjectPtr( new NormalBombExplosion(Scene()) );
  }

  pExpl->SetSettings(explS);
  pExpl->Init();

  std::string align = explS.GetS("explosion_align", "xcenter,ycenter");
  float x = 0, y = 0;

  if(align == "xcenter,ybottom")
  {    
    ge_rectF rcExplosion;
    pExpl->GetSize(rcExplosion);

    y = -rcExplosion.h() / 2.0f;
  }

  pExpl.get<IScreenObject>()->AlignToObject(this, x, y);  

  Scene()->LA_AddGameObject(pExpl);
  Scene()->LA_RemoveGameObject( ObjectPtr(this) );

  SetCanTakeDamage(false);
}

}
