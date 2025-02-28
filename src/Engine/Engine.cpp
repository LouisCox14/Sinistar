#include "Engine.h"
#include <optional>

Weave::Engine::Engine(std::string windowName) : updateGroup(world.CreateSystemGroup()), renderer(Graphics::Renderer(&window, "Assets/"))
{
	window.create(sf::VideoMode({ 800, 600 }), windowName, sf::Style::Resize | sf::Style::Close, sf::State::Windowed);

    world.RegisterSystem(std::bind(&Weave::Graphics::Renderer::RenderSprites, renderer, std::placeholders::_1), updateGroup);
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

void Weave::Engine::Run()
{
    while (window.isOpen())
    {
        while (const std::optional windowEvent = window.pollEvent())
        {
            window.clear(sf::Color::Black);

            if (windowEvent->is<sf::Event::Closed>())
                window.close();

            world.CallSystemGroup(updateGroup);

            window.display();
        }
    }
}