#pragma once
#include <map>
#include <memory>
#include "SDL.h"
#include "Gfx\gfx_animation.h"

class GfxAnimation;

enum TextureId
{
  texReefBgAnim,
  texReefWaterline,

  texShipHull,
  texShipWavesIdle,
  texShipWavesAccel,
  texShipWavesFullSpeed,

  texRegSubMove,
  texRegSubCrash,
  texRegSubFall,
  texRegSubBottom,

  texNormalBombOnShip,
  texNormalBombSink,
  texNormalBombExplosion,

  texBombSplash,
  texBubbles,

  texAtomSubMove,
  texAtomSubCrash,
  texAtomSubFall,
  texAtomSubBottom,

  texRocketFly,
  texRocketExhaust,

  texIonBomb,
  texIonExplosion,

  texShockwave,

  texDockSceneBg,
  texDockSceneCursor,

  texBonusRecovery,
  texBonusBombSpeed,
  texBonusShield,
  texBonusShipSpeed,


  __texCount
};

typedef void* hgeFont; // TEST TEST BUILD
typedef DWORD HTEXTURE; // TEST TEST BUILD
typedef std::auto_ptr<hgeFont> hgeFontPtr;



class ResourceManager
{
public:
  ~ResourceManager();
  static ResourceManager& instance();

  void Init(SDL_Renderer* pRenderer);

  SDL_Texture* LoadTexture(TextureId id);  

  bool MakeAnimation(TextureId texid, gfx_animation& anmOut);
  bool MakeAnimation(TextureId texid, float scale, gfx_animation& anmOut);
  bool MakeSprite(TextureId texid, gfx_sprite& sprOut);

private:
  ResourceManager();

  SDL_Texture* FindTexture(TextureId id);
  SDL_Surface* FindSurface(TextureId id);

  void InitAnimationFrameRegionsAndBoundings(TextureId id);

private:
  static ResourceManager m_instance;

  typedef std::map<TextureId, SDL_Texture*> TextureMap;
  TextureMap m_textures;

  typedef std::map<TextureId, SDL_Surface*> SurfaceMap;
  SurfaceMap m_surfaces;

  typedef std::map<TextureId, FrameRegionsPtr> AnimationRegionsMap;
  AnimationRegionsMap m_anmRegions;

  SDL_Renderer* m_pRenderer;

  std::string m_strResourcePath;
};

#define resources() ResourceManager::instance()