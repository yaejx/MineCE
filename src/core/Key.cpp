#include "Key.h"

Key::Key()
    : m_currentState(false)
    , m_previousState(false)
    , m_holdTime(0)
{
}

void Key::update(bool isPressed)
{
    m_previousState = m_currentState;
    m_currentState = isPressed;

    if (m_currentState)
    {
        m_holdTime++;
    }
    else
    {
        m_holdTime = 0;
    }
}

bool Key::isPressed() const
{
    return m_currentState;
}

bool Key::wasPressed() const
{
    return m_currentState && !m_previousState;
}

bool Key::wasReleased() const
{
    return !m_currentState && m_previousState;
}

size_t Key::getHoldTime() const
{
    return m_holdTime;
}