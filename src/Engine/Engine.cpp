#include "Engine.h"
#include <optional>

Weave::Engine::Engine(std::string windowName) : updateGroup(world.CreateSystemGroup()), renderer(Graphics::Renderer(&window, "Assets/"))
{
	window.create(sf::VideoMode({ 1360, 960 }), windowName, sf::Style::Close, sf::State::Windowed);
    renderer.TargetCamera({0, 0});

    world.GetSystemGroup(updateGroup).Subscribe(renderer, &Weave::Graphics::Renderer::RenderSprites);
}

Weave::Engine::~Engine() { }

Weave::ECS::World& Weave::Engine::GetWorld()
{
    return world;
}

Weave::Graphics::Renderer& Weave::Engine::GetRenderer()
{
    return renderer;
}

Weave::Input::InputHandler& Weave::Engine::GetInputHandler()
{
    return inputHandler;
}

void Weave::Engine::Run()
{
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);

        while (const std::optional windowEvent = window.pollEvent())
        {
            if (windowEvent->is<sf::Event::Closed>())
            {
                window.close();
                return;
            }

            inputHandler.HandleEvent(windowEvent.value());
        }

        world.CallSystemGroup(updateGroup);

        window.display();
    }
}