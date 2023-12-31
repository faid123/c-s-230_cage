/******************************************************************************/
/*!
\file		Collision.h
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 2016
\brief

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#ifndef CS230_COLLISION_H_
#define CS230_COLLISION_H_


/******************************************************************************/
/*!
 */
/******************************************************************************/
struct LineSegment
{
	AEVec2	m_pt0;
	AEVec2	m_pt1;
	AEVec2	m_normal;
};

void BuildLineSegment(LineSegment &lineSegment,								//Line segment reference - input
					  const AEVec2 &pos,									//position - input
					  float scale,											//scale - input
					  float dir);											//direction - input

/******************************************************************************/
/*!
 */
/******************************************************************************/
struct Circle
{
	AEVec2  m_center;
	float	m_radius;

	// Extra credits
	float   m_mass{ 1.0f };
};

struct Ray
{
	AEVec2	m_pt0;
	AEVec2	m_dir;
};


// INTERSECTION FUNCTIONS
int CollisionIntersection_CircleLineSegment(const Circle &circle,			//Circle data - input
	const AEVec2 &ptEnd,													//End circle position - input
	const LineSegment &lineSeg,												//Line segment - input
	AEVec2 &interPt,														//Intersection position of the circle - output 
	AEVec2 &normalAtCollision,												//Normal vector at collision time - output
	float &interTime,														//Intersection time ti - output
	bool & checkLineEdges);													//The last parameter is new - for Extra Credits: true = check collision with line segment edges

// Extra credits
int CheckMovingCircleToLineEdge(bool withinBothLines,						//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
	const Circle &circle,													//Circle data - input
	const AEVec2 &ptEnd,													//End circle position - input
	const LineSegment &lineSeg,												//Line segment - input
	AEVec2 &interPt,														//Intersection position of the circle - output 
	AEVec2 &normalAtCollision,												//Normal vector at collision time - output
	float &interTime);														//Intersection time ti - output


// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
// In the case of "dynamic circle vs static pillar", velB will be 0.0f
int CollisionIntersection_CircleCircle(const Circle &circleA,				//CircleA data - input
	const AEVec2 &velA,														//CircleA velocity - input
	const Circle &circleB,													//CircleB data - input
	const AEVec2 &velB,														//CircleA velocity - input
	AEVec2 &interPtA,														//Intersection point of CircleA at collision time - output
	AEVec2 &interPtB,														//Intersection point of CircleB at collision time - output
	float &interTime);														//intersection time - output

// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
// this is a helper function to be used inside "CollisionIntersection_CircleCircle" function
// it is checking collision a moving dot vs a static circle
int CollisionIntersection_RayCircle(const Ray &ray,							//A ray containing the data of the moving dot - input
	const Circle &circle,													//Static circle data - input
	float &interTime);														//Intersection time - output






// RESPONSE FUNCTIONS
void CollisionResponse_CircleLineSegment(const AEVec2 &ptInter,				//Intersection position of the circle - input
	const AEVec2 &normal,													//Normal vector of reflection on collision time - input
	AEVec2 &ptEnd,															//Final position of the circle after reflection - output
	AEVec2 &reflected);														//Normalized reflection vector direction - output

void CollisionResponse_CirclePillar(const AEVec2 &normal,					//Normal vector of reflection on collision time - input
	const float &interTime,													//Intersection time - input
	const AEVec2 &ptStart,													//Starting position of the circle - input
	const AEVec2 &ptInter,													//Intersection position of the circle - input
	AEVec2 &ptEnd,															//Final position of the circle after reflection - output
	AEVec2 &reflectedVectorNormalized);										//Normalized reflection vector - output

// Extra credits
void CollisionResponse_CircleCircle(AEVec2 &normal,							//Normal vector of reflection on collision time - input
	const float interTime,													//Intersection time - input
	AEVec2 &velA,															//Velocity of CircleA - input
	const float &massA,														//Mass of CircleA - input
	AEVec2 &interPtA,														//Intersection position of circle A at collision time - input
	AEVec2 &velB,															//Velocity of CircleB - input
	const float &massB,														//Mass of CircleB - input
	AEVec2 &interPtB,														//Intersection position of circle B at collision time - input
	AEVec2 &reflectedVectorA,												//Non-Normalized reflected vector of Circle A - output
	AEVec2 &ptEndA,															//Final position of the circle A after reflection - output
	AEVec2 &reflectedVectorB,												//Non-Normalized reflected vector of Circle B - output
	AEVec2 &ptEndB);														//Final position of the circle B after reflection - output



#endif // CS230_COLLISION_H_