#include <cmath>

namespace MathPlus{
	constexpr double PI = 3.1415927;

	/// Interpolation between the given values.
	/// Pre: fraction must be between 0 and 1.
	template <class T>
	inline T interpolate(T value0, T value1, double fraction){
		//Smooth fraction with cosine
		fraction = (1 - std::cos(fraction * PI)) / 2.0;
		return static_cast<T>(value0 * (1.0 - fraction) + value1 * (fraction));
	}
};
