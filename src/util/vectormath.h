#pragma once
#include <SFML/System/Vector2.hpp>

namespace VectorMath
{
	template <typename T>
	constexpr T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2){
		return v1.x * v2.x + v1.y * v2.y;
	}

	template <typename T>
	constexpr T length(const sf::Vector2<T>& self){
		return std::sqrt(dot(self,self));
	}

	template <typename T>
	constexpr T normalize(const T& self){
		return self / length(self);
	}

	template <typename R, typename T>
	constexpr R convert(const sf::Vector2<T>& source){
		return R(source.x, source.y);
	}
}
