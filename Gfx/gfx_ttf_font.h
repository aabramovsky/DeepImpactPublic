#pragma once

#include <SDL.h>
#include <stdio.h>
#include "SDL_Image.h"
#include "gerect.h"
#include "gepoint.h"
#include "SDL_ttf.h"
#include <string>
#include "Utility.h"

class gfx_ttf_font_module
{
  static int refCount;

private:
  gfx_ttf_font_module() {};

  static void init();
  static void cleanup();

public:
  static void addRef();
  static void release();
};

//////////////////////////////////////////////////////////////////////////
//
//
class gfx_ttf_font
{
public:
  enum AlignH
  {
    hLeft, hCenter, hRight
  };

  enum AlignV
  {
    vTop, vCenter, vBottom
  };

public:
  gfx_ttf_font();
	~gfx_ttf_font();  

  void draw_solid(float x, float y, AlignH alighH, AlignV alignV, const std::string& text, int ptsize, const SDL_Color& clr);
  void draw_solid(float x, float y, const std::string& text, int ptsize, const SDL_Color& clr);
  void draw_solid(SDL_Renderer *pRenderer, float x, float y, const std::string& text, int ptsize, const SDL_Color& clr);

  void size_string(const std::string& text, int ptsize, int *w, int *h);

private:
  void load_font( int ptsize );
  void clear_font();
  void make_text_texture(SDL_Renderer *pRenderer, const std::string& text, const SDL_Color& clr);
  void clear_texture();

private:
  TTF_Font *m_pFont;
  int m_ptsize;

  std::string m_text;
  SDL_Texture* m_pTexture;
  SDL_Color m_clr;

  AlignH m_alignH;
  AlignV m_alignV;

  Files::FileBuffer fbuf;
};