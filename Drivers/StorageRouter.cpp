#include "StorageRouter.h"

void StorageRouter::addStorage(DataSource source, std::unique_ptr<DataStorage> storage)
{
	storages_[source] = std::move(storage);
}

void StorageRouter::store(const ParseResult& result)
{
	auto it = storages_.find(result.source);
	if (it != storages_.end()) {
		it->second->store(result);
	}
}
