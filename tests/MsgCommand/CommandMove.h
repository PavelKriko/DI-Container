#pragma once

#include <vector>
#include "ICommand/ICommand.h"
#include "Movable.h"
#include <algorithm>
#include <memory>
#include "vectors/vectorOps.h"

template <typename T>
class CommandMove : public ICommand
{
private:
	std::shared_ptr<Movable<T>> _moveAdapt;

public:
	CommandMove() = delete;
	CommandMove(std::shared_ptr<Movable<T>> obj) : _moveAdapt(obj) {}

	void execute()
	{
		try {
			_moveAdapt->setPosition(_moveAdapt->getPosition() + _moveAdapt->getVelocity());
		}
		catch (...) {
			throw std::out_of_range("MoveCommand exception");
		}
	}
};