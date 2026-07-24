#include "MapChipField.h"
#include <cassert>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <map>
#include <sstream>

using namespace KamataEngine;

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {

	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// マップチップデータをリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		std::getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			std::getline(lineStream, word, ',');

			std::map<std::string, MapChipType>::iterator it = mapChipTable.find(word);
			if (it != mapChipTable.end()) {
				mapChipData_.data[i][j] = it->second;
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(int32_t xIndex, int32_t yIndex) {

	if (!IsValidIndex(xIndex, yIndex)) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[static_cast<std::size_t>(yIndex)][static_cast<std::size_t>(xIndex)];
}

Vector3 MapChipField::GetMapChipPositionByIndex(int32_t xIndex, int32_t yIndex) {

	return {
	    kBlockWidth * static_cast<float>(xIndex),
	    kBlockHeight * (static_cast<float>(kNumBlockVirtical - 1) - static_cast<float>(yIndex)),
	    0.0f,
	};
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};

	int32_t xIndex = static_cast<int32_t>(std::floor((position.x + kBlockWidth / 2.0f) / kBlockWidth));
	int32_t reversedYIndex = static_cast<int32_t>(std::floor((position.y + kBlockHeight / 2.0f) / kBlockHeight));
	int32_t yIndex = static_cast<int32_t>(kNumBlockVirtical) - 1 - reversedYIndex;

	indexSet.xIndex = xIndex;
	indexSet.yIndex = yIndex;

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(int32_t xIndex, int32_t yIndex) {

	// 指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

Vector3 MapChipField::GetLeftBottomBlankPosition() {

	for (int32_t yIndex = static_cast<int32_t>(kNumBlockVirtical) - 1; yIndex >= 0; --yIndex) {
		for (int32_t xIndex = 0; xIndex < static_cast<int32_t>(kNumBlockHorizontal); ++xIndex) {
			if (GetMapChipTypeByIndex(xIndex, yIndex) == MapChipType::kBlank) {
				return GetMapChipPositionByIndex(xIndex, yIndex);
			}
		}
	}

	return {0.0f, 0.0f, 0.0f};
}

bool MapChipField::IsValidIndex(int32_t xIndex, int32_t yIndex) const {

	if (xIndex < 0 || yIndex < 0) {
		return false;
	}

	const std::size_t y = static_cast<std::size_t>(yIndex);
	const std::size_t x = static_cast<std::size_t>(xIndex);

	if (y >= mapChipData_.data.size()) {
		return false;
	}

	return x < mapChipData_.data[y].size();
}
