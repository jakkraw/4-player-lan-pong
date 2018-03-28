#include"entityCollection.h"

EntityCollection::~EntityCollection(){
	clear();
}

Entity* EntityCollection::get(id id)
{
	std::lock_guard<std::mutex> locker(mtx);
	Entity* entity = nullptr;
	auto found = collection.find(id);
	if (found != collection.end()) {
		entity = found->second;
	}

	return entity;
}

void EntityCollection::add(Entity*e){	
	std::lock_guard<std::mutex> locker(mtx);
	collection[e->getId()] = e;
	Log("[EC]Added item %d ", e->getId());
}

void EntityCollection::remove(id id){
	delete get(id);
	std::lock_guard<std::mutex> locker(mtx);
	collection.erase(id);
}

void EntityCollection::clear()
{
	std::lock_guard<std::mutex> locker(mtx);
	for (auto i : collection) {
		delete i.second;
	}collection.clear();
}


void EntityCollection::collisions()
{
	std::lock_guard<std::mutex> locker(mtx);
	for (auto i = collection.begin(); i != collection.end();i++)
	{
		auto j = i;
			while (++j != collection.end()){
				i->second->collide(j->second);
			}
	}
}

void EntityCollection::render()
{
	std::lock_guard<std::mutex> locker(mtx);
	for (auto i : collection) {
		i.second->draw();
	}
}

void  EntityCollection::update()
{
	std::lock_guard<std::mutex> locker(mtx);
	for (auto i : collection){
		i.second->update();
	}
}
