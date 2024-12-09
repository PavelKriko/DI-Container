#pragma once


template <typename TypePos, typename TypeSize, typename DirectionType, typename VelocityType>
class IMovable {
public:
	virtual TypePos getPosition() = 0;

	virtual void setPosition(const TypePos& pos) = 0;

	virtual TypeSize getWidth() = 0;

	virtual TypeSize getHeight() = 0;

	virtual DirectionType getDirection() = 0;
    
	virtual void setVelocity(const VelocityType & velocity) = 0;

	virtual VelocityType getVelocity() = 0;

};