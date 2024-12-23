#include "debug.h"
#include "input.h"

#include <chrono>

#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_bgfx.h>
#include <imgui/backends/imgui_impl_glfw.h>

void ryn::Debug::initializeImGui(GLFWwindow* glfwWindow, const bgfx::ViewId bgfxViewId) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.IniFilename = NULL;

	ImGui_Implbgfx_Init(bgfxViewId);
	ImGui_ImplGlfw_InitForOther(glfwWindow, true);
}

void ryn::Debug::beginImGuiFrame() {
	ImGui_Implbgfx_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ryn::Debug::renderImGuiFrame() {
	ImGui::Render();
	ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
}

void ryn::Debug::shutdownImGui() {
	ImGui_Implbgfx_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ryn::Debug::drawDebugMenu(bool* open) {
	ImGui::SetNextWindowSize(ImVec2(280, 380), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);

	ImGui::Begin("Debug", open, ImGuiWindowFlags_NoTitleBar);

	if (ImGui::BeginTabBar("DebugTabs")) {
		if (ImGui::BeginTabItem("Stats")) {
			drawStatsMenu();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Input")) {
			drawInputVisualizerMenu();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void ryn::Debug::FpsTracker::update() {
	frameCount++;

	auto now = std::chrono::high_resolution_clock::now();
	int64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() - lastTime;

	if (elapsed >= UPDATE_INTERVAL) {
		// Calculate the average FPS for this interval
		averageFps = frameCount / static_cast<double>(elapsed) * 1000.0; // FPS = frames / seconds

		// Reset for the next interval
		lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		frameCount = 0;
	}
}

double ryn::Debug::FpsTracker::getAverageFps() const {
	return averageFps;
}

void ryn::Debug::drawStatsMenu() {
	// Gather information
	static FpsTracker fpsTracker;
	fpsTracker.update();

	const bgfx::Stats* stats = bgfx::getStats();
	double cpuTime = static_cast<double>(stats->cpuTimeFrame) / static_cast<double>(stats->cpuTimerFreq);
	double gpuTime = static_cast<double>(stats->gpuTimeEnd - stats->gpuTimeBegin) / static_cast<double>(stats->gpuTimerFreq);

	// Render menu
	ImGui::Text("Avarage FPS: %.1f", fpsTracker.getAverageFps());
	TooltipMarker(
		"These values are estimated and might be slightly off. "
		"However, they still represent an approximation of the performance. "
		"For more accurate performance indicators, refer to CPU/GPU time."
	);
	ImGui::Text("CPU Rendering time: %.7f", cpuTime);
	ImGui::Text("GPU Rendering time: %.7f", gpuTime);
	ImGui::Separator();
	ImGui::Text("GPU Memory: %i MB / %i MB", stats->gpuMemoryUsed / 1000000, stats->gpuMemoryMax / 1000000); 
	ImGui::Separator();
	ImGui::Text("Draw calls: %i", stats->numDraw);
}

void ryn::Debug::drawInputVisualizerMenu() {
	// Gather information
	static const ImVec4 inactiveColor = ImVec4(1, 1, 1, 0.2f);
	static const ImVec4 activeColor = ImVec4(1, 1, 1, 0.85f);

	// Render menu
	ImGui::SeparatorText("Keyboard");
	ImGui::TextColored(ryn::Input::regularKeyStates == 0			? inactiveColor : activeColor,	"Regular Key States: 0x%016llX", ryn::Input::regularKeyStates);
	ImGui::TextColored(ryn::Input::regularKeyStatesLastFrame == 0	? inactiveColor : activeColor,	"Regular Last Frame: 0x%016llX", ryn::Input::regularKeyStatesLastFrame);
	ImGui::TextColored(ryn::Input::specialKeyStates == 0			? inactiveColor : activeColor,	"Special Key States: 0x%016llX", ryn::Input::specialKeyStates);
	ImGui::TextColored(ryn::Input::specialKeyStatesLastFrame == 0	? inactiveColor : activeColor,	"Special Last Frame: 0x%016llX", ryn::Input::specialKeyStatesLastFrame);

	ImGui::SeparatorText("Mouse");
	ImGui::TextColored(ryn::Input::mouseX == 0 && ryn::Input::mouseY == 0				? inactiveColor : activeColor, "Mouse Position: (%.0f, %.0f)", ryn::Input::mouseX, ryn::Input::mouseY);
	ImGui::TextColored(ryn::Input::mouseScrollX == 0 && ryn::Input::mouseScrollY == 0	? inactiveColor : activeColor, "Mouse Scroll: (%.1f, %.1f)", ryn::Input::mouseScrollX, ryn::Input::mouseScrollY);
	ImGui::Spacing();
	ImGui::TextColored(ryn::Input::mouseButtonStates == 0			? inactiveColor : activeColor, "Mouse Button States: 0x%04X", ryn::Input::mouseButtonStates);
	ImGui::TextColored(ryn::Input::mouseButtonStatesLastFrame == 0	? inactiveColor : activeColor, "Mouse Button Last F: 0x%04X", ryn::Input::mouseButtonStatesLastFrame);
}

void ryn::Debug::TooltipMarker(const char* desc) {
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30);
		ImGui::TextWrapped(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
