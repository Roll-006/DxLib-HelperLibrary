#pragma once
#include <nlohmann/json.hpp>
#include <DxLib.h>

inline MATRIX operator+ (const MATRIX& mat1, const MATRIX& mat2)	{ return MAdd (mat1, mat2); }
inline MATRIX operator* (const MATRIX& mat1, const MATRIX& mat2)	{ return MMult(mat1, mat2); }

inline MATRIX operator* (const MATRIX& mat, const float scale)		{ return MScale(mat, scale); }
inline MATRIX operator* (const float scale, const MATRIX& mat)		{ return MScale(mat, scale); }

inline MATRIX operator+=(MATRIX& mat1, const MATRIX& mat2)			{ mat1 = mat1 + mat2; return mat1; }
inline MATRIX operator*=(MATRIX& mat1, const MATRIX& mat2)			{ mat1 = mat1 * mat2; return mat1; }

template<typename ScaleT>
inline MATRIX operator*=(MATRIX& mat, const ScaleT scale)			{ mat = mat * scale; return mat; }

inline bool operator==(const MATRIX& mat1, const MATRIX& mat2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (mat1.m[i][j] != mat2.m[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
inline bool operator!=(const MATRIX& mat1, const MATRIX& mat2) { return !(mat1 == mat2); }

namespace matrix
{
	inline void SetPos(MATRIX& mat, const VECTOR& pos)
	{
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
	}

	inline void Draw(const int x, const int y, const MATRIX& mat)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				int pos_x = x + j * 200;
				int pos_y = y + i * 20;
				DrawFormatString(pos_x, pos_y, 0xffffff, "%f", mat.m[i][j]);
			}
		}
	}
}


#pragma region from / to JSON
namespace DxLib
{
	inline void from_json(const nlohmann::json& data, MATRIX& mat)
	{
		auto m = data.get<std::array<std::array<float, 4>, 4>>();
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat.m[i][j] = m[i][j];
			}
		}
	}

	inline void to_json(nlohmann::json& data, const MATRIX& mat)
	{
		std::array<std::array<float, 4>, 4> m{};
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = mat.m[i][j];
			}
		}

		data = mat;
	}
}
#pragma endregion
