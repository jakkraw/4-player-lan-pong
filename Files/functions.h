#pragma once 
#include <vector>
#include <string>


namespace functions {
	inline std::vector<std::string> explode(const std::string& str, const char delimiter) {
		std::vector<std::string> elements;
		std::stringstream stream(str);
		std::string item;
		while (getline(stream, item, delimiter))
			elements.push_back(item);
		return elements;
	}

	inline std::string toLower(std::string string) {
		std::transform(string.begin(), string.end(), string.begin(), ::tolower);
		return string;
	}

}


