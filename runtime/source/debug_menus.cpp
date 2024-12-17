#include "debug_menus.h"

#include "input.h"

#include <imgui/imgui.h>
#include <fmt/core.h>
#include <bgfx/bgfx.h>

void ryn::Debug::showStatisticsMenu(bool* open) {
	// Gather relevant data
	const bgfx::Stats* stats = bgfx::getStats();
	double cpuTime = static_cast<double>(stats->cpuTimeFrame) / static_cast<double>(stats->cpuTimerFreq);
	double gpuTime = static_cast<double>(stats->gpuTimeEnd - stats->gpuTimeBegin) / static_cast<double>(stats->gpuTimerFreq);

	// Render menu
	ImGui::SetNextWindowSize(ImVec2(240, 180), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

	ImGui::Begin("Statistics", open, ImGuiWindowFlags_NoResize);

	ImGui::Text(fmt::format("FPS {} ", 1 / cpuTime).c_str());
	ImGui::Text(fmt::format("Cpu time: {:.7f} ", cpuTime).c_str());
	ImGui::Text(fmt::format("Gpu time: {:.7f} ", gpuTime).c_str());
	ImGui::Text(fmt::format("Gpu mem: {} MB / {} MB ", stats->gpuMemoryUsed / 1000000, stats->gpuMemoryMax / 1000000).c_str());
	ImGui::Spacing();
	ImGui::SeparatorText("Graphics");
	ImGui::Text(fmt::format("NumberDraw calls: {} ", stats->numDraw).c_str());

	ImGui::End();
}

void ryn::Debug::showInputVisualizerMenu(bool* open) {
	// Render menu
	ImGui::SetNextWindowSize(ImVec2(320, 180), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 180), ImGuiCond_FirstUseEver);

	ImGui::Begin("Input Debug", open, ImGuiWindowFlags_NoResize);

	ImGui::Text("Regular Key States: 0x%016llX", ryn::Input::regularKeyStates);
	ImGui::Text("Regular Last Frame: 0x%016llX", ryn::Input::regularKeyStatesLastFrame);
	ImGui::Text("Special Key States: 0x%016llX", ryn::Input::specialKeyStates);
	ImGui::Text("Special Last Frame: 0x%016llX", ryn::Input::specialKeyStatesLastFrame);
	ImGui::Spacing();
	ImGui::Text("Mouse Button States: 0x%04X", ryn::Input::mouseButtonStates);
	ImGui::Text("Mouse Button Last F: 0x%04X", ryn::Input::mouseButtonStatesLastFrame);
	ImGui::Spacing();
	ImGui::Text("Mouse Position: (%.1f, %.1f)", ryn::Input::mouseX, ryn::Input::mouseY);
	ImGui::Text("Mouse Scroll: (%.1f, %.1f)", ryn::Input::mouseScrollX, ryn::Input::mouseScrollY);

	ImGui::End();
}
