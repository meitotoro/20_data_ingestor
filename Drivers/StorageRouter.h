#pragma once
#include "CommonTypes.h"
#include "DataStorage.h"
#include <unordered_map>
#include <memory>

class StorageRouter
{
public:
	void addStorage(DataSource source, std::unique_ptr<DataStorage> storage);
	void store(const ParseResult& result);
private:
	std::unordered_map<DataSource, std::unique_ptr<DataStorage>> storages_;
};

