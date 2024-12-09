#pragma once

#include <stdexcept>
#include <iostream>
#include <vector>
#include "ICommand/ICommand.h"
#include "CommandMove.h"
#include "UObject/IUObject.h"
#include "GameQueue/IGameQueue.h"
#include "Movable.h"



template <typename T>
class CommandStartMove: public ICommand
{

private:
	std::shared_ptr<Movable<T>> _moveAdapt;
	std::vector<T> _vel;

public:
	CommandStartMove() = delete;
	CommandStartMove(std::shared_ptr<Movable<T>> move, const std::vector<T> vel) : _moveAdapt(move),_vel(vel) {}
		
	void execute() override
	{
		try{
			_moveAdapt->setVelocity(_vel);
		}
		catch (...) {
			throw std::out_of_range("startMoveCommand exception");
		}

	}
};