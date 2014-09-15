#pragma once
#include <SFML/System/Vector2.hpp>

namespace VectorMath
{
	template <typename T>
	constexpr inline T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2){
		return v1.x * v2.x + v1.y * v2.y;
	}

	template <typename T>
	constexpr inline float length(const sf::Vector2<T>& self){
		return std::sqrt((float)dot(self,self));
	}

	template <typename T>
	constexpr inline T normalize(const T& self){
		return self / std::sqrt(dot(self,self));
	}

	template <typename R, typename T>
	constexpr inline R convert(const sf::Vector2<T>& source){
		return R(source.x, source.y);
	}
}
