#pragma once

#include "IMovable.h"
#include "UObject/IUObject.h"

#include<vector>
#include<memory>
#include<any>

class MovableAdapter : public IMovable<std::vector<double>,double,std::vector<double>,double> {
public:
	MovableAdapter() = delete;

	MovableAdapter(std::shared_ptr<IUObject> obj) : object(obj) {};

	void setPosition(const std::vector<double>& pos) override {
		object->setPropertyValue("position", pos);
	}

	std::vector<double> getPosition() override {
		return std::any_cast<std::vector<double>>(object->getProperty("position"));
	}

	double getWidth() override {
		return std::any_cast<double>(object->getProperty("width"));
	}

	double getHeight() override {
		return std::any_cast<double>(object->getProperty("height"));
	}

	std::vector<double> getDirection() override {
		return std::any_cast<std::vector<double>>(object->getProperty("direction"));
	}

	void setVelocity(const double & velocity) override {
		object->setPropertyValue("velocity", velocity);
	}
	
	double getVelocity() {
		return std::any_cast<double>(object->getProperty("velocity"));
	}


private:
	std::shared_ptr<IUObject> object;
};