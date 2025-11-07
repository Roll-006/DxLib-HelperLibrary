#pragma once
#include <Vector/vector_3d.hpp>

/// @brief XYZŽ²
struct Axis
{
	VECTOR x_axis;
	VECTOR y_axis;
	VECTOR z_axis;
};

namespace axis
{
	[[nodiscard]] inline VECTOR GetWorldXAxis() { return VGet(1.0f, 0.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldYAxis() { return VGet(0.0f, 1.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldZAxis() { return VGet(0.0f, 0.0f, 1.0f); }
	[[nodiscard]] inline Axis   GetWorldAxis () { return Axis(GetWorldXAxis(), GetWorldYAxis(), GetWorldZAxis()); }

	inline void Draw(const Axis& axis, const VECTOR& begin_pos, const float length)
	{
		DrawLine3D(begin_pos, begin_pos + axis.x_axis * length, GetColor(UCHAR_MAX,   0,   0));
		DrawLine3D(begin_pos, begin_pos + axis.y_axis * length, GetColor(  0, UCHAR_MAX,   0));
		DrawLine3D(begin_pos, begin_pos + axis.z_axis * length, GetColor(  0,   0, UCHAR_MAX));
	};
}


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Axis& axis)
{
	data.at("x_axis").get_to(axis.x_axis);
	data.at("y_axis").get_to(axis.y_axis);
	data.at("z_axis").get_to(axis.z_axis);
}

inline void to_json(nlohmann::json& data, const Axis& axis)
{
	data = nlohmann::json
	{
		{ "x_axis",	axis.x_axis },
		{ "y_axis",	axis.y_axis },
		{ "z_axis",	axis.z_axis }
	};
}
#pragma endregion
