#include "stdafx.h"
#include "gfx_sprite.h"
#include "Framework.h"

gfx_sprite::gfx_sprite() 
  : _pTex(NULL), _width(0), _height(0), _scale(0), _bHorisontalFlip(false)
{}

gfx_sprite::gfx_sprite(SDL_Texture* pTex)
  : _pTex(NULL), _width(0), _height(0), _scale(0), _bHorisontalFlip(false)
{
	init(pTex);
}

gfx_sprite::~gfx_sprite()
{
}

void gfx_sprite::init( SDL_Texture* pTex )
{
  _pTex = pTex;

  SDL_QueryTexture(pTex, NULL, NULL, &_width, &_height);
  _srcRect.set(0,0,_width,_height);

  _scale = 1.0f;  
}

void gfx_sprite::render( SDL_Renderer *pDstRenderer, float x, float y, float rotAngle /*= 0.0f*/ )
{
	SDL_Rect rcSrc = {_srcRect.x(), _srcRect.y(), _srcRect.w(), _srcRect.h()};

	ge_rectF dstF = _srcRect;
	dstF *= _scale;

	float dx = x - _hotSpot.x()*_scale;
	float dy = y - _hotSpot.y()*_scale;

	dstF.moveBy(dx, dy);
	
	SDL_Rect rcDst = {dstF.x(), dstF.y(), dstF.w(), dstF.h()};

	SDL_Point ptRotCenter = {_hotSpot.x()*_scale, _hotSpot.y()*_scale};
  
	SDL_RendererFlip srf = SDL_FLIP_NONE;
	if(_bHorisontalFlip)
		srf = SDL_FLIP_HORIZONTAL;
	
	SDL_RenderCopyEx(pDstRenderer, _pTex, &rcSrc, &rcDst, RadToDegrees(rotAngle), &ptRotCenter, srf);

  if(0)
  {
    SDL_SetRenderDrawColor(pDstRenderer, 0, 255, 0 ,0);
    SDL_RenderDrawRect(pDstRenderer, &rcDst);
    
    int rsize = 2;
    //SDL_Rect rcTargetPt = {x-rsize, y-rsize, rsize*2, rsize*2 };
    //SDL_RenderFillRect(pDstRenderer, &rcTargetPt);
  }
}	

void gfx_sprite::render( float x, float y, float rotAngle /*= 0.0f*/ )
{
  render( sdl_renderer(), x, y, rotAngle );
}

void gfx_sprite::render( float x, float y, float w, float h )
{
  SDL_Rect rcSrc = {_srcRect.x(), _srcRect.y(), _srcRect.w(), _srcRect.h()};
  SDL_Rect rcDst = {x, y, w, h};

  SDL_RendererFlip srf = SDL_FLIP_NONE;
  if(_bHorisontalFlip)
    srf = SDL_FLIP_HORIZONTAL;

  SDL_RenderCopyEx(sdl_renderer(), _pTex, &rcSrc, &rcDst, 0, NULL, srf);
}

void gfx_sprite::set_source_rect( const ge_rectI& rcSource )
{
	_srcRect = rcSource;
}

void gfx_sprite::set_source_rect( int x, int y, int w, int h )
{
	_srcRect.set(x, y, w, h);
}

void gfx_sprite::set_hot_spot( float x, float y )
{
  _hotSpot.set(x, y);
}
