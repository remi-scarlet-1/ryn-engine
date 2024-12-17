#pragma once

// Contains tools available in debug & release configurations
namespace ryn {
	class Debug {
	public:
		// Render a simple ImGui menu listing runtime statistics
		static void showStatisticsMenu(bool* open);

		// Render a simple ImGui menu listing all current inputs
		static void showInputVisualizerMenu(bool* open);
	};
}
