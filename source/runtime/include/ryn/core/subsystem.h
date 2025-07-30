#pragma once

#include "ryn/defines.h"

// TODO: document
namespace ryn {
	class RYN_API Subsystem {
	public:
		virtual ~Subsystem() = default;

		virtual void initialize() {}
		virtual void update() {}
		virtual void render() {}
		virtual void shutdown() {}
	};
}