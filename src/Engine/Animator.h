#pragma once
#include "ECS.h"
#include "Renderer.h"
#include "Time.h"

namespace Weave
{
	namespace Graphics
	{
		struct Animator
		{
			SpriteSheet* spriteSheet;
			std::vector<uint16_t> frames;
			uint16_t currentIndex = 0;

			float frameTime;
			float time = 0.0f;

			bool looping = true;
			bool ended = false;
		};

		void UpdateAnimations(ECS::World& world);
	}
}