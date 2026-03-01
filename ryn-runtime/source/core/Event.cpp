module;
#include "Prelude.h"

module Ryntime.Core;

namespace ryn {
    IEvent::~IEvent()                     = default;
    IEventDispatcher::~IEventDispatcher() = default;
}