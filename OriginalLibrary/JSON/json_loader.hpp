#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

namespace json_loader
{
	/// @brief JSONデータを外部ファイルに保存する
	/// @brief ファイルがない場合は自動的に作成される
	/// @param file_path JSONファイルパス
	/// @param data 保存するJSONデータ
	/// @return true : 保存成功, false : 保存失敗
    inline bool Save(const std::string_view& file_path, const nlohmann::json& data)
    {
        // 書き込み用ファイルを上書き指定で展開
        // ファイルがない場合自動作成
        std::ofstream ofs(std::string(file_path), std::ios::out);

        // 展開に失敗
        if (!ofs) { return false; }

        try
        {
            // 書き込み
            ofs << data.dump(4);
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

	/// @brief 外部ファイルからJSONデータを読み込む
	/// @param file_path JSONファイルパス
	/// @param data 読み込むJSONデータ
	/// @return true : 読み込み成功, false : 読み込み失敗
    [[nodiscard]] inline bool Load(const std::string_view& file_path, nlohmann::json& data)
    {
        // 読み込み用ファイルを展開
        std::string path = std::string(file_path);
        std::ifstream ifs(path);

        // 展開に失敗
        if (!ifs) return false;

        try
        {
            // 読み込み
            ifs >> data;
        }
        catch (...)
        {
            return false;
        }

        return true;
    }
}
