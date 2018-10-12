#pragma once

#include <SDL.h>
#include <stdio.h>
#include "SDL_Image.h"
#include "gerect.h"
#include "gepoint.h"

class gfx_sprite
{
public:
  gfx_sprite();
	gfx_sprite(SDL_Texture* pTex);
	~gfx_sprite();

  void init(SDL_Texture* pTex);

	void render(float x, float y, float rotAngle = 0.0f);
  void render(float x, float y, float w, float h);
  void render(SDL_Renderer *pDstRenderer, float x, float y, float rotAngle = 0.0f);

	float width() const { return _width * _scale; }
	float height() const { return _height * _scale; }

	int texture_width() const { return _width; }
	int texture_height() const { return _height; }

	void set_source_rect(const ge_rectI& rcSource);
	void set_source_rect(int x, int y, int w, int h);

  const ge_point2dF& get_hot_spot() const { return _hotSpot; }
  void set_hot_spot(float x, float y);

	void set_flipped_h(bool bHorisontalFlip) { _bHorisontalFlip = bHorisontalFlip; }
  bool is_flipped_h() const { return _bHorisontalFlip; }
	
  float get_scale() const { return _scale; }
  void set_scale(float scale) { _scale = scale; }

private:
	SDL_Texture* _pTex;

	// width and height in pixels
	int _width;
	int _height;

	ge_rectI _srcRect;
	ge_point2dF _hotSpot;

	float _scale;

	bool _bHorisontalFlip;
};