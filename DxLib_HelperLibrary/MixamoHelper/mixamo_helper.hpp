#pragma once
#include <Axis/axis.hpp>
#include <JSON/json_loader.hpp>

/// @brief mixamoモデル用のヘルパー関数
/// @brief MEMO : 配布を行うため、既存の自作関数およびクラスは使用しないものとする
namespace mixamo_helper
{
    /// @brief 回転行列をXYZ軸に変換する
    /// @param rot_matrix 変換対象の回転行列
    /// @param out_right 変換後のX軸を格納
    /// @param out_up 変換後のY軸を格納
    /// @param out_forward 変換後のZ軸を格納
    [[nodiscard]] inline Axis ConvertRotMatrixToAxis(const MATRIX& rot_matrix)
    {
        // 回転行列を取得
        const auto rot      = MGetRotElem(rot_matrix);
        const auto x_axis   = v3d::GetNormalizedV(VGet(rot.m[0][0], rot.m[0][1], rot.m[0][2]));
        const auto y_axis   = v3d::GetNormalizedV(VGet(rot.m[1][0], rot.m[1][1], rot.m[1][2]));
        const auto z_axis   = v3d::GetNormalizedV(VGet(rot.m[2][0], rot.m[2][1], rot.m[2][2]));

        return { x_axis, y_axis, z_axis };
    }

	/// @brief モデルのフレームを描画する
	/// @param model_handle モデルハンドル
	/// @param is_draw_joint 関節を描画するかどうか (初期値 : true)
	/// @param is_draw_bone ボーンを描画するかどうか (初期値 : true)
	/// @param is_draw_axis 関節のXYZ軸を描画するかどうか (初期値 : true)
	/// @param is_fill 関節及びボーンを塗りつぶすかどうか (初期値 : true)
    inline void DrawFrames(const int model_handle, const bool is_draw_joint = true, const bool is_draw_bone = true, const bool is_draw_axis = true, const bool is_fill = true)
	{
		nlohmann::json data;
        if (!json_loader::Load("DxLib_HelperLibrary/Data/JSON/mixamo_frame_hierarchy.json", data)) { return; }
		
        // 最下層フレーム
		const auto  hips    = data.at("Armature").at("mixamorig:Hips");
        auto        hips_m  = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, "mixamorig:Hips"));

        // 子を辿る再帰関数を定義
        std::function<void(const nlohmann::json&, MATRIX&)> Traverse;

        Traverse = [&](const nlohmann::json& node, MATRIX& parent_matrix)
        {
            for (auto itr = node.begin(); itr != node.end(); ++itr)
            {
                const auto frame_name   = itr.key();
                const auto frame_index  = MV1SearchFrame(model_handle, frame_name.c_str());
                if (frame_index <= -1) { continue; }

                auto       child_m      = MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
                const auto child_pos    = MGetTranslateElem(child_m);
                const auto parent_pos   = MGetTranslateElem(parent_matrix);
                const auto distance     = VSize(child_pos - parent_pos);
                const auto radius       = distance * 0.2f;
                const auto axis_length  = distance * 0.6f;
                const auto parent_axis  = ConvertRotMatrixToAxis(parent_matrix);

                // 関節・ボーン・XYZ軸の描画
                if (is_draw_joint) { DrawSphere3D(parent_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                if (is_draw_bone ) { DrawCone3D(child_pos, parent_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                if (is_draw_axis ) { axis::Draw(parent_axis, parent_pos, axis_length); }

                // 子がいないため再帰しない
                if (itr.value().empty())
                {
                    const auto child_axis = ConvertRotMatrixToAxis(child_m);
                    
                    // 子がいない場合、関節・XYZ軸のみ描画
                    if (is_draw_joint) { DrawSphere3D(child_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                    if (is_draw_axis)  { axis::Draw(child_axis, child_pos, axis_length); }

                    continue;
                }

                // 子がいるため再帰
                Traverse(itr.value(), child_m);
            }
        };

        Traverse(hips, hips_m);
	}
}
