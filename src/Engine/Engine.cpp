#pragma once
#include "Engine.h"

Weave::Engine::Engine(std::string windowName) : updateGroup(CreateSystemGroup()), lateUpdateGroup(CreateSystemGroup()), fixedUpdateGroup(CreateSystemGroup()), lateFixedUpdateGroup(CreateSystemGroup()), worldRenderGroup(CreateSystemGroup()), uiRenderGroup(CreateSystemGroup()), renderer(Graphics::Renderer(&window, "Assets/"))
{
	window.create(sf::VideoMode({ 1360, 960 }), windowName, sf::Style::Close, sf::State::Windowed);
    renderer.TargetCamera({0, 0});

    GetSystemGroup(worldRenderGroup).Subscribe(renderer, &Weave::Graphics::Renderer::RenderSprites);
    GetSystemGroup(lateUpdateGroup).Subscribe(&Weave::Graphics::UpdateAnimations);
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

Weave::SystemGroupID Weave::Engine::CreateSystemGroup()
{
    if (!availableSystemGroupIDs.empty())
        return availableSystemGroupIDs.extract(availableSystemGroupIDs.begin()).value();

    systemGroups.resize(nextSystemGroupID + 1);
    return nextSystemGroupID++;
}

void Weave::Engine::RetireSystemGroup(Weave::SystemGroupID systemGroup)
{
    if (!IsSystemGroupRegistered(systemGroup)) return;

    systemGroups[systemGroup].clear();
    availableSystemGroupIDs.insert(systemGroup);
}

bool Weave::Engine::IsSystemGroupRegistered(Weave::SystemGroupID systemGroup)
{
    if (systemGroup > nextSystemGroupID) return false;
    if (availableSystemGroupIDs.find(systemGroup) != availableSystemGroupIDs.end()) return false;

    return true;
}

Weave::SystemGroup& Weave::Engine::GetSystemGroup(SystemGroupID groupID)
{
    if (!IsSystemGroupRegistered(groupID)) throw std::logic_error("System is not registered.");

    return systemGroups[groupID];
}

void Weave::Engine::CallSystemGroup(Weave::SystemGroupID groupID)
{
    if (!IsSystemGroupRegistered(groupID)) return;

    systemGroups[groupID](world);
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

        CallSystemGroup(updateGroup);
        CallSystemGroup(lateUpdateGroup);

        CallSystemGroup(worldRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::UI);
        CallSystemGroup(uiRenderGroup);
        renderer.SetRenderMode(Graphics::RenderMode::World);

        Time::DeltaTime = fixedUpdateInterval;

        while (timeSinceFixedUpdate > fixedUpdateInterval)
        {
            CallSystemGroup(fixedUpdateGroup);
            CallSystemGroup(lateFixedUpdateGroup);

            timeSinceFixedUpdate -= fixedUpdateInterval;
        }

        window.display();
    }
}