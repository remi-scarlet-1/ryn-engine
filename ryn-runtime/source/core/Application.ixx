module;
#include "Prelude.h"

export module Ryntime.Core:Application;
export import :Event;
export import :Service;
export import Ryntime.Core.Container;

export namespace ryn {
    class RYN_API IApplication {
    public:
        static void bootstrap(int argc, char** argv, Box<IApplication> appBox);

        IApplication();
        virtual ~IApplication() = default;

    protected:
        virtual void onInit()     = 0;
        virtual void onShutdown() = 0;

        [[nodiscard]] IServiceStack&   getServiceStack();
        [[nodiscard]] IServiceLocator& getServiceLocator();

    private:
        bool         m_Running = true;
        ServiceStack m_ServiceStack;

        void run(int argc, char** argv);
        void onEventIntercept(EventListener& listener);
    };
}