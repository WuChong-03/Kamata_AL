#include "MapChipField.h"
#include <cassert>
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

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex >= kNumBlockHorizontal) {
		return MapChipType::kBlank;
	}

	if (yIndex >= kNumBlockVirtical) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return {kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0.0f}; }

Vector3 MapChipField::GetLeftBottomBlankPosition() {

	for (int32_t yIndex = static_cast<int32_t>(kNumBlockVirtical) - 1; yIndex >= 0; --yIndex) {
		for (uint32_t xIndex = 0; xIndex < kNumBlockHorizontal; ++xIndex) {
			if (GetMapChipTypeByIndex(xIndex, static_cast<uint32_t>(yIndex)) == MapChipType::kBlank) {
				return GetMapChipPositionByIndex(xIndex, static_cast<uint32_t>(yIndex));
			}
		}
	}

	return {0.0f, 0.0f, 0.0f};
}
