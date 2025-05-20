#pragma once
#include "Engine.h"

Weave::Engine::Engine(std::string windowName) : updateGroup(world.CreateSystemGroup()), lateUpdateGroup(world.CreateSystemGroup()), fixedUpdateGroup(world.CreateSystemGroup()), lateFixedUpdateGroup(world.CreateSystemGroup()), worldRenderGroup(world.CreateSystemGroup()), uiRenderGroup(world.CreateSystemGroup()), renderer(Graphics::Renderer(&window, "Assets/"))
{
	window.create(sf::VideoMode({ 1360, 960 }), windowName, sf::Style::Close, sf::State::Windowed);
    renderer.TargetCamera({0, 0});

    world.GetSystemGroup(worldRenderGroup).Subscribe(renderer, &Weave::Graphics::Renderer::RenderSprites);
    world.GetSystemGroup(lateUpdateGroup).Subscribe(&Weave::Graphics::UpdateAnimations);
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
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    double timeSinceFixedUpdate = 0.0f;

    while (window.isOpen())
    {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        Time::DeltaTime = std::chrono::duration<double>(currentTime - startTime).count();
        timeSinceFixedUpdate += Time::DeltaTime;

        startTime = currentTime;

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
        world.CallSystemGroup(lateUpdateGroup);

        world.CallSystemGroup(worldRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::UI);
        world.CallSystemGroup(uiRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::World);

        Time::DeltaTime = fixedUpdateInterval;

        while (timeSinceFixedUpdate > fixedUpdateInterval)
        {
            world.CallSystemGroup(fixedUpdateGroup);
            world.CallSystemGroup(lateFixedUpdateGroup);

            timeSinceFixedUpdate -= fixedUpdateInterval;
        }

        window.display();
    }
}