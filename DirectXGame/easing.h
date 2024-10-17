#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>

namespace Easing {

// Ease Out Quad
float easeOutQuad(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }

// Ease In Quad
float easeInQuad(float t) { return t * t; }

// Ease In Out Quart
float easeInOutQuart(float t) { return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - static_cast<float>(pow(-2.0f * t + 2.0f, 4.0f)) / 2.0f; }

// Ease In Quart
float easeInQuart(float t) { return t * t * t * t; }

// Ease Out Quart
float easeOutQuart(float t) { return 1.0f - static_cast<float>(pow(1.0f - t, 4.0f)); }

// Ease In Out Quad
float easeInOutQuad(float t) { return t < 0.5f ? 2.0f * t * t : 1.0f - static_cast<float>(pow(-2.0f * t + 2.0f, 2.0f)) / 2.0f; }

// Ease In Cubic
float easeInCubic(float t) { return t * t * t; }

// Ease Out Cubic
float easeOutCubic(float t) { return 1.0f - static_cast<float>(pow(1.0f - t, 3.0f)); }

// Ease In Out Cubic
float easeInOutCubic(float t) { return t < 0.5f ? 4.0f * t * t * t : 1.0f - static_cast<float>(pow(-2.0f * t + 2.0f, 3.0f)) / 2.0f; }

// Ease In Quint
float easeInQuint(float t) { return t * t * t * t * t; }

// Ease Out Quint
float easeOutQuint(float t) { return 1.0f - static_cast<float>(pow(1.0f - t, 5.0f)); }

// Ease In Out Quint
float easeInOutQuint(float t) { return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - static_cast<float>(pow(-2.0f * t + 2.0f, 5.0f)) / 2.0f; }

// Ease In Sine
float easeInSine(float t) { return 1.0f - static_cast<float>(cos(static_cast<float>(t) * M_PI / 2.0f)); }

// Ease Out Sine
float easeOutSine(float t) { return static_cast<float>(sin(static_cast<float>(t) * M_PI / 2.0f)); }

// Ease In Out Sine
float easeInOutSine(float t) { return -(static_cast<float>(cos(M_PI * t)) - 1.0f) / 2.0f; }

// Ease In Expo
float easeInExpo(float t) { return t == 0.0f ? 0.0f : static_cast<float>(pow(2.0f, 10.0f * t - 10.0f)); }

// Ease Out Expo
float easeOutExpo(float t) { return t == 1.0f ? 1.0f : 1.0f - static_cast<float>(pow(2.0f, -10.0f * t)); }

// Ease In Out Expo
float easeInOutExpo(float t) {
	return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : t < 0.5f ? static_cast<float>(pow(2.0f, 20.0f * t - 10.0f)) / 2.0f : (2.0f - static_cast<float>(pow(2.0f, -20.0f * t + 10.0f))) / 2.0f;
}

// Ease In Circ
float easeInCirc(float t) { return 1.0f - static_cast<float>(sqrt(1.0f - pow(t, 2.0f))); }

// Ease Out Circ
float easeOutCirc(float t) { return static_cast<float>(sqrt(1.0f - pow(t - 1.0f, 2.0f))); }

// Ease In Out Circ
float easeInOutCirc(float t) { return t < 0.5f ? (1.0f - static_cast<float>(sqrt(1.0f - pow(2.0f * t, 2.0f)))) / 2.0f : (static_cast<float>(sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f))) + 1.0f) / 2.0f; }

// Ease In Elastic
float easeInElastic(float t) {
	const float c4 = static_cast<float>((2.0f * M_PI) / 3.0f);
	return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : -static_cast<float>(pow(2.0f, 10.0f * t - 10.0f)) * static_cast<float>(sin((t * 10.0f - 10.75f) * c4));
}

// Ease Out Elastic
float easeOutElastic(float t) {
	const float c4 = static_cast<float>((2.0f * M_PI) / 3.0f);
	return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : static_cast<float>(pow(2.0f, -10.0f * t)) * static_cast<float>(sin((t * 10.0f - 0.75f) * c4)) + 1.0f;
}

// Ease In Out Elastic
float easeInOutElastic(float t) {
	const float c5 = static_cast<float>((2.0f * M_PI) / 4.5f);
	return t == 0.0f   ? 0.0f
	       : t == 1.0f ? 1.0f
	       : t < 0.5f  ? -(static_cast<float>(pow(2.0f, 20.0f * t - 10.0f)) * static_cast<float>(sin((20.0f * t - 11.125f) * c5))) / 2.0f
	                   : (static_cast<float>(pow(2.0f, -20.0f * t + 10.0f)) * static_cast<float>(sin((20.0f * t - 11.125f) * c5))) / 2.0f + 1.0f;
}

} // namespace Easing
