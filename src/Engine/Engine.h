#pragma once
#include "ECS.h"
#include "Renderer.h"
#include "Animator.h"
#include "Events.h"
#include "InputHandler.h"
#include "Time.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

namespace Weave
{
	class Engine
	{
	private:
		ECS::World world;

		sf::RenderWindow window;
		Graphics::Renderer renderer;
		Input::InputHandler inputHandler;

		const float fixedUpdateInterval = 0.02f;

	public:
		Engine(std::string windowName);
		~Engine();

		const ECS::SystemGroupID updateGroup;
		const ECS::SystemGroupID lateUpdateGroup;
		const ECS::SystemGroupID fixedUpdateGroup;
		const ECS::SystemGroupID lateFixedUpdateGroup;
		const ECS::SystemGroupID worldRenderGroup;
		const ECS::SystemGroupID uiRenderGroup;

		ECS::World& GetWorld();
		Graphics::Renderer& GetRenderer();
		Input::InputHandler& GetInputHandler();

		void Run();
	};
}