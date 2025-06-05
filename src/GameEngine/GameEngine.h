#pragma once
#include "Engine.h"
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
	class GameEngine : public ECS::Engine
	{
	private:
		sf::RenderWindow window;
		Graphics::Renderer renderer;
		Input::InputHandler inputHandler;

		const float fixedUpdateInterval = 0.02f;

	public:
		GameEngine(std::string windowName);
		~GameEngine();

		const ECS::SystemGroupID updateGroup;
		const ECS::SystemGroupID fixedUpdateGroup;
		const ECS::SystemGroupID worldRenderGroup;
		const ECS::SystemGroupID uiRenderGroup;

		Graphics::Renderer& GetRenderer();
		Input::InputHandler& GetInputHandler();

		void Run();
	};
}