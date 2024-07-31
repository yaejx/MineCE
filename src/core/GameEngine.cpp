#include "GameEngine.h"

#include "EngineComponents.h"

GameEngine::GameEngine()
    : m_running(true)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::run()
{
    init();

    while (m_running)
    {
        input();
        update();
        render();
    }
}

void GameEngine::init()
{
    m_assets.initialize();
}

void GameEngine::input()
{
    EngineComponents::InputHandler::updateInputs(m_keys);
}

void GameEngine::update()
{
    EngineComponents::InputHandler::checkForExit(m_keys, m_running);
    EngineComponents::MapController::moveMapView(m_assets.getTileData(), m_keys);
}

void GameEngine::render() const
{
    EngineComponents::Renderer::renderTilemap(m_assets.getTileData());
    gfx_SwapDraw();
}
