#pragma once
#include<string>
#include<unordered_map>
#include<any>
#include"IStrategy.h"
#include"ResolveException.h"
#include<exception>
#include<memory>
#include<functional>

using argumentList = std::unordered_map<std::string, std::any>;

class IoC {
public:
	IoC() {}
	template<typename returnType>
	returnType resolve(const std::string& strategyName, const argumentList& args) {
		try {
			return std::any_cast<returnType>(iocStrategyStorage.at(strategyName)(args));
		}
		catch (...) {
			throw ResolveDependencyException("can't resolve");
		}
	}

	template<typename returnType>
	returnType resolve(const std::string & strategyName) {
		try {
			return std::any_cast<returnType>(iocStrategyStorage.at(strategyName)({}));
		}
		catch (...) {
			throw ResolveDependencyException("can't resolve");
		}
	}

	void addStrategy(const std::string &key, std::function<std::any(const argumentList&)> strategy){
		try { 
			iocStrategyStorage[key] = strategy; }
		catch (...) {
			throw ResolveDependencyException("can't add strategy");
		}
	}
	void addStrategy(const std::string& key, IStrategy& strategy) {
		try {
			iocStrategyStorage[key] = std::function<std::any(const argumentList&)>(std::bind(&IStrategy::executeStrategy, &strategy, std::placeholders::_1));
		}
		catch (...) {
			throw ResolveDependencyException("can't add strategy");
		}
	}

private:
	std::unordered_map<std::string, std::function<std::any(const argumentList&)> > iocStrategyStorage;
};

thread_local inline IoC IoContainer;



