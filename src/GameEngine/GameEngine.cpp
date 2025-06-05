#pragma once
#include "GameEngine.h"

Weave::GameEngine::GameEngine(std::string windowName) : Engine(), updateGroup(CreateSystemGroup()), fixedUpdateGroup(CreateSystemGroup()), worldRenderGroup(CreateSystemGroup()), uiRenderGroup(CreateSystemGroup()), renderer(Graphics::Renderer(&window, "Assets/"))
{
	window.create(sf::VideoMode({ 1360, 960 }), windowName, sf::Style::Close, sf::State::Windowed);
    renderer.TargetCamera({0, 0});

    RegisterSystem(worldRenderGroup, [renderer = &renderer](ECS::World& world) { renderer->RenderSprites(world); });
    RegisterSystem(updateGroup, &Weave::Graphics::UpdateAnimations, 10.0f);
}

Weave::GameEngine::~GameEngine() { }

Weave::Graphics::Renderer& Weave::GameEngine::GetRenderer()
{
    return renderer;
}

Weave::Input::InputHandler& Weave::GameEngine::GetInputHandler()
{
    return inputHandler;
}

void Weave::GameEngine::Run()
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

        CallSystemGroup(updateGroup);

        CallSystemGroup(worldRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::UI);
        CallSystemGroup(uiRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::World);

        Time::DeltaTime = fixedUpdateInterval;

        while (timeSinceFixedUpdate > fixedUpdateInterval)
        {
            CallSystemGroup(fixedUpdateGroup);

            timeSinceFixedUpdate -= fixedUpdateInterval;
        }

        window.display();
    }
}