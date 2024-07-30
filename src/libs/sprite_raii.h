#include <graphx.h>

class sprite_raii {
public:
    sprite_raii(): m_sprite(nullptr) {}

    sprite_raii(uint16_t width, uint8_t height)
    : m_sprite(gfx_MallocSprite(width, height)) {}

    ~sprite_raii() { free(m_sprite); }

    sprite_raii(sprite_raii&& other) noexcept
    : m_sprite(other.m_sprite) {
        other.m_sprite = nullptr;
    }

    sprite_raii& operator=(sprite_raii&& other) noexcept {
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