#pragma once
#include <cmath>
#include <concepts>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace Weave
{
	namespace Mathematics
	{
		template <typename T>
		concept Numeric = std::is_arithmetic_v<T>;

        template <typename T>
        concept FloatingPoint = std::is_floating_point_v<T>;

		const double pi = 3.1415926535897932384;
        const float infinity = std::numeric_limits<float>::infinity();

		template<Numeric T = float>
		struct Vector2
		{
		public:
			T x;
			T y;

            Vector2(T x, T y)
            {
                this->x = x;
                this->y = y;
            }

            Vector2()
            {
                this->x = (T)0;
                this->y = (T)0;
            }

            Vector2(const sf::Vector2<T>& in)
            {
                this->x = in.x;
                this->y = in.y;
            }

            template<Numeric CastType>
            operator Vector2<CastType>() const
            {
                return Vector2<CastType>(static_cast<CastType>(this->x), static_cast<CastType>(this->y));
            }

            template<Numeric CastType>
            operator sf::Vector2<CastType>() const
            {
                return sf::Vector2<CastType>(static_cast<CastType>(this->x), static_cast<CastType>(this->y));
            }

            float GetMagnitude()
            {
                return (float)std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2));
            }

            Vector2<float> GetUnitVector()
            {
                return *this / this->GetMagnitude();
            }

            Vector2<T> GetPerpendicularVector()
            {
                return Vector2<T>(y, -x);
            }

            template<Numeric ArgType>
            Vector2<T>& operator *=(ArgType operand)
            {
                this->x *= operand;
                this->y *= operand;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator *(ArgType operand)
            {
                return Vector2(static_cast<T>(this->x * operand), static_cast<T>(this->y * operand));
            }

            template<Numeric ArgType>
            Vector2<T>& operator *=(Vector2<ArgType> operand)
            {
                this->x *= operand.x;
                this->y *= operand.y;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator *(Vector2<ArgType> operand)
            {
                return Vector2(static_cast<T>(this->x * operand.x), static_cast<T>(this->y * operand.y));
            }

            template<Numeric ArgType>
            Vector2<T>& operator /=(ArgType operand)
            {
                this->x /= operand;
                this->y /= operand;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator /(ArgType operand)
            {
                return Vector2(static_cast<T>(this->x / operand), static_cast<T>(this->y / operand));
            }

            template<Numeric ArgType>
            Vector2<T>& operator /=(Vector2<ArgType> operand)
            {
                this->x /= operand.x;
                this->y /= operand.y;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator /(Vector2<ArgType> operand)
            {
                return Vector2(static_cast<T>(this->x / operand.x), static_cast<T>(this->y / operand.y));
            }

            template<Numeric ArgType>
            Vector2<T>& operator +=(Vector2<ArgType> operand)
            {
                this->x += operand.x;
                this->y += operand.y;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator +(Vector2<ArgType> operand)
            {
                return Vector2(static_cast<T>(this->x + operand.x), static_cast<T>(this->y + operand.y));
            }

            template<Numeric ArgType>
            Vector2<T>& operator -=(Vector2<ArgType> operand)
            {
                this->x -= operand.x;
                this->y -= operand.y;

                return *this;
            }

            template<Numeric ArgType>
            Vector2<T> operator -(Vector2<ArgType> operand)
            {
                return Vector2(static_cast<T>(this->x - operand.x), static_cast<T>(this->y - operand.y));
            }

            template<Numeric T>
            static float Angle(Vector2<T> pointA, Vector2<T> pointB)
            {
                Vector2<float> direction = pointB - pointA;
                return std::atan2(direction.x, direction.y);
            }

            template<Numeric T>
            static float Dot(Vector2<T> vectorA, Vector2<T> vectorB)
            {
                return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
            }
		};

        template<Numeric T>
        std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec)
        {
            return os << "(" << vec.x << ", " << vec.y << ")";
        }

        template<FloatingPoint ValueType>
        ValueType RadiansToDegrees(ValueType radians)
        {
            return radians * 180 / static_cast<ValueType>(pi);
        }

        template<FloatingPoint ValueType>
        ValueType DegreesToRadians(ValueType degrees)
        {
            return degrees * static_cast<ValueType>(pi) / 180;
        }

		template<Numeric ArgType, FloatingPoint TimeType>
        ArgType Lerp(ArgType a, ArgType b, TimeType t)
		{
			return static_cast<ArgType>(a * (static_cast<TimeType>(1) - t) + (b * t));
		}

        template<Numeric ArgType, FloatingPoint TimeType>
        Vector2<ArgType> Lerp(Vector2<ArgType> a, Vector2<ArgType> b, TimeType t)
        {
            return static_cast<Vector2<ArgType>>(a * (static_cast<TimeType>(1) - t) + (b * t));
        }

		template<Numeric ValueType>
		ValueType Clamp(ValueType value, ValueType min, ValueType max)
		{
			return std::max(min, std::min(value, max));
		}
	}
}