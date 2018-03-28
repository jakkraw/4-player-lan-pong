#pragma once
typedef int mixcolor;

inline SDL_Color toSDLColor(mixcolor c) {
	return{ Uint8((c >> 16) & 0xff),Uint8((c >> 8) & 0xff),Uint8(c & 0xff),Uint8((c >> 24) & 0xff) };
}


// WHITE & ALPHA25
enum color  {
	WHITE = 0xFFFFFFFF,
	BLACK = 0xFF000000,
	RED = 0xFFFF0000,
	BLUE = 0xFF0000FF,
	GREEN = 0xFF00FF00,
	CYAN = 0xFF00FFFF,
	MAGNETA = 0xFFFF00FF,
	LIGHT_BLUE = 0xFF32a69b,
	ALPHA25 = ~((0xFF / 4) << 24),
	ALPHA50 = ~((0xFF / 2) << 24),
	ALPHA75 = ~((0xFF * (3 / 4)) << 24),
	ALPHA65 =~((0xFF * (65 / 100)) << 24),
};
