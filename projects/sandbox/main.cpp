#include "window.h"

int main() {
	ryn::window::WindowSettings windowSettings("Test", false, 1920, 1080, false);
	ryn::window::Window window(windowSettings);

	while (!window.getShouldClose()) {
		window.pollEvents();
		// TODO: rest of the main loop
	}
}