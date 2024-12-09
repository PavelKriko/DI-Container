#pragma once

#include"IUObject.h"
#include<unordered_map>
#include<exception>
#include<stdexcept>

class UObject:public IUObject {
public:
	UObject() {};
	void setPropertyValue(const std::string& key, const std::any& property) override;
		
	std::any getProperty(const std::string& key) override;

	void addProperty(const std::string& key, const std::any& property) override;

	void eraseProperty(const std::string& key) override;

	~UObject(){};
private: 
	std::unordered_map<std::string, std::any>property_storage;
};

