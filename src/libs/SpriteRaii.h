#ifndef SPRITE_RAII_H
#define SPRITE_RAII_H

#include <graphx.h>

class SpriteRaii {
public:
    SpriteRaii(): m_sprite(nullptr) {}

    SpriteRaii(uint16_t width, uint8_t height)
    : m_sprite(gfx_MallocSprite(width, height)) {}

    ~SpriteRaii() { free(m_sprite); }

    SpriteRaii(SpriteRaii&& other) noexcept
    : m_sprite(other.m_sprite) {
        other.m_sprite = nullptr;
    }

    SpriteRaii& operator=(SpriteRaii&& other) noexcept {
        if (this != &other) {
            free(m_sprite);
            m_sprite = other.m_sprite;
            other.m_sprite = nullptr;
        }
        return *this;
    }

    gfx_sprite_t* get() const { return m_sprite; }

private:
    gfx_sprite_t* m_sprite;
};

#endif // SPRITE_RAII_H
