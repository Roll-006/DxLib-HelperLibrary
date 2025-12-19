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
	/// @brief X軸回転(ピッチ軸回転)をcosθ、sinθから生成
	[[nodiscard]] inline MATRIX CreateXMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] = 1.0f;			mat.m[0][1] = 0.0f;			mat.m[0][2] = 0.0f;			mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;			mat.m[1][1] =  cos_theta;	mat.m[1][2] = sin_theta;	mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;			mat.m[2][1] = -sin_theta;	mat.m[2][2] = cos_theta;	mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	/// @brief Y軸回転(ヨー軸回転)をcosθ、sinθから生成
	[[nodiscard]] inline MATRIX CreateYMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] = cos_theta;	mat.m[0][1] = 0.0f;			mat.m[0][2] = -sin_theta;	mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;			mat.m[1][1] = 1.0f;			mat.m[1][2] = 0.0f;			mat.m[1][3] = 0.0f;
		mat.m[2][0] = sin_theta;	mat.m[2][1] = 0.0f;			mat.m[2][2] =  cos_theta;	mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	/// @brief Z軸回転(ロール軸回転)をcosθ、sinθから生成
	[[nodiscard]] inline MATRIX CreateZMatrix(const float cos_theta, const float sin_theta)
	{
		auto mat = MGetIdent();
		mat.m[0][0] =  cos_theta;	mat.m[0][1] = sin_theta;	mat.m[0][2] = 0.0f;			mat.m[0][3] = 0.0f;
		mat.m[1][0] = -sin_theta;	mat.m[1][1] = cos_theta;	mat.m[1][2] = 0.0f;			mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;			mat.m[2][1] = 0.0f;			mat.m[2][2] = 1.0f;			mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;			mat.m[3][1] = 0.0f;			mat.m[3][2] = 0.0f;			mat.m[3][3] = 1.0f;
		return mat;
	}

	/// @brief 行列の座標成分を取得
	[[nodiscard]] inline VECTOR GetPos(const MATRIX& mat)
	{
		return { mat.m[3][0], mat.m[3][1], mat.m[3][2] };
	}

	/// @brief 行列のスケールを取得
	[[nodiscard]] inline VECTOR GetScale(const MATRIX& mat)
	{
		const auto scale_x = VGet(mat.m[0][0], mat.m[0][1], mat.m[0][2]);
		const auto scale_y = VGet(mat.m[1][0], mat.m[1][1], mat.m[1][2]);
		const auto scale_z = VGet(mat.m[2][0], mat.m[2][1], mat.m[2][2]);

		return { VSize(scale_x), VSize(scale_y), VSize(scale_z) };
	}
	
	/// @brief 行列の回転成分を取得
	[[nodiscard]] inline MATRIX GetRotMatrix(const MATRIX& mat)
	{
		const auto scale		= GetScale(mat);
		const auto scale_rot_m	= MGetRotElem(mat);

		auto rot_m = scale_rot_m;
		rot_m.m[0][0] /= scale.x; rot_m.m[0][1] /= scale.x; rot_m.m[0][2] /= scale.x;
		rot_m.m[1][0] /= scale.y; rot_m.m[1][1] /= scale.y; rot_m.m[1][2] /= scale.y;
		rot_m.m[2][0] /= scale.z; rot_m.m[2][1] /= scale.z; rot_m.m[2][2] /= scale.z;
		return rot_m;
	}

	/// @brief 行列に座標成分を設定
	inline void SetPos(MATRIX& mat, const VECTOR& pos)
	{
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
	}

	/// @brief 行列にスケール成分を設
	inline void SetScale(MATRIX& mat, const VECTOR& scale)
	{
		// 軸を正規化してスケールを掛ける
		const auto current_scale = GetScale(mat);

		if (current_scale.x != 0.0f)
		{
			mat.m[0][0] = (mat.m[0][0] / current_scale.x) * scale.x;
			mat.m[0][1] = (mat.m[0][1] / current_scale.x) * scale.x;
			mat.m[0][2] = (mat.m[0][2] / current_scale.x) * scale.x;
		}

		if (current_scale.y != 0.0f)
		{
			mat.m[1][0] = (mat.m[1][0] / current_scale.y) * scale.y;
			mat.m[1][1] = (mat.m[1][1] / current_scale.y) * scale.y;
			mat.m[1][2] = (mat.m[1][2] / current_scale.y) * scale.y;
		}

		if (current_scale.z != 0.0f)
		{
			mat.m[2][0] = (mat.m[2][0] / current_scale.z) * scale.z;
			mat.m[2][1] = (mat.m[2][1] / current_scale.z) * scale.z;
			mat.m[2][2] = (mat.m[2][2] / current_scale.z) * scale.z;
		}
	}

	/// @brief 行列に回転成分を設定
	inline void SetRot(MATRIX& mat, const MATRIX& rot_mat)
	{
		const auto scale = GetScale(mat);

		mat.m[0][0] = rot_mat.m[0][0] * scale.x;
		mat.m[1][0] = rot_mat.m[1][0] * scale.y;
		mat.m[2][0] = rot_mat.m[2][0] * scale.z;

		mat.m[0][1] = rot_mat.m[0][1] * scale.x;
		mat.m[1][1] = rot_mat.m[1][1] * scale.y;
		mat.m[2][1] = rot_mat.m[2][1] * scale.z;

		mat.m[0][2] = rot_mat.m[0][2] * scale.x;
		mat.m[1][2] = rot_mat.m[1][2] * scale.y;
		mat.m[2][2] = rot_mat.m[2][2] * scale.z;
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
