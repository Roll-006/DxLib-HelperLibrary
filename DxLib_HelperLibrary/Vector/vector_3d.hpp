#pragma once
#include <nlohmann/json.hpp>
#include <DxLib.h>

inline VECTOR operator+ (const VECTOR& v)	{ return v; }
inline VECTOR operator- (const VECTOR& v)	{ return { -v.x, -v.y, -v.z }; }

inline VECTOR operator+ (const VECTOR& v1, const VECTOR& v2)	{ return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
inline VECTOR operator- (const VECTOR& v1, const VECTOR& v2)	{ return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
inline VECTOR operator* (const VECTOR& v1, const VECTOR& v2)	{ return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z }; }

template<typename ScaleT>
inline VECTOR operator* (const VECTOR& v, const ScaleT scale)	{ return { v.x * scale, v.y * scale, v.z * scale }; }
template<typename ScaleT>
inline VECTOR operator* (const ScaleT scale, const VECTOR& v)	{ return v * scale; }

inline auto operator+=(VECTOR& v1, const VECTOR& v2)	{ v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
inline auto operator-=(VECTOR& v1, const VECTOR& v2)	{ v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
inline auto operator*=(VECTOR& v1, const VECTOR& v2)	{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }

template<typename ScaleT>
inline auto operator*=(VECTOR& v, const ScaleT scale)	{ v.x *= scale; v.y *= scale; v.z *= scale; return v; }

inline bool operator==(const VECTOR& v1, const VECTOR& v2){ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
inline bool operator!=(const VECTOR& v1, const VECTOR& v2){ return !(v1 == v2); }


namespace v3d
{
    [[nodiscard]] inline VECTOR GetZeroV()                      { return VGet(0.0f, 0.0f, 0.0f); }
    [[nodiscard]] inline VECTOR GetNormalizedV(const VECTOR& v) { return VSize(v) != 0.0f ? VNorm(v) : v; }
}


#pragma region from / to JSON
namespace DxLib
{
	inline void from_json(const nlohmann::json& j_data, VECTOR& vector)
	{
		j_data.at("x").get_to(vector.x);
		j_data.at("y").get_to(vector.y);
		j_data.at("z").get_to(vector.z);
	}

	inline void to_json(nlohmann::json& j_data, const VECTOR& vector)
	{
		j_data = nlohmann::json
		{
			{ "x",	vector.x },
			{ "y",	vector.y },
			{ "z",	vector.z }
		};
	}
}
#pragma endregion
