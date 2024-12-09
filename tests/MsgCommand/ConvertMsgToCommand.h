#pragma once

#include "IoC/IStrategy.h"
#include "ICommand/ICommand.h"
#include "ICommand/EmptyCommand.h"
#include "CommandStartMove.h"
#include "CommandMove.h"
#include "CommandEndMove.h"
#include "AdapterMove.h"

#include "Movable.h"

#include<functional>
#include<unordered_map>
#include<string>
#include<memory>
#include<any>

class ConvertMsgToCommand : public IStrategy {
public:
	ConvertMsgToCommand() {};

	std::any executeStrategy(const argumentList& args)override {
		try {
			return convert_map.at(std::any_cast<std::string>(args.at("msg")))(args);
		}
		catch (...) {
			return std::shared_ptr<ICommand>(new EmptyCommand);
		}
	}

private:
	std::unordered_map<std::string, std::function<std::shared_ptr<ICommand>(const argumentList& args)>> convert_map = {
		{"StartMove",[](const argumentList& args) {
		std::shared_ptr<Movable<int>> movingObj = std::make_shared<AdapterMove>(std::any_cast<std::shared_ptr<IUObject>>(args.at("obj")));
		std::shared_ptr<ICommand> startMove = std::make_shared<CommandStartMove<int>>(movingObj, std::any_cast<std::vector<int>>(args.at("vel")));
		return startMove;
		}},

		{"Move", [](const argumentList& args) {
			std::shared_ptr<Movable<int>> movingObj = std::make_shared<AdapterMove>(std::any_cast<std::shared_ptr<IUObject>>(args.at("obj")));
			std::shared_ptr<ICommand> move = std::make_shared<CommandMove<int>>(movingObj);
			return move;
		}},

		{"EndMove", [](const argumentList& args) {
			std::shared_ptr<Movable<int>> movingObj = std::make_shared<AdapterMove>(std::any_cast<std::shared_ptr<IUObject>>(args.at("obj")));
			std::shared_ptr<ICommand> endMove = std::make_shared<CommandEndMove<int>>(movingObj);
			return endMove;
		}}

	};
};