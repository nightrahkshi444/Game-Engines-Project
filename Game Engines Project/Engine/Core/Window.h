#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>

#include "Debug.h"

using namespace std;

class Window
{
public:
	Window();
	~Window();
	bool OnCreate(string name_, int width_, int height_);
	void OnDestroy();

	inline int GetWidth() const { return width; };
	inline int GetHeight() const { return height; };
	inline SDL_Window* GetWindow() const { return window; };

private:
	void SetPreAttributes();
	void SetPostAttributes();

	int width;
	int height;
	SDL_Window* window;
	SDL_GLContext context;
};

#endif