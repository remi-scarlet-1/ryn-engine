#pragma once

// Contains tools available in debug & release configurations
namespace ryn::debug {
	// Render a simple ImGui menu listing runtime statistics
	void renderStatisticsMenu(bool* open);
	// Render a simple ImGui menu listing all current inputs
	void renderInputDebugMenu(bool* open);
}
