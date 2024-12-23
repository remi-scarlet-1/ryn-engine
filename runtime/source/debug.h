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

		// Draws a main debug menu providing essential runtime statistics and access to different debugging tools.
		static void drawDebugMenu(bool* open);

	private:
		struct FpsTracker {
			const double UPDATE_INTERVAL = 200;  // 200 milliseconds (0.2s)
			int64_t lastTime = 0;
			int frameCount = 0;
			double averageFps = 0.0;

			void update();
			double getAverageFps() const;
		};

		static void drawStatsMenu();
		static void drawInputVisualizerMenu();

#pragma region Helper functions
		static void TooltipMarker(const char* desc);
#pragma endregion
	};
}
