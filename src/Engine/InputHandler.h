#pragma once
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include "Events.h"
#include "Mathematics.h"

namespace Weave
{
    namespace Input
    {
        enum class InputType
        {
            Pressed,
            Released
        };

        class InputHandler 
        {
        private:
            std::unordered_map<sf::Keyboard::Scancode, Utilities::Event<>> keyPressEvents;
            std::unordered_map<sf::Keyboard::Scancode, Utilities::Event<>> keyReleaseEvents;
            std::unordered_map<sf::Mouse::Button, Utilities::Event<>> mousePressEvents;
            std::unordered_map<sf::Mouse::Button, Utilities::Event<>> mouseReleaseEvents;
            Utilities::Event<Mathematics::Vector2<float>> onMouseMove;

        public:
            Utilities::Event<>& GetEvent(sf::Keyboard::Scancode key, InputType type);
            Utilities::Event<>& GetEvent(sf::Mouse::Button button, InputType type);
            Utilities::Event<Weave::Mathematics::Vector2<float>>& GetMouseMoveEvent();

            void HandleEvent(const sf::Event event);
        };
    }
}