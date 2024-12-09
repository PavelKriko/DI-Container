#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CDT/CollisionStrategy/CollisionStrategy.h"
#include "CDT/DT/ReadCsv.h"
#include "CDT/DT/CDT.h"
#include "CDT/DT/Accuracy.h"
#include "IoC/IoC.h"
#include "UObject/UObject.h"
#include "CDT/MoveAdapter/MovableAdapter.h"
#include "CDT/CollisionStrategy/CollisionStrategy.h"

#include "vectors/vectorOps.h"

TEST(DTEST,ACCURACY_AND_COLLISION_STRATEGY) {
	X x_train;
	y y_train;

	X x_test;
	y y_test;

	readCsv("adds/train.csv", x_train, y_train);
	readCsv("adds/test.csv", x_test, y_test);
	
	std::shared_ptr<IDT<std::vector<double>, bool>> DT(new CDT(10));

	DT->fit(x_train, y_train);

	auto y_pred = DT->predict(x_test);

	auto acc = accuracy(y_test, y_pred);

	EXPECT_GT(acc, 0.9); //prediction accuracy > 0.9

	IoContainer.addStrategy("getTank", [](const argumentList& args)->std::any {
		auto tank = static_cast<std::shared_ptr<IUObject>>(std::make_shared<UObject>());
		tank->addProperty("position", args.at("position"));
		tank->addProperty("width", args.at("width"));
		tank->addProperty("height", args.at("height"));
		tank->addProperty("direction", args.at("direction"));
		tank->addProperty("velocity", args.at("velocity"));
		return tank;
	});      
	
	auto tank_1 = IoContainer.resolve<std::shared_ptr<IUObject>>("getTank", { {"position",std::vector<double>{4,4}},{"width",4.0},{"height",4.0},
	{"direction",std::vector<double>{0,1}},{"velocity", 0.0} });

	std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>> mov_tank1 = std::make_shared<MovableAdapter>(tank_1);

	auto tank_2 = IoContainer.resolve<std::shared_ptr<IUObject>>("getTank", { {"position",std::vector<double>{4,4}},{"width",7.0},{"height",7.0},
	{"direction",std::vector<double>{0.75,0.15}},{"velocity", 1.0} });

	std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>> mov_tank2 = std::make_shared<MovableAdapter>(tank_2);

	CollisionStrategy collisStr;

	IoContainer.addStrategy("isCollision", collisStr);

	EXPECT_TRUE(IoContainer.resolve<bool>("isCollision", { { "object_1",mov_tank1 },{"object_2",mov_tank2},{"DT",DT} })); //collision must be true;

	//now let's move the first tank away from the second one

	mov_tank1->setPosition(std::vector<double>{45, 45});

	EXPECT_FALSE(IoContainer.resolve<bool>("isCollision", { { "object_1",mov_tank1 },{"object_2",mov_tank2},{"DT",DT} }));

}

TEST(ADAPTER, CURENT_POS) {
	IoContainer.addStrategy("getTank", [](const argumentList& args)->std::any {
		auto tank = static_cast<std::shared_ptr<IUObject>>(std::make_shared<UObject>());
		tank->addProperty("position", args.at("position"));
				tank->addProperty("width", args.at("width"));
				tank->addProperty("height", args.at("height"));
				tank->addProperty("direction", args.at("direction"));
				tank->addProperty("velocity", args.at("velocity"));
				return tank;
	}); 

	auto tank_1 = IoContainer.resolve<std::shared_ptr<IUObject>>("getTank", { {"position",std::vector<double>{4,4}},{"width",4.0},{"height",4.0},
		{"direction",std::vector<double>{0,1}},{"velocity", 0.0} });


	std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>> mov_tank1 = std::make_shared<MovableAdapter>(tank_1);

	auto vec = mov_tank1->getPosition();

	mov_tank1->setPosition(std::vector<double>{1, 1});

	auto vec_1 = mov_tank1->getPosition();
	auto vec_2 = std::vector<double>{ 1,1 };
	EXPECT_EQ(vec_2, vec_1);
}

