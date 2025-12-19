#pragma once
#include <cmath>
#include <nlohmann/json.hpp>

template<typename ElemT>
struct Vector2D
{
    ElemT x;
    ElemT y;

    Vector2D  operator+() const { return *this; }
	Vector2D  operator-() const { return Vector2D{ -x, -y }; }

	template<typename T>
    Vector2D& operator= (const Vector2D<T>& v) { x  = static_cast<ElemT>(v.x);   y  = static_cast<ElemT>(v.y);   return *this; }

	template<typename T>
    Vector2D& operator+=(const Vector2D<T>& v) { x += static_cast<ElemT>(v.x);   y += static_cast<ElemT>(v.y);   return *this; }

	template<typename T>
    Vector2D& operator-=(const Vector2D<T>& v) { x -= static_cast<ElemT>(v.x);   y -= static_cast<ElemT>(v.y);   return *this; }

	template<typename T>
    Vector2D& operator*=(const Vector2D<T>& v) { x *= static_cast<ElemT>(v.x);   y *= static_cast<ElemT>(v.y);   return *this; }

	template<typename ScaleT>
	Vector2D& operator*=(const ScaleT scale)   { x *= static_cast<ElemT>(scale); y *= static_cast<ElemT>(scale); return *this; }
};

template<typename T, typename U>
inline auto operator+ (const Vector2D<T>& v1,	const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x + v2.x)>{ v1.x + v2.x, v1.y + v2.y }; }

template<typename T, typename U>
inline auto operator- (const Vector2D<T>& v1,	const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x - v2.x)>{ v1.x - v2.x, v1.y - v2.y }; }

template<typename T, typename U>
inline auto operator* (const Vector2D<T>& v1,	const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x * v2.x)>{ v1.x * v2.x, v1.y * v2.y }; }

template<typename VecT, typename ScaleT>
inline auto operator* (const Vector2D<VecT>& v, const ScaleT scale)			{ return Vector2D<VecT>{ static_cast<VecT>(v.x * scale), static_cast<VecT>(v.y * scale) }; }

template<typename VecT, typename ScaleT>
inline auto operator* (const ScaleT scale,		const Vector2D<VecT>& v)	{ return v * scale; }

template<typename T, typename U>
inline bool operator==(const Vector2D<T>& v1,	const Vector2D<U>& v2)		{ return v1.x == v2.x && v1.y == v2.y; }

template<typename T, typename U>
inline bool operator!=(const Vector2D<T>& v1,	const Vector2D<U>& v2)		{ return !(v1 == v2); }


namespace v2d
{
	template<typename T>
	[[nodiscard]] inline float GetSize(const Vector2D<T>& v) { return static_cast<float>(sqrt(v.x * v.x + v.y * v.y)); }

	template<typename T>
	[[nodiscard]] inline float GetSquareSize(const Vector2D<T>& v) { return v.x * v.x + v.y * v.y; }

	template<typename T>
	[[nodiscard]] inline Vector2D<T> GetZeroV() { return { 0, 0 }; }

	template<typename T>
	[[nodiscard]] inline Vector2D<T> GetNormalizedV(const Vector2D<T>& v)
	{
		float size = GetSize(v);
		return size != 0 ? Vector2D<T>(v.x / size, v.y / size) : v;
	}
	
	template<typename T>
	[[nodiscard]] inline float GetDot(const Vector2D<T>& v1, const Vector2D<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
}


#pragma region from / to JSON
template<typename T>
inline void from_json(const nlohmann::json& j_data, Vector2D<T>& vector)
{
	j_data.at("x").get_to(vector.x);
	j_data.at("y").get_to(vector.y);
}

template<typename T>
inline void to_json(nlohmann::json& j_data, const Vector2D<T>& vector)
{
	j_data = nlohmann::json
	{
		{ "x",	vector.x },
		{ "y",	vector.y }
	};
}
#pragma endregion
