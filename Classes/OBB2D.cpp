#include "OBB2D.h"
#include <cmath>
#include <algorithm>

USING_NS_CC;

static CCPoint normalize(const CCPoint& v)
{
	float len = sqrtf(v.x * v.x + v.y * v.y);
	if (len <= 0.00001f)
		return CCPointZero;
	return ccp(v.x / len, v.y / len);
}

OBB2D::OBB2D()
	: center(CCPointZero)
	, halfExtents(CCPointZero)
	, rotationDeg(0.f)
{
	computeAxes();
}

OBB2D::OBB2D(const CCPoint& c, const CCPoint& halfExt, float rotDeg)
	: center(c)
	, halfExtents(halfExt)
	, rotationDeg(rotDeg)
{
	computeAxes();
}

void OBB2D::set(const CCPoint& c, const CCPoint& halfExt, float rotDeg)
{
	center = c;
	halfExtents = halfExt;
	rotationDeg = rotDeg;
	computeAxes();
}

void OBB2D::computeAxes()
{
	float rad = CC_DEGREES_TO_RADIANS(rotationDeg);
	float c = cosf(rad);
	float s = sinf(rad);

	axis[0] = ccp(c, s);   // eye X
	axis[1] = ccp(-s, c);  // eye Y
}

void OBB2D::getCorners(CCPoint outCorners[4]) const
{
	CCPoint hx = ccp(axis[0].x * halfExtents.x,
		axis[0].y * halfExtents.x);

	CCPoint hy = ccp(axis[1].x * halfExtents.y,
		axis[1].y * halfExtents.y);

	outCorners[0] = center - hx - hy; // bottom-left
	outCorners[1] = center + hx - hy; // bottom-right
	outCorners[2] = center + hx + hy; // top-right
	outCorners[3] = center - hx + hy; // top-left
}

float OBB2D::projectionRadiusOn(const CCPoint& axisUnit) const
{
	float dotX = fabsf(axisUnit.x * axis[0].x + axisUnit.y * axis[0].y);
	float dotY = fabsf(axisUnit.x * axis[1].x + axisUnit.y * axis[1].y);

	return halfExtents.x * dotX + halfExtents.y * dotY;
}

bool OBB2D::intersectsOBB(const OBB2D& other) const
{
	CCPoint d = other.center - center;

	CCPoint axesToTest[4] =
	{
		axis[0],
		axis[1],
		other.axis[0],
		other.axis[1]
	};

	for (int i = 0; i < 4; ++i)
	{
		CCPoint au = normalize(axesToTest[i]);

		float dist = fabsf(d.x * au.x + d.y * au.y);
		float rA = projectionRadiusOn(au);
		float rB = other.projectionRadiusOn(au);

		if (dist > rA + rB)
			return false;
	}

	return true;
}

bool OBB2D::containsPoint(const CCPoint& point) const
{
	CCPoint d = point - center;

	float px = d.x * axis[0].x + d.y * axis[0].y;
	float py = d.x * axis[1].x + d.y * axis[1].y;

	return fabsf(px) <= halfExtents.x &&
		fabsf(py) <= halfExtents.y;
}

bool OBB2D::intersectsCircle(const CCPoint& circleCenter, float radius) const
{
	CCPoint d = circleCenter - center;

	float localX = d.x * axis[0].x + d.y * axis[0].y;
	float localY = d.x * axis[1].x + d.y * axis[1].y;

	float clampedX = MAX(-halfExtents.x, MIN(localX, halfExtents.x));
	float clampedY = MAX(-halfExtents.y, MIN(localY, halfExtents.y));

	CCPoint closest =
		center +
		ccp(axis[0].x * clampedX, axis[0].y * clampedX) +
		ccp(axis[1].x * clampedY, axis[1].y * clampedY);

	CCPoint diff = circleCenter - closest;

	float distSq = diff.x * diff.x + diff.y * diff.y;
	return distSq <= radius * radius;
}
