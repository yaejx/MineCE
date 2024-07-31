#pragma once

#include <keypadc.h> // kb_lkey_t
#include <graphx.h> // gfx_tilemap_t

#include "AssetManager.h" // AssetManager
#include "Key.h" // Key
#include "libs/map.h" // map

// comments above because I keep trying to delete the #include's lol

class GameEngine
{
public:
    using InputManager = map<kb_lkey_t, Key>;

    GameEngine();
    ~GameEngine();
 
    void run();

private:
    void init();
    
    void input();
    void update();
    void render() const;

    AssetManager m_assets;
    InputManager m_keys;
    bool m_running;
};
