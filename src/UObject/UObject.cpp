#include"UObject.h"
#include<any>

void UObject::setPropertyValue(const std::string& key, const std::any& property)
{
	try {
		property_storage.at(key) = property;
	}
	catch (std::out_of_range) {
		throw std::out_of_range("can't set value, because property is not exist");
	}
}

std::any UObject::getProperty(const std::string& key)
{
	try {
		return property_storage.at(key);
	}
	catch (std::out_of_range) {
		throw std::out_of_range("can't get property, because property is not exist");
	}
}

void UObject::addProperty(const std::string& key, const std::any& property) 
{
	if (property_storage.find(key) == property_storage.end()) {
		property_storage[key] = property;
	}
	else {
		throw std::runtime_error("can't add property, because property already exist");
	}
}

void UObject::eraseProperty(const std::string& key)
{
	if (property_storage.erase(key)) {
		return;
	}
	else {
		throw std::runtime_error("can't erase property, because property is not exist");
	}
}
