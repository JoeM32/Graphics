#include "../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Texture Mapping!", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;

	}
	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;

	}
	float rotate = 0.0f;
	float scale = 1.0f;
	while (w.UpdateWindow() &&
		!Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
			--rotate;
			renderer.UpdateTextureMatrix(rotate, scale);

		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
			++rotate;
			renderer.UpdateTextureMatrix(rotate, scale);

		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS)) {
			scale*=1.1f;
			renderer.UpdateTextureMatrix(rotate, scale);

		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) {
			scale *= 0.9f;
			renderer.UpdateTextureMatrix(rotate, scale);

		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.ToggleFiltering();

		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.ToggleRepeating();

		}
		renderer.RenderScene();
		renderer.SwapBuffers();

	}
	return 0;

}