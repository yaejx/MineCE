#include "Game.h"

#include <cstdlib> // srand()
#include <graphx.h>
#include <sys/rtc.h> // rtc_Time()

#include "GameEngine.h"

Game::Game()
{
    srand(rtc_Time());

    gfx_Begin();
    gfx_SwapDraw();
}

Game::~Game()
{
    gfx_End();
}

void Game::start() const
{
    GameEngine engine;
    engine.run();
}