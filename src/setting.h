#ifndef SETTING_H
#define SETTING_H


#include <iostream>

#define COORD_TRANSLATE(x, x_a, x_b, X_a, X_b) \
((((x) - (x_a)) * ((X_b) - (X_a)) / ((x_b) - (x_a))) + (X_a))

constexpr float PI = 3.14159265f;
constexpr float MAX_RAY_DIST = 60.0f;
constexpr float RAY_STEP = 0.025f; // Меньше шаг — выше точность стен!
constexpr float GRAVITY = 0.1f;

// Палитра от ближнего (яркого) к дальнему (темному)
//const std::string GRADIENT = "@$#*!=+-:. "; 

const std::string GRADIENT = "$@B8&WM#oahkbdpqwmZO0QLCJUYXzcvunxrjft|()1{}[]?-_+~<>i!lI;:,^.";

#endif // *SETTING_H
