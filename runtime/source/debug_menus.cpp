#include "debug_menus.h"

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
	// TODO: Gather relevant data

	// Render menu
	ImGui::SetNextWindowSize(ImVec2(240, 180), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 180), ImGuiCond_FirstUseEver);

	ImGui::Begin("Input Debug", open, ImGuiWindowFlags_NoResize);

	ImGui::End();
}
