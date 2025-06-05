#pragma once
#include "InputHandler.h"

Weave::Utilities::Event<>& Weave::Input::InputHandler::GetEvent(sf::Keyboard::Scancode key, Weave::Input::InputType type)
{
    return (type == InputType::Pressed) ? keyPressEvents[key] : keyReleaseEvents[key];
}

Weave::Utilities::Event<>& Weave::Input::InputHandler::GetEvent(sf::Mouse::Button button, Weave::Input::InputType type)
{
    return (type == InputType::Pressed) ? mousePressEvents[button] : mouseReleaseEvents[button];
}

Weave::Utilities::Event<Weave::Mathematics::Vector2<float>>& Weave::Input::InputHandler::GetMouseMoveEvent()
{
    return onMouseMove;
}

void Weave::Input::InputHandler::HandleEvent(const sf::Event event)
{
    if (const auto* mouseMoveEvent = event.getIf<sf::Event::MouseMoved>())
    {
        onMouseMove(Mathematics::Vector2(mouseMoveEvent->position));
    }
    else if (const sf::Event::KeyPressed* keyPressEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressEvents.contains(keyPressEvent->scancode))
            keyPressEvents[keyPressEvent->scancode]();
    }
    else if (const sf::Event::KeyReleased* keyReleaseEvent = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleaseEvents.contains(keyReleaseEvent->scancode))
            keyReleaseEvents[keyReleaseEvent->scancode]();
    }
    else if (const sf::Event::MouseButtonPressed* mousePressEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressEvents.contains(mousePressEvent->button))
            mousePressEvents[mousePressEvent->button]();
    }
    else if (const sf::Event::MouseButtonReleased* mouseReleaseEvent = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleaseEvents.contains(mouseReleaseEvent->button))
            mouseReleaseEvents[mouseReleaseEvent->button]();
    }
}