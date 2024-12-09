#pragma once

#include"IoC/IStrategy.h"
#include"ICommand/ICommand.h"
#include"UObject/UObject.h"

#include<functional>

class BuildTank : public IStrategy {
public:
	BuildTank() {}
	std::any executeStrategy(const argumentList& args) override {
		std::shared_ptr<UObject> tank = std::make_shared<UObject>();
		tank->addProperty("position", std::vector<int>{0, 0});
		tank->addProperty("velocity", int(0));
		tank->addProperty("moveDirection", std::vector<int>{0, 0});
		return tank;
	}
};

