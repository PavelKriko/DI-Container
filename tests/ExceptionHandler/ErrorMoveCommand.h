#pragma once

#include "CDT/MoveAdapter/IMovable.h"
#include "ICommand/ICommand.h"
#include <memory>
#include <stdexcept>


class ErrorMoveCommand : public ICommand {
public:
	void execute() override {
		throw std::out_of_range("moveCommand exception");
	}

private:
	std::shared_ptr<IMovable<std::vector<double>, double, std::vector<double>, double>> obj;
};
