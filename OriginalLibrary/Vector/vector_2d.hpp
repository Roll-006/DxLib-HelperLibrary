#pragma once
#include <cmath>
#include <nlohmann/json.hpp>

#include "vector_concepts.hpp"

template<typename ElemT>
struct Vector2D
{
    ElemT x;
    ElemT y;

    Vector2D  operator+() const             { return *this; }
	Vector2D  operator-() const				{ return { -x, -y }; }

    Vector2D& operator= (const Vector2D& v) { x = v.x;  y = v.y;  return *this; }

    Vector2D& operator+=(const Vector2D& v) { x += v.x; y += v.y; return *this; }
    Vector2D& operator-=(const Vector2D& v) { x -= v.x; y -= v.y; return *this; }
    Vector2D& operator*=(const Vector2D& v) { x *= v.x; y *= v.y; return *this; }

    template<typename ScaleT>
    Vector2D& operator*=(const ScaleT scale){ return Vector2D(x * scale, y * scale); }
};

inline auto operator+ (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2){ return Vector2D(v1.x + v2.x, v1.y + v2.y); }
inline auto operator- (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2){ return Vector2D(v1.x - v2.x, v1.y - v2.y); }
inline auto operator* (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2){ return Vector2D(v1.x * v2.x, v1.y * v2.y); }

template<typename ScaleT>
inline auto operator* (const v2d_concepts::ValidT auto& v,  const ScaleT scale)                 { return Vector2D(v.x * scale, v.y * scale); }
template<typename ScaleT>
inline auto operator* (const ScaleT scale,                  const v2d_concepts::ValidT auto& v) { return v * scale; }

inline bool operator==(const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2){ return v1.x == v2.x && v1.y == v2.y; }
inline bool operator!=(const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2){ return !(v1 == v2); }


namespace v2d
{
	template<typename CastT>
	[[nodiscard]] inline Vector2D<CastT> ConvertVecType(const v2d_concepts::ValidT auto& v)
	{
		return Vector2D<CastT>(v.x, v.y);
	}

	[[nodiscard]] inline float GetSize(const v2d_concepts::ValidT auto& v)
	{
		return static_cast<float>(sqrt(v.x * v.x + v.y * v.y));
	}
	[[nodiscard]] inline float GetSquareSize(const v2d_concepts::ValidT auto& v)
	{
		return GetSize(v) * GetSize(v);
	}

	template<v2d_concepts::ValidT VecT>
	[[nodiscard]] inline VecT GetZeroV()
	{
		return VecT(0, 0);
	}

	template<v2d_concepts::ValidT VecT>
	[[nodiscard]] inline VecT GetNormalizedV(const VecT& v)
	{
		float size = GetSize(v);
		return size != 0 ? VecT(v.x / size, v.y / size) : v;
	}
	
	template<v2d_concepts::ValidT VecT>
	[[nodiscard]] inline float GetDot(const VecT& v1, const VecT& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
}


#pragma region from / to JSON
template<v2d_concepts::ValidT VecT>
inline void from_json(const nlohmann::json& data, VecT& v)
{
	data.at("x").get_to(v.x);
	data.at("y").get_to(v.y);
}

template<v2d_concepts::ValidT VecT>
inline void to_json(nlohmann::json& data, const VecT& v)
{
	data = nlohmann::json
	{
		{ "x", v.x },
		{ "y", v.y }
	};
}
#pragma endregion
