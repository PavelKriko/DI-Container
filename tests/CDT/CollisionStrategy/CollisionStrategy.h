#pragma once

#include "IoC/IStrategy.h"
#include "CDT/MoveAdapter/IMovable.h"
#include "CDT/DT/IDT.h"

#include <any>



class CollisionStrategy : public IStrategy {

public:
	std::any executeStrategy(const argumentList& args) override {
		auto object_1 = std::any_cast<std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>>>(args.at("object_1"));
		auto object_2 = std::any_cast<std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>>>(args.at("object_2"));
		auto collisionDT = std::any_cast<std::shared_ptr<IDT<std::vector<double>, bool>>>(args.at("DT"));
		
		const auto pos1 = object_1->getPosition();
		const auto pos2 = object_2->getPosition();

		const auto height1 = object_1->getHeight();
		const auto height2 = object_2->getHeight();

		const auto widht1 = object_1->getWidth();
		const auto widht2 = object_2->getWidth();

		const auto dir1 = object_1->getDirection();
		const auto dir2 = object_2->getDirection();

		const auto vel1 = object_1->getVelocity();
		const auto vel2 = object_2->getVelocity();

		return collisionDT->predict({pos1[0]+(vel1*dir1[0]) - pos2[0] + (vel2 * dir2[0]),
			                         pos1[1] + (vel1 * dir1[1]) -pos2[1] + (vel2 * dir2[1]),
			                         height1-height2,
			                         widht1-widht2, 
			                         dir1[0]- dir2[0],
			                         dir1[1]- dir2[1]});
	}

};
