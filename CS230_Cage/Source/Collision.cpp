/******************************************************************************/
/*!
\file		Collision.cpp
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 2016
\brief

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#include "main.h"

/******************************************************************************/
/*!
 */
/******************************************************************************/
void BuildLineSegment(LineSegment &lineSegment,
	const AEVec2 &pos,
	float scale,
	float dir)
{
	// your code goes here
	UNREFERENCED_PARAMETER(lineSegment);
	UNREFERENCED_PARAMETER(pos);
	UNREFERENCED_PARAMETER(scale);
	UNREFERENCED_PARAMETER(dir);
}

/******************************************************************************/
/*!
 */
/******************************************************************************/
int CollisionIntersection_CircleLineSegment(const Circle &circle,
	const AEVec2 &ptEnd,
	const LineSegment &lineSeg,
	AEVec2 &interPt,
	AEVec2 &normalAtCollision,
	float &interTime,
	bool & checkLineEdges)
{
	// your code goes here
	UNREFERENCED_PARAMETER(circle);
	UNREFERENCED_PARAMETER(ptEnd);
	UNREFERENCED_PARAMETER(lineSeg);
	UNREFERENCED_PARAMETER(normalAtCollision);
	UNREFERENCED_PARAMETER(interPt);
	UNREFERENCED_PARAMETER(interTime);
	UNREFERENCED_PARAMETER(checkLineEdges);

	return 0; // no intersection
}

/******************************************************************************/
/*!
*/
/******************************************************************************/
int CheckMovingCircleToLineEdge(bool withinBothLines,
	const Circle &circle,
	const AEVec2 &ptEnd,
	const LineSegment &lineSeg,
	AEVec2 &interPt,
	AEVec2 &normalAtCollision,
	float &interTime)
{
	// your code goes here
	UNREFERENCED_PARAMETER(withinBothLines);
	UNREFERENCED_PARAMETER(circle);
	UNREFERENCED_PARAMETER(ptEnd);
	UNREFERENCED_PARAMETER(lineSeg);
	UNREFERENCED_PARAMETER(interPt);
	UNREFERENCED_PARAMETER(normalAtCollision);
	UNREFERENCED_PARAMETER(interTime);

	return 0;//no collision
}

/******************************************************************************/
/*!
 */
/******************************************************************************/
int CollisionIntersection_CircleCircle(const Circle &circleA,
	const AEVec2 &velA,
	const Circle &circleB,
	const AEVec2 &velB,
	AEVec2 &interPtA,
	AEVec2 &interPtB,
	float &interTime)
{
	// your code goes here
	UNREFERENCED_PARAMETER(circleA);
	UNREFERENCED_PARAMETER(velA);
	UNREFERENCED_PARAMETER(circleB);
	UNREFERENCED_PARAMETER(velB);
	UNREFERENCED_PARAMETER(interPtA);
	UNREFERENCED_PARAMETER(interPtB);
	UNREFERENCED_PARAMETER(interTime);

	return 0;
}

/******************************************************************************/
/*!
 */
/******************************************************************************/
int CollisionIntersection_RayCircle(const Ray &ray,
	const Circle &circle,
	float &interTime)
{
	// your code goes here
	UNREFERENCED_PARAMETER(ray);
	UNREFERENCED_PARAMETER(circle);
	UNREFERENCED_PARAMETER(interTime);

	return 0;
}


/******************************************************************************/
/*!
 */
/******************************************************************************/
void CollisionResponse_CircleLineSegment(const AEVec2 &ptInter,
	const AEVec2 &normal,
	AEVec2 &ptEnd,
	AEVec2 &reflected)
{
	// your code goes here
	UNREFERENCED_PARAMETER(ptInter);
	UNREFERENCED_PARAMETER(normal);
	UNREFERENCED_PARAMETER(ptEnd);
	UNREFERENCED_PARAMETER(reflected);
}

/******************************************************************************/
/*!
 */
/******************************************************************************/
void CollisionResponse_CirclePillar(const AEVec2 &normal,
	const float &interTime,
	const AEVec2 &ptStart,
	const AEVec2 &ptInter,
	AEVec2 &ptEnd,
	AEVec2 &reflectedVectorNormalized)
{
	// your code goes here
	UNREFERENCED_PARAMETER(normal);
	UNREFERENCED_PARAMETER(interTime);
	UNREFERENCED_PARAMETER(ptStart);
	UNREFERENCED_PARAMETER(ptInter);	
	UNREFERENCED_PARAMETER(ptEnd);
	UNREFERENCED_PARAMETER(reflectedVectorNormalized);
}

/******************************************************************************/
/*!
	Extra credits
 */
/******************************************************************************/
void CollisionResponse_CircleCircle(AEVec2 &normal,
	const float interTime,
	AEVec2 &velA,
	const float &massA,
	AEVec2 &interPtA,
	AEVec2 &velB,
	const float &massB,
	AEVec2 &interPtB,
	AEVec2 &reflectedVectorA,
	AEVec2 &ptEndA,
	AEVec2 &reflectedVectorB,
	AEVec2 &ptEndB)
{
	UNREFERENCED_PARAMETER(normal);
	UNREFERENCED_PARAMETER(interTime);
	UNREFERENCED_PARAMETER(velA);
	UNREFERENCED_PARAMETER(massA);
	UNREFERENCED_PARAMETER(interPtA);
	UNREFERENCED_PARAMETER(velB);
	UNREFERENCED_PARAMETER(massB);
	UNREFERENCED_PARAMETER(interPtB);	
	UNREFERENCED_PARAMETER(reflectedVectorA);
	UNREFERENCED_PARAMETER(ptEndA);
	UNREFERENCED_PARAMETER(reflectedVectorB);
	UNREFERENCED_PARAMETER(ptEndB);	
}
