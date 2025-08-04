#include "ryn/core/game.h"
#include "ryn/core/memory.h"
#include "ryn/core/subsystem.h"
#include <vector>
#include <unordered_map>

struct ryn::Game::Impl {
	bool gameHasStarted = false;
	bool isRunning = false;
	std::vector<UniquePtr<Subsystem>> subsystems;
	std::unordered_map<TypeID, size_t> subsystemTypetoIndex;
};

ryn::Game::Game() : m_impl(new Impl()) {}
ryn::Game::~Game() { delete m_impl; }

void ryn::Game::internal_createSubsystem(TypeID typeID, Subsystem* subsystem) {
	if (m_impl->subsystemTypetoIndex.contains(typeID))
		return;

	m_impl->subsystems.emplace_back(std::move(UniquePtr<Subsystem>(subsystem)));
	m_impl->subsystemTypetoIndex[typeID] = m_impl->subsystems.size() - 1;

	if (m_impl->gameHasStarted)
		subsystem->initialize();

	subsystem->m_game = this;
}

bool ryn::Game::internal_deleteSubsystem(TypeID typeID) {
	if (!m_impl->subsystemTypetoIndex.contains(typeID))
		return false;

	auto index = m_impl->subsystemTypetoIndex[typeID];
	auto subsystem = m_impl->subsystems[index].get();

	if (m_impl->gameHasStarted)
		subsystem->shutdown();

	m_impl->subsystems.erase(m_impl->subsystems.begin() + index);
	m_impl->subsystemTypetoIndex.erase(typeID);

	for (auto& [id, idx] : m_impl->subsystemTypetoIndex)
		if (idx > index)
			--idx;

	return true;
}

ryn::Subsystem* ryn::Game::internal_getSubsystem(TypeID typeID) {
	if (!m_impl->subsystemTypetoIndex.contains(typeID))
		return nullptr;

	auto index = m_impl->subsystemTypetoIndex[typeID];
	auto subsystem = m_impl->subsystems[index].get();
	return subsystem;
}

bool ryn::Game::isRunning() {
	return m_impl->isRunning;
}

void ryn::Game::terminate() {
	m_impl->isRunning = false;
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