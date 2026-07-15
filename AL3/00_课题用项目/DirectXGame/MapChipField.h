#pragma once
#include "KamataEngine.h"
#include <cstdint>
#include <string>
#include <vector>

// マップチップ種別
enum class MapChipType {
	kBlank,
	kBlock,
};

// マップチップデータ
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	// 範囲矩形
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	/// <summary>
	/// マップチップデータをリセット
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// マップチップCSVを読み込む
	/// </summary>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ種別を取得する
	/// </summary>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標を取得する
	/// </summary>
	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// 座標からマップチップ番号を取得する
	/// </summary>
	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	/// <summary>
	/// ブロックの範囲矩形を取得する
	/// </summary>
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// 左下から探した最初の空白マスの座標を取得する
	/// </summary>
	KamataEngine::Vector3 GetLeftBottomBlankPosition();

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

private:
	MapChipData mapChipData_;
};
