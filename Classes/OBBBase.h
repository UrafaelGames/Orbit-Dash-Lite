#pragma once

#include "cocos2d.h"
#include "OBB2D.h"

class OBBBase : public cocos2d::CCNode
{
public:
	virtual OBB2D getOBB() const = 0;
	virtual ~OBBBase() {}
};