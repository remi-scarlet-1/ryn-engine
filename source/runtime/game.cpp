#include "ryn/core/game.h"
#include "ryn/core/memory.h"
#include "ryn/core/subsystem.h"
#include <vector>
#include <unordered_map>

struct ryn::Game::Impl {
	bool gameHasStarted = false;
	bool isRunning = false;
	std::vector<memory::UniquePtr<Subsystem>> subsystems;
	std::unordered_map<meta::TypeID, size_t> subsystemTypetoIndex;
};

ryn::Game::Game() : m_impl(new Impl()) {}
ryn::Game::~Game() { delete m_impl; }

bool ryn::Game::isRunning() { return m_impl->isRunning; }
void ryn::Game::terminate() { m_impl->isRunning = false; }

void ryn::Game::internal_createSubsystem(meta::TypeID typeID, Subsystem* subsystem) {
	// Ensure there isn't already a singleton for the subsystem type
	if (m_impl->subsystemTypetoIndex.contains(typeID))
		return;

	m_impl->subsystems.emplace_back(std::move(memory::UniquePtr<Subsystem>(subsystem)));
	m_impl->subsystemTypetoIndex[typeID] = m_impl->subsystems.size() - 1;
	subsystem->m_game = this;

	// Immediately initialize subsystem if game is already running
	if (m_impl->gameHasStarted)
		subsystem->initialize();
}

bool ryn::Game::internal_deleteSubsystem(meta::TypeID typeID) {
	// Abort deletion if no singleton exists for subsystem type
	if (!m_impl->subsystemTypetoIndex.contains(typeID))
		return false;

	auto index = m_impl->subsystemTypetoIndex[typeID];
	auto subsystem = m_impl->subsystems[index].get();
	if (m_impl->gameHasStarted)
		subsystem->shutdown();

	// Erase subsystem entries
	m_impl->subsystems.erase(m_impl->subsystems.begin() + index);
	m_impl->subsystemTypetoIndex.erase(typeID);

	// Shift subsystem indices to the left to account for offset through erase() operation
	for (auto& [id, idx] : m_impl->subsystemTypetoIndex)
		if (idx > index)
			--idx;

	return true;
}

ryn::Subsystem* ryn::Game::internal_getSubsystem(meta::TypeID typeID) {
	// Return nullptr if requested subsystem type has no corresponding singleton
	if (!m_impl->subsystemTypetoIndex.contains(typeID))
		return nullptr;

	auto index = m_impl->subsystemTypetoIndex[typeID];
	return m_impl->subsystems[index].get();
}


void ryn::Game::initialize() {
	onBeforeInit();
	for (auto& subsystem : m_impl->subsystems)
		subsystem->initialize();
	onAfterInit();

	m_impl->gameHasStarted = true;
	m_impl->isRunning = true;
}

void ryn::Game::update() {
	onBeforeUpdate();
	for (auto& subsystem : m_impl->subsystems)
		subsystem->update();
	onAfterUpdate();
}

void ryn::Game::render() {
	onBeforeRender();
	for (auto& subsystem : m_impl->subsystems)
		subsystem->render();
	onAfterRender();
}

void ryn::Game::shutdown() {
	onBeforeShutdown();
	for (auto& subsystem : m_impl->subsystems)
		subsystem->shutdown();
	onAfterShutdown();
}