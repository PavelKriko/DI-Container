#pragma once

#include<string>
#include<any>

class IUObject {
public:
	virtual void setPropertyValue(const std::string & key,const std::any& property) = 0;
	virtual std::any getProperty(const std::string & key) = 0;
	virtual void eraseProperty(const std::string& key) = 0;
	virtual void addProperty(const std::string& key, const std::any& property) = 0;
	virtual ~IUObject() = default;
};