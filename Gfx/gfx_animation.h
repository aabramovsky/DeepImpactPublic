#pragma once

#include <SDL.h>
#include <stdio.h>
#include "SDL_Image.h"
#include "gerect.h"
#include "gepoint.h"
#include "gfx_sprite.h"
#include "gfx_regions.h"

class gfx_animation
{
public:
	enum PlayMode
	{
		playForward, 
		playLoop,
	};

public:
  gfx_animation();
	gfx_animation(SDL_Texture* pTex, int numFrames, float fps);
	~gfx_animation();

  void init(SDL_Texture* pTex, int numFrames, float fps);
  void init(SDL_Texture* pTex, int numFrames, float fps, FrameRegionsPtr& frPtr);

  void render(float x, float y, float rotAngle = 0.0f);
  void render(float x, float y, float w, float h);
  void render(SDL_Renderer *pDstRenderer, float x, float y, float rotAngle = 0.0f);

	void play();	
	void stop();
	//void resume();
	bool update(float dt);
	bool is_playing() const;

  PlayMode get_mode() const;
	void set_mode(PlayMode mode);
	
  int get_frame() const { return _curFrame; }
  void set_frame(int n);	

  float get_scale() const;
  void set_scale(float scale);
  
  void set_flipped_h(bool bHorisontalFlip);
  bool is_flipped_h() const;

  float fps() const;
  void set_fps(float fps);

  ge_rectI frame_size();
  ge_rectF frame_box(float x, float y);
  ge_rectF bounding_box(float x, float y);
  void bounding_region(float x, float y, gfx_region& res);

private:
  void init_fields();

private:
	gfx_sprite _sprite;

	int _numFrames;
	float _fps;

	int _curFrame;

	float _fSpeed;
	float _secsSinceLastFrame;

	bool _bPlaying;

	int _frameWidth;
	int _frameHeight;

  PlayMode _playMode;

  ge_point2dF _hotSpot;

  FrameRegionsPtr m_regions;
};