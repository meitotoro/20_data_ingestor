#pragma once
#include <vector>
#include "DataStorage.h"


struct PositionData {
	double lon;
	double lat;
	double alt;
};

class BeidouStorage :
	public DataStorage
{
public:
	void store(const ParseResult& result) override;
private:

	PositionData parsePosition(const std::vector<uint8_t>& data);
	void storePosition(const PositionData& pos);
};

