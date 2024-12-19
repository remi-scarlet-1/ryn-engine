#include "window.h"
#include "input.h"
#include "debug.h"

#include <iostream>

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_bgfx.h>
#include <imgui/backends/imgui_impl_glfw.h>

static bool showStats = true;
static bool useVsync = true;

int main(int argc, char** argv)
{
	// Initialize window
	ryn::Window window = ryn::Window("Ryn test window", 1600, 900, true, showStats);

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
	init.resolution.reset = useVsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;

	if (!bgfx::init(init))
		return 1;

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId view = 0;
	bgfx::setViewClear(view, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(view, 0, 0, bgfx::BackbufferRatio::Equal);

	// Handle window resize (bgfx)
	window.setResizeCallback([](int width, int height) { 
		bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
		bgfx::setViewRect(view, 0, 0, bgfx::BackbufferRatio::Equal);
	});

	// Initialize imgui
	ryn::Debug::initializeImGui(window.getGlfwHandle(), view);

	while (!window.getShouldClose()) {
		window.pollEvents();

		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(view);

		// Draw debug menus
		ryn::Debug::beginImGuiFrame();
		ryn::Debug::showRuntimeStatisticsMenu(&showStats);
		ryn::Debug::showInputVisualizerMenu(&showStats);
		ryn::Debug::renderImGuiFrame();

		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
	}

	// Shutdown imgui
	ryn::Debug::shutdownImGui();

	bgfx::shutdown();
	return 0;
}