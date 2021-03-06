#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

using namespace glm;

class CoreEngine;
class MouseEventListener
{
public:
	MouseEventListener(const MouseEventListener&) = delete;
	MouseEventListener(MouseEventListener&&) = delete;
	MouseEventListener& operator=(const MouseEventListener&) = delete;
	MouseEventListener& operator=(MouseEventListener&&) = delete;

	MouseEventListener() = delete;
	~MouseEventListener();

	static void RegisterEngineObject(CoreEngine* engine_);
	static void Update(SDL_Event e_);

	static void NotifyOfMousePressed(int buttonType_);
	static void NotifyOfMouseReleased(int buttonType_);
	static void NotifyOfMouseMove();
	static void NotifyOfMouseScroll(int y_);

	inline static ivec2 GetPreviousMousePosition() { return prevMouse; };
	inline static ivec2 GetMousePosition() { return mouse; };
	inline static ivec2 GetMouseOffset() { return ivec2(mouse.x - prevMouse.x, prevMouse.y - mouse.y); };

private:
	static CoreEngine* engineInstance;
	static ivec2 mouse, prevMouse;
	static bool firstUpdate;
	static void UpdateMousePosition();
};

#endif