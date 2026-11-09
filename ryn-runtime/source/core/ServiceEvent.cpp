module;
#include "Prelude.h"

module Ryntime.Core;
import Ryntime.Logger;

namespace ryn {
    IEvent::~IEvent()                     = default;
    IEventDispatcher::~IEventDispatcher() = default;
}