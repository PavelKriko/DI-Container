#pragma once

#include<string>
#include<any>

using argumentList = std::unordered_map<std::string, std::any>;

class IStrategy {
public:
	virtual  std::any executeStrategy(const argumentList& args) = 0;
	virtual ~IStrategy() = default;
};