#pragma once

#include "Movable.h"
#include "UObject/IUObject.h"

#include <memory>
#include <any>

class AdapterMove : public Movable<int> {
public:
	AdapterMove() = delete;

	AdapterMove(std::shared_ptr<IUObject> move) : obj(move) {};

	std::vector<int> getPosition() override {
		return std::any_cast<std::vector<int>>(obj->getProperty("position"));
	}

	void setPosition(const std::vector<int>& value) override {
		obj->setPropertyValue("position", value);
	}

	std::vector<int> getVelocity() override {
		return std::any_cast<std::vector<int>>(obj->getProperty("velocity"));
	}

	void setVelocity(const std::vector<int>& value) {
		obj->setPropertyValue("velocity", value);
	}

private:
	std::shared_ptr<IUObject> obj;

};