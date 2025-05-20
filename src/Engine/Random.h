#pragma once
#include <random>
#include "Mathematics.h"

namespace Weave
{
	class Random
	{
	private:
		static std::default_random_engine engine;

	public:
		template<Mathematics::FloatingPoint T>
		static T GenerateRandomInBounds(T low, T high);

		template<Mathematics::Integral T>
		static T GenerateRandomInBounds(T low, T high);
	};

	template<Mathematics::FloatingPoint T>
	inline T Random::GenerateRandomInBounds(T low, T high)
	{
		std::uniform_real_distribution<T> distribution(low, high);
		return distribution(engine);
	}

	template<Mathematics::Integral T>
	inline T Random::GenerateRandomInBounds(T low, T high)
	{
		std::uniform_int_distribution<T> distribution(low, high);
		return distribution(engine);
	}
}