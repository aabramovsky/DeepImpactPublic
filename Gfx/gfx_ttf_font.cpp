#include "stdafx.h"
#include "gfx_ttf_font.h"
#include "Framework.h"
#include "Utility.h"

int gfx_ttf_font_module::refCount = 0;

void gfx_ttf_font_module::init()
{
  if(TTF_WasInit() == 0)
    TTF_Init();

}

void gfx_ttf_font_module::cleanup()
{
  if(TTF_WasInit() == 1)
    TTF_Quit();
}

void gfx_ttf_font_module::addRef()
{
  if(refCount == 0)
    init();

  ++refCount;
}

void gfx_ttf_font_module::release()
{
  --refCount;
  if(refCount == 0)
    cleanup();
}

//////////////////////////////////////////////////////////////////////////
//

gfx_ttf_font::gfx_ttf_font()
  : m_pFont(NULL), m_ptsize(0), m_pTexture(NULL), m_alignH(hLeft), m_alignV(vTop)
{
  gfx_ttf_font_module::addRef();
}

gfx_ttf_font::~gfx_ttf_font()
{
  clear_font();
  clear_texture();

  gfx_ttf_font_module::release();
}

void gfx_ttf_font::draw_solid( SDL_Renderer *pRenderer, float x, float y, const std::string& text, int ptsize, const SDL_Color& clr )
{
  if(m_ptsize != ptsize || m_pFont == NULL)
    load_font(ptsize);

  if(m_pFont == NULL)
    return;

  if(m_text != text || !IsEqual(m_clr, clr) || m_pTexture == NULL)
    make_text_texture(pRenderer, text, clr);

  gfx_sprite spr(m_pTexture);
  
  float hsX=0.0f, hsY=0.0f;  
  
  switch(m_alignH)
  {
  case hLeft: hsX = 0; break;
  case hCenter: hsX = spr.texture_width()/2.0f; break;
  case hRight: hsX = spr.texture_width(); break;
  }

  switch(m_alignV)
  {
  case vTop: hsY = 0; break;
  case vCenter: hsY = spr.texture_height()/2.0f; break;
  case vBottom: hsY = spr.texture_height(); break;
  }

  spr.set_hot_spot(hsX, hsY);
  spr.render(pRenderer, x, y);
}

void gfx_ttf_font::draw_solid( float x, float y, const std::string& text, int ptsize, const SDL_Color& clr )
{
  draw_solid(sdl_renderer(), x, y, text, ptsize, clr);
}

void gfx_ttf_font::draw_solid( float x, float y, AlignH alignH, AlignV alignV, const std::string& text, int ptsize, const SDL_Color& clr )
{
  m_alignH = alignH;
  m_alignV = alignV;
  draw_solid(x, y, text, ptsize, clr);
}

void gfx_ttf_font::size_string( const std::string& text, int ptsize, int *w, int *h )
{
  if(m_ptsize != ptsize || m_pFont == NULL)
    load_font(ptsize);

  if(m_pFont == NULL)
    return;

  TTF_SizeText(m_pFont, text.c_str(), w, h);
}

void gfx_ttf_font::load_font( int ptsize )
{
  clear_font();

  std::string fontPath = Files::GetFontsFolderPath() + "\\arial.ttf";
  if( !fbuf.Load(fontPath) )
  {
    LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("failed to create file buffer; path=") << fontPath.c_str());
    return;
  }
  fbuf.bAutoFreeRW = false;
  //m_pFont = TTF_OpenFont(fontPath.c_str(), ptsize);
  m_pFont = TTF_OpenFontIndexRW(fbuf.GetRWops(), 1, ptsize, 0);
  m_ptsize = ptsize;
}

void gfx_ttf_font::clear_font()
{
  if(m_pFont != NULL)
  {
    TTF_CloseFont(m_pFont);
    m_pFont = NULL;
  }
}

void gfx_ttf_font::make_text_texture(SDL_Renderer *pRenderer, const std::string& text, const SDL_Color& clr)
{
  clear_texture();

  SDL_Surface *pTextSurf = TTF_RenderText_Solid(m_pFont, text.c_str(), clr);
  m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurf);
  SDL_FreeSurface(pTextSurf);

  m_text = text;
  m_clr = clr;
}

void gfx_ttf_font::clear_texture()
{
  SDL_DestroyTexture(m_pTexture);
  m_pTexture = NULL;
  m_text.clear();
}
