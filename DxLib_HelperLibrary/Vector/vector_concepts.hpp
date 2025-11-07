#pragma once
#include <concepts>
#include <DxLib.h>

template<typename ElemT> struct Vector2D;

namespace v2d_concepts
{
    template<typename T>
    concept ValidT = std::same_as<T, Vector2D<int>>
                  || std::same_as<T, Vector2D<long>>
                  || std::same_as<T, Vector2D<long long>>
                  || std::same_as<T, Vector2D<float>>
                  || std::same_as<T, Vector2D<double>>
                  || std::same_as<T, Vector2D<long double>>;
}
