#pragma once

#include <cstdint>

// Forward declarations
struct GLFWwindow;

namespace bgfx {
	typedef uint16_t ViewId;
}

// Contains tools available in debug & release configurations
namespace ryn {
	class Debug {
	public:
		// Initialize ImGui for the application.
		// This must be called once during application startup to set up ImGui's context.
		static void initializeImGui(GLFWwindow* glfwWindow, const bgfx::ViewId bgfxViewId);

		// Begins a new ImGui frame.
		// This function should be called at the beginning of each frame to prepare ImGui for rendering.
		static void beginImGuiFrame();

		// Render the ImGui frame.
		// Call this function at the end of a frame to render all ImGui elements to the screen.
		static void renderImGuiFrame();

		// Shutdown and clean up ImGui resources
		// This must be called once during application shutdown to free all ImGui resources.
		static void shutdownImGui();

		// Show a menu displaying runtime statistics.
		static void showRuntimeStatisticsMenu(bool* open);

		// Show a menu visualizing all current inputs.
		static void showInputVisualizerMenu(bool* open);
	};
}
