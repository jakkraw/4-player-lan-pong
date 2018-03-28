#pragma once

#include <unordered_map>
#include <mutex>
#include "logger.h"
#include "entity.h"
typedef std::unordered_map<::id, Entity*> entMap;
class EntityCollection
{
protected:
	std::mutex mtx;
	entMap collection;

public:
	
	virtual ~EntityCollection();
	virtual Entity* get(id);
	virtual void add(Entity*);
	virtual void remove(id);
	virtual void clear();

	virtual void render();
	virtual void update();
	virtual void collisions();
	
	entMap::iterator begin() { return collection.begin(); }

	entMap::const_iterator begin() const { return collection.cbegin(); }

	entMap::iterator end() { return collection.end(); }

	entMap::const_iterator end() const { return collection.cend(); }

};
