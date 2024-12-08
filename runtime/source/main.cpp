#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "logo.h"
#include "window.h"

// TODO: add imgui (debug info)
// TODO: cleanup code (finish window class etc.)
// TODO: cleanup build system

static bool s_showStats = false;

int main(int argc, char** argv)
{
	rn::Window window = rn::Window("Engine Window Test!", 1600, 900, false, false);

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();

	// Get window resolution
	int windowWidth, windowHeight;
	window.getResolution(&windowWidth, &windowHeight);

	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
	init.platformData.nwh = window.getNativeHandle();
	init.resolution.width = (uint32_t)windowWidth;
	init.resolution.height = (uint32_t)windowHeight;
	init.resolution.reset = BGFX_RESET_NONE;

	if (!bgfx::init(init))
		return 1;

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId view = 0;
	bgfx::setViewClear(view, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(view, 0, 0, bgfx::BackbufferRatio::Equal);

	while (!window.getShouldClose()) {
		window.pollEvents();

		// Handle window resize.
		//int oldWidth = windowWidth, oldHeight = windowHeight;
		//glfwGetWindowSize(window, &windowWidth, &windowHeight);
		//if (windowWidth != oldWidth || windowHeight != oldHeight) {
		//	bgfx::reset((uint32_t)windowWidth, (uint32_t)windowHeight, BGFX_RESET_VSYNC);
		//	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		//}

		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(view);

		// Use debug font to print information about this example.
		bgfx::dbgTextClear();

		bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(windowWidth / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(windowHeight / 2 / 16), 6) - 6, 40, 12, s_logo, 160);

		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

		// Enable stats or debug text.
		bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
	}

	bgfx::shutdown();
	glfwTerminate();
	return 0;
}