#pragma once

#include "cocos2d.h"

class OBB2D
{
public:
	cocos2d::CCPoint center;
	cocos2d::CCPoint halfExtents;
	float rotationDeg;
	cocos2d::CCPoint axis[2];

public:
	OBB2D();
	OBB2D(const cocos2d::CCPoint& c, const cocos2d::CCPoint& halfExt, float rotDeg);

	void set(const cocos2d::CCPoint& c, const cocos2d::CCPoint& halfExt, float rotDeg);

	void computeAxes();
	void getCorners(cocos2d::CCPoint outCorners[4]) const;
	float projectionRadiusOn(const cocos2d::CCPoint& axisUnit) const;

	bool intersectsOBB(const OBB2D& other) const;
	bool containsPoint(const cocos2d::CCPoint& point) const;
	bool intersectsCircle(const cocos2d::CCPoint& circleCenter, float radius) const;
};
