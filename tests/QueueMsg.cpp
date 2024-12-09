#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "UObject/UObject.h"
#include "ICommand/ICommand.h"
#include "IoC/IStrategy.h"
#include "IoC/IoC.h"

#include "MsgCommand/Movable.h"
#include "MsgCommand/AdapterMove.h"
#include "MsgCommand/CommandStartMove.h"
#include "MsgCommand/CommandMove.h"
#include "MsgCommand/CommandEndMove.h"
#include "MsgCommand/IQueue.h"
#include "MsgCommand/Queue.h"

#include "vectors/vectorOps.h"

#include "MsgCommand/ConvertMsgToCommand.h"

#include<any>

TEST(Msg, Queue) {

	IoContainer.addStrategy("getObj", [](const argumentList& args)->std::any {
		std::shared_ptr<IUObject> obj(new UObject);
		obj->addProperty("velocity", std::vector<int>{0, 0});
		obj->addProperty("position", std::vector<int>{0, 0});
		return obj;
	}); //here we build object with pos{0,0} and vel{0,0};

	IoContainer.addStrategy("Queue", [](const argumentList& args)->std::any {
		std::shared_ptr<IQueue> queue(new Queue);
		return queue;
	});
	
	IoContainer.addStrategy("StartMove", [](const argumentList& args)->std::any {
		std::shared_ptr<ICommand> startMove = std::make_shared<CommandStartMove<int>>(std::any_cast<std::shared_ptr<Movable<int>>>(args.at("object")), std::any_cast<std::vector<int>>(args.at("vel")));
		return startMove;
	});

	IoContainer.addStrategy("Move", [](const argumentList& args)->std::any {
		std::shared_ptr<ICommand> move = std::make_shared<CommandMove<int>>(std::any_cast<std::shared_ptr<Movable<int>>>(args.at("object")));
		return move;
	});

	IoContainer.addStrategy("EndMove", [](const argumentList& args)->std::any {
		std::shared_ptr<ICommand> endMove = std::make_shared<CommandEndMove<int>>(std::any_cast<std::shared_ptr<Movable<int>>>(args.at("object")));
		return endMove;
	});


	std::vector<std::string> msgFromPlayer{ "StartMove","Move","EndMove" };

	auto queue = IoContainer.resolve<std::shared_ptr<IQueue>>("Queue");

	auto obj = IoContainer.resolve<std::shared_ptr<IUObject>>("getObj");

	std::shared_ptr<Movable<int>> adapter = std::make_shared<AdapterMove>(obj);

	for (const auto &elem : msgFromPlayer) {
		queue->push_back(IoContainer.resolve<std::shared_ptr<ICommand>>(elem, { {"object",adapter},{"vel",std::vector<int>{5,5}}}));
	}

	EXPECT_TRUE(!queue->empty()); // queue is not empty(is good)

	while (!queue->empty()) {
		try{
			queue->top()->execute();
		}
		catch (...) {
			throw std::out_of_range("bad command");
		}
		queue->pop();
	}
	//after this commands obj must be in possition {5,5} with vel {0,0};

	std::vector<int> currPoss = { 5,5 };
	EXPECT_EQ(adapter->getPosition(), currPoss);

	std::vector<int> currVel = { 0,0 };
	EXPECT_EQ(adapter->getVelocity(), currVel);


}

TEST(MsgConverter, Queue_2) {
	IoContainer.addStrategy("getObj", [](const argumentList& args)->std::any {
		std::shared_ptr<IUObject> obj(new UObject);
		obj->addProperty("velocity", std::vector<int>{0, 0});
		obj->addProperty("position", std::vector<int>{0, 0});
		return obj;
	}); //here we build object with pos{0,0} and vel{0,0};

	IoContainer.addStrategy("Queue", [](const argumentList& args)->std::any {
		std::shared_ptr<IQueue> queue(new Queue);
		return queue;
	});

	auto queue = IoContainer.resolve<std::shared_ptr<IQueue>>("Queue");

	auto obj = IoContainer.resolve<std::shared_ptr<IUObject>>("getObj");

	ConvertMsgToCommand converter;

	IoContainer.addStrategy("ConvertMsgToCommand", converter);

	std::vector<std::string> msgFromPlayer{ "StartMove","Move","EndMove" };

	for (const auto& elem : msgFromPlayer) {
		queue->push_back(IoContainer.resolve<std::shared_ptr<ICommand>>("ConvertMsgToCommand", { {"msg",elem}, {"obj",obj},{"vel",std::vector<int>{5,5}}})); //
	}

	EXPECT_TRUE(!queue->empty()); // queue is not empty(is good)

	while (!queue->empty()) {
		try {
			queue->top()->execute();
		}
		catch (...) {
			throw std::out_of_range("bad command");
		}
		queue->pop();
	}
	//after this commands obj must be in possition {5,5} with vel {0,0};

	std::vector<int> currPoss = { 5,5 };
	EXPECT_EQ(std::any_cast<std::vector<int>>(obj->getProperty("position")), currPoss);

	std::vector<int> currVel = { 0,0 };
	EXPECT_EQ(std::any_cast<std::vector<int>>(obj->getProperty("velocity")), currVel);

}