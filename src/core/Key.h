#pragma once

#include <cstddef>

class Key
{
public:
    Key();

    void update(bool isPressed);

    bool isPressed() const;
    bool wasPressed() const;
    bool wasReleased() const;

    size_t getHoldTime() const;

private:
    bool m_currentState;
    bool m_previousState;
    size_t m_holdTime;
};