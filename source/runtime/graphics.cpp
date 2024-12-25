#include "graphics.h"

#include <stdexcept>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

RYN_API void ryn::Graphics::initialize(Window window) {
    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    // Get window resolution
    int windowWidth, windowHeight;
    window.getResolution(&windowWidth, &windowHeight);

    // Initialize bgfx using the provided window as context
    bgfx::Init init;
    init.platformData.nwh = window.getNativeHandle();
    init.resolution.width = (uint32_t)windowWidth;
    init.resolution.height = (uint32_t)windowHeight;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init))
        throw std::exception("BGFX (rendering backend) failed while initializing");
}

RYN_API void ryn::Graphics::shutdown() {
    bgfx::shutdown();
}
