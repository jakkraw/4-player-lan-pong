#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <regex>
#include "logger.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


template <class T>
class ResourceManager
{
	std::string name;
	std::unordered_map<std::string, T> resources;

	T(*createFunction)(const char*);
	void(*deleteFunction)(T);

	void loadResource(std::string path, std::string fileName);

public:
	ResourceManager(T(*createFunction)(const char*), void(*deleteFunction)(T));
	~ResourceManager();

	void setName(std::string name);
	void loadMatchingFiles(std::string path, std::regex match);
	T get(std::string fileName);
};


template<class T>
inline ResourceManager<T>::ResourceManager(T(*createFunction)(const char*), void(*deleteFunction)(T)) {
	this->createFunction = createFunction;
	this->deleteFunction = deleteFunction;
}

template<class T>
inline ResourceManager<T>::~ResourceManager()
{
	for (auto const &item : resources) {
		deleteFunction(item.second);
		Log("[RS_%s]Freed: %s",this->name.c_str() ,item.first.c_str());
	}
}

template<class T>
inline void ResourceManager<T>::loadResource(std::string path, std::string fileName)
{
	T x = createFunction((path + fileName).c_str());
	if (!x) {
		LogError("[RS_%s] Couldn't load : %s \n", this->name.c_str(), fileName.c_str());
		deleteFunction(x);
	}
	else {
		Log("[RS_%s]Loaded: %s", this->name.c_str(), fileName.c_str());
		resources.insert({ fileName, x });
	}
}

template<class T>
inline T ResourceManager<T>::get(std::string fileName)
{
	T item = resources[fileName];
	if(!item){
		LogError(
			"[RS_%s]Couldn't find : %s\n", this->name.c_str(), fileName.c_str());
		return nullptr;
	}

	return item;
}

template<class T>
inline void ResourceManager<T>::loadMatchingFiles(std::string path, std::regex match)
{

#ifdef _WIN32

	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile((path + "*").c_str(), &data);

	if (handle != INVALID_HANDLE_VALUE){
		do{
			if (std::regex_match(data.cFileName, match)){
					loadResource(path, data.cFileName);
			}
		} while (FindNextFile(handle, &data));
	}
	FindClose(handle);

#elif __APPLE__



#elif __linux__ || __unix__


#endif


}

template<class T>
inline void ResourceManager<T>::setName(std::string name) {
	this->name = name;
}


#endif