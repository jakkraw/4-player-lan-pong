#include "input.h"

#define SDL_KEYCODE_TO_SCANCODE(X)  (X & ~SDLK_SCANCODE_MASK)


bool Input::initialize()
{
	for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
	{
		_keysDown[i] = false;
		_keysPressed[i] = false;
		_keysReleased[i] = false;
	}

	for (size_t i = 0; i < MOUSE_ARRAY_LEN; i++)
	{
		_mouseDown[i] = false;
		_mousePressed[i] = false;
		_mouseReleased[i] = false;
	}
	return true;
}

void Input::shutdown(){
}

void Input::handleEvents(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		setKeyDown(e.key.keysym.sym);
		break;

	case SDL_KEYUP:
		setKeyReleased(e.key.keysym.sym);
		setKeyUp(e.key.keysym.sym);
		break;

	case SDL_MOUSEMOTION:
		mX = e.motion.x;
		mY = e.motion.y;
		break;

	case SDL_MOUSEBUTTONDOWN:
		setMouseDown(e.button.button);		
		break;

	case SDL_MOUSEBUTTONUP:
		setMouseReleased(e.button.button);
		setMouseUp(e.button.button);
		break;

	default: break;
	}

}


bool Input::anyKeyPressed()
{
	for (int i = 0; i < KEYS_ARRAY_LEN; i++)
		if (_keysPressed[i] == true)
			return true;
	return false;
}


bool Input::keyPressed(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
		return _keysPressed[k];
	return false;
		
}
 
bool Input::keyDown(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
		return _keysDown[k];
	return false;
}

bool Input::keyReleased(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
		return _keysReleased[k];
	return false;
}


bool Input::mousePressed(int key)
{
	if (key < MOUSE_ARRAY_LEN)
		return _mousePressed[key];
	return false;
}

bool Input::mouseDown(int key)
{
	if (key < MOUSE_ARRAY_LEN)
		return _mouseDown[key];
	return false;
}

bool Input::mouseReleased(int key)
{
	if (key < MOUSE_ARRAY_LEN)
		return _mouseReleased[key];
	return false;
}


void Input::setKeyDown(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
	{
		if (!_keysDown[k])
			_keysPressed[k] = true;
		_keysDown[k] = true;
	}
		
}

void Input::setKeyUp(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
	{
		_keysDown[k] = false;
	}
		
}

void Input::setKeyReleased(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN && keyDown(key)){
		_keysReleased[k] = true;
	}

}


void Input::setMouseDown(int key)
{

	if (key < MOUSE_ARRAY_LEN)
	{
		if (!_mouseDown[key]) 
			_mousePressed[key] = true;
		_mouseDown[key] = true;
	}

}

void Input::setMouseReleased(int key)
{
	if (key < MOUSE_ARRAY_LEN && mouseDown(key))
		_mouseReleased[key] = true;
}

void Input::setMouseUp(int key)
{
	if (key < MOUSE_ARRAY_LEN)
		_mouseDown[key] = false;
}


void Input::clearKey(int key)
{
	Uint32 k = SDL_KEYCODE_TO_SCANCODE(key);
	if (k < KEYS_ARRAY_LEN)
	{
		_keysReleased[k] = false;
		_keysPressed[k] = false;
		_keysDown[k] = false;
	}
}

void Input::clearMouse(int key)
{
	if (key < MOUSE_ARRAY_LEN)
	{
		_mouseDown[key] = false;
		_mousePressed[key] = false;
		_mouseReleased[key] = false;
	}
}

void Input::clearAll(bool withKeysDown)
{
	for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
	{
		_keysPressed[i] = false;
		_keysReleased[i] = false;
		if(withKeysDown)
			_keysDown[i] = false;
	}

	for (size_t i = 0; i < MOUSE_ARRAY_LEN; i++)
	{
		_mousePressed[i] = false;
		_mouseReleased[i] = false;
		if(withKeysDown)
			_mouseDown[i] = false;
	}

}
