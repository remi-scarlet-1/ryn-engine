#include "entrypoint.h"
#include "ryntime.h"

class MyGame : public ryn::Game {
public:
    void onStarted() override { }
    void onUpdate() override { }
    void onShutdown() override { }
};

ryn::Game* ryn::createGame() {
    return new MyGame();
}