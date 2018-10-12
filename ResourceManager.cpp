#include "StdAfx.h"
#include "ResourceManager.h"
#include "Framework.h"
#include <map>
#include <utility>
#include <string>
#include "SDL_Image.h"
#include "Utility.h"
#include <log4cplus/logger.h>

struct TextureTrais
{
  const char* path;
  int numFrames;
  float fps;
};

TextureTrais g_textures[__texCount] = 
{
  { "Resources\\reef_background\\bg_anim.png", 7, 5 },
  { "Resources\\reef_waterline\\reef_waterline.png", 7, 5 },

  { "Resources\\ship\\hull.png", 30, 13 },
  { "Resources\\ship\\waves_idle.png", 12, 13 },
  { "Resources\\ship\\waves_accelerating.png", 23, 6 },
  { "Resources\\ship\\waves_full_speed.png", 5, 6 },  
 
  { "Resources\\regular_submarine\\reg_sub_move.png", 12, 7 },  
  { "Resources\\regular_submarine\\reg_sub_crash.png", 19, 10 },
  { "Resources\\regular_submarine\\reg_sub_fall.png", 2, 10 },
  { "Resources\\regular_submarine\\reg_sub_bottom.png", 17, 10 },

  { "Resources\\normal_bomb\\normal_bomb_fall.png", 1, 1 },
  { "Resources\\normal_bomb\\normal_bomb_fallInWater.png", 27, 9 },
  { "Resources\\normal_bomb\\normal_bomb_expl.png", 6, 9 },

  { "Resources\\effects\\bomb_splash.png", 14, 7 },
  { "Resources\\effects\\normal_bomb_expl_bubles.png", 12, 9 },

  { "Resources\\atom_submarina\\atom_submarine_move.png", 12, 5 },
  { "Resources\\atom_submarina\\atom_submarine_explosion.png", 23, 7 },
  { "Resources\\atom_submarina\\atom_submarine_fall.png", 3, 7 },
  { "Resources\\atom_submarina\\atom_submarine_bottom.png", 25, 7 },

  { "Resources\\rocket\\rocket_fly.png", 4, 3 },
  { "Resources\\rocket\\rocket_exhaust.png", 0, 0 },

  { "Resources\\ion_bomb\\ion_bomb_fall.png", 0, 0 },
  { "Resources\\ion_bomb\\ion_explosion.png", 11, 17 },

  { "Resources\\effects\\shockwave.png", 4, 4 },

  { "Resources\\dock_scene\\dock_scene_bg.png", 1, 1000 },
  { "Resources\\dock_scene\\cursor.png", 0, 0 },

  { "Resources\\bonus\\recovery.png", 11, 10 },
  { "Resources\\bonus\\bomb_speed.png", 11, 10 },
  { "Resources\\bonus\\shield.png", 11, 10 },
  { "Resources\\bonus\\ship_speed.png", 11, 10 },
};


typedef std::pair<TextureId, float> AnimationKey;
typedef std::map<AnimationKey, GfxAnimation*> AnimationsMap;

AnimationsMap g_map;

ResourceManager ResourceManager::m_instance;

ResourceManager::ResourceManager() : m_pRenderer(NULL)
{
}

ResourceManager::~ResourceManager()
{
  TextureMap::iterator itTex = m_textures.begin();
  while( itTex != m_textures.end() )
  {
    SDL_DestroyTexture(itTex->second);
    ++itTex;
  }

  m_textures.clear();

  SurfaceMap::iterator itSurf = m_surfaces.begin();
  while( itSurf != m_surfaces.end() )
  {
    SDL_FreeSurface(itSurf->second);
    ++itSurf;
  }

  m_surfaces.clear();
}

void ResourceManager::Init(SDL_Renderer* pRenderer)
{
  m_pRenderer = pRenderer;
  m_strResourcePath = Files::GetRespackPath();
} 

SDL_Texture* ResourceManager::LoadTexture( TextureId id )
{
  SDL_Texture* res = FindTexture(id);
  if( res != NULL )
    return res;

  const char* name = g_textures[id].path;

  SDL_Surface *surface = Files::LoadSurface(name);
  if (surface == NULL)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Files::LoadSurface failed; id=") << std::dec << id << "; name=" << name);
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer(), surface);
  if(texture == NULL)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("SDL_CreateTextureFromSurface failed; id=") << std::dec << id << "; name=" << name);
    return NULL;
  }
  
  m_textures[id] = texture;
  m_surfaces[id] = surface;

  InitAnimationFrameRegionsAndBoundings(id);

  return texture;
}

void ResourceManager::InitAnimationFrameRegionsAndBoundings( TextureId id )
{
  SDL_Surface *surface = FindSurface(id);
  if(surface == NULL)
    return;

  const int numFrames = g_textures[id].numFrames;
  if(numFrames == 0)
    return;

  FrameRegionsPtr regionsPtr(new FrameRegions);

  const int fw = surface->w / numFrames;
  const int fh = surface->h;
  const int pitch = surface->pitch / 4; //pitch in pixels

  const Uint32 transparentPixel = SDL_MapRGBA(surface->format, 255, 0, 255, SDL_ALPHA_TRANSPARENT);
  Uint32 clr = 0;
  bool bProcessingOpaque = false;
  int xS = -1;
  Uint8  r, g, b, a;

  SDL_LockSurface(surface);

  Uint32* pPixels = (Uint32*)surface->pixels;

  for(int iFrame = 0; iFrame < numFrames; iFrame++)
  {
    gfx_region curFrameRegion;

    for(int y = 0; y < fh; y++)
    {
      for(int x = 0; x < fw; x++)
      {
        clr = pPixels[y*pitch + iFrame*fw + x];
        SDL_GetRGBA(clr, surface->format, &r, &g, &b, &a);
        
        if( a == SDL_ALPHA_TRANSPARENT )
        {
          if( bProcessingOpaque )
          {
            curFrameRegion.Push( ge_rectF(xS, y, x-xS-1, 0) );
            bProcessingOpaque = false;
          }
        }
        else // point not transparent
        {
          if( !bProcessingOpaque )
          {
            xS = x;
            bProcessingOpaque = true;
          }
        }
      }

      if( bProcessingOpaque )
        curFrameRegion.Push( ge_rectF(xS, y, fw-xS-1, 0) );

      bProcessingOpaque = false;
    }

    (*regionsPtr)[iFrame] = curFrameRegion;
  }

  SDL_UnlockSurface(surface);

  m_anmRegions[id] = regionsPtr;
}

SDL_Texture* ResourceManager::FindTexture( TextureId id )
{
  TextureMap::iterator it = m_textures.find(id);
  if( it == m_textures.end() )
    return NULL;

  return it->second;
}

SDL_Surface* ResourceManager::FindSurface( TextureId id )
{
  SurfaceMap::iterator it = m_surfaces.find(id);
  if( it == m_surfaces.end() )
    return NULL;

  return it->second;
}

ResourceManager& ResourceManager::instance()
{
  return m_instance;
}

bool ResourceManager::MakeAnimation( TextureId texid, gfx_animation& anmOut )
{
  SDL_Texture* pTex = LoadTexture(texid);
  if(pTex == NULL)
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Texture loading failed; texId=") << std::dec << texid);
    return false;
  }

  anmOut.init(pTex, g_textures[texid].numFrames, g_textures[texid].fps, m_anmRegions[texid]);
  return true;
}

bool ResourceManager::MakeAnimation( TextureId texid, float scale, gfx_animation& anmOut )
{
  if( !MakeAnimation(texid, anmOut) )
    return false;

  anmOut.set_scale(scale);
  return true;
}

bool ResourceManager::MakeSprite(TextureId texid, gfx_sprite& sprOut)
{
  SDL_Texture* pTex = LoadTexture(texid);
  if(pTex == NULL)
    return false;

  sprOut.init(pTex);
  return true;
}

//void ResourceManager::MakeGfxAnimation( TextureId texid, int nframes, float FPS, float scale, GfxAnimation& outRes )
//{
//  AnimationKey key(texid, scale);
//  AnimationsMap::iterator it = g_map.find(key);
//  if(it == g_map.end())
//  {
//    GfxAnimation* pOriginal = new GfxAnimation();
//    pOriginal->Create(texid, nframes, FPS, scale);
//    g_map[key] = pOriginal;
//  }
//
//  GfxAnimation* pOriginal = g_map[key];
//  outRes.CopyFrom(*pOriginal);
//}