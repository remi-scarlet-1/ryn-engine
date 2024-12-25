//#pragma once
//
//#include "public/defines.h"
//
//struct GLFWwindow;
//
//namespace bgfx {
//	typedef uint16_t ViewId;
//}
//
//namespace ryn {
//	class Debug {
//	public:
//		static void initializeImGui(GLFWwindow* glfwWindow, const bgfx::ViewId bgfxViewId);
//		static void beginImGuiFrame();
//		static void renderImGuiFrame();
//		static void shutdownImGui();
//
//		static void drawDebugMenu(bool* open);
//
//	private:
//		struct FpsTracker {
//			const double UPDATE_INTERVAL = 200;
//			int64_t lastTime = 0;
//			int frameCount = 0;
//			double averageFps = 0.0;
//
//			void update();
//			double getAverageFps() const;
//		};
//
//		static void drawStatsMenu();
//		static void drawInputVisualizerMenu();
//
//#pragma region Helper functions
//		static void TooltipMarker(const char* desc);
//#pragma endregion
//	};
//}
