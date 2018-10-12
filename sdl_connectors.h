#pragma once
#include "SDL_rect.h"
#include "gerect.h"

inline SDL_Rect ToSDL(const ge_rectF& rc)
{
  SDL_Rect rcS = {rc.left(), rc.top(), rc.w(), rc.h()};
  return rcS;
}

