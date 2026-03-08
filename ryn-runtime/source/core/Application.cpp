module;
#include "Prelude.h"

module Ryntime.Core;
import Ryntime.Service;

RYN_SCOPE_FILE("Application");

namespace ryn {
    void IApplication::bootstrap(int argc, char** argv, Box<IApplication> appBox) {
        RYN_SCOPE_LOCAL("Entrypoint");
#if defined(RYN_ENABLE_LOGGING)
        Log::init();
#endif

        RYN_INFO("Bootstrap application...");
        RYN_VERIFY(appBox, "Provided app box is invalid");

        appBox->run(argc, argv);
        appBox.reset();

#if defined(RYN_ENABLE_LOGGING)
        Log::shutdown();
#endif
    }

    IApplication::IApplication() {
        m_ServiceStack.setEventInterceptor([this](auto& listener) { this->onEventIntercept(listener); });
    }

    IServiceStack&   IApplication::getServiceStack() { return m_ServiceStack; }
    IServiceLocator& IApplication::getServiceLocator() { return m_ServiceStack; }

    void IApplication::run(int, char**) {
        {
            Timer clock;
            RYN_INFO("Initialize...");
            onInit();
            RYN_INFO("Initialization completed in {0:.3f} second(s)", clock.elapsed());
        }

        RYN_INFO("Start main loop...");
        while (m_Running)
            m_ServiceStack.update();

        RYN_INFO("Shutdown...");
        onShutdown();
    }

    void IApplication::onEventIntercept(EventListener& listener) {
        listener.match<event::WindowClose>([this](event::WindowClose& event) {
            m_Running = false;
            event.consume();
        });
    }
}