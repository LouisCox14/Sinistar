#pragma once
#include "Animator.h"

void Weave::Graphics::UpdateAnimations(ECS::World& world)
{
	for (auto [entity, animator, sprite] : world.GetView<Animator, Sprite>())
	{
		animator.time += Time::DeltaTime;

		if (animator.time < animator.frameTime) continue;

		animator.time -= animator.frameTime;
		animator.currentIndex++;

		if (animator.currentIndex == animator.frames.size())
		{
			if (animator.looping)
				animator.currentIndex = 0;
			else
				continue;
		}

		sprite = animator.spriteSheet->GetSprite(animator.frames[animator.currentIndex]);
	}
}