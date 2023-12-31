/******************************************************************************/
/*!
\file		GameState_Cage.cpp
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
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX		= 32;	//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX	= 2048;	//The total number of different game object instances

//Flags
const unsigned int	FLAG_ACTIVE				= 0x00000001;
const unsigned int	FLAG_VISIBLE			= 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE		= 0x00000004;

const float			PI_OVER_180				= PI/180.0f;


//values: 0,1,2,3
//0: original: 
//1: Extra Credits: with line edges only 
//2: Extra Credits: circle-circle-with-mass only (no line edges)
//3: 1&2 = All extra credits included
int EXTRA_CREDITS = 0;



enum TYPE_OBJECT
{
	TYPE_OBJECT_BALL,	//0
	TYPE_OBJECT_WALL,	//1
	TYPE_OBJECT_PILLAR, //2

	TYPE_OBJECT_NUM
};

/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/
struct GameObj
{
	unsigned int	type;		// object type
	AEGfxVertexList *	pMesh;		// pbject
};


struct GameObjInst
{
	GameObj*		pObject;	// pointer to the 'original'
	unsigned int	flag;		// bit flag or-ed together
	float			scale;
	AEVec2			posCurr;	// object current position
	AEVec2			velCurr;	// object current velocity
	float			dirCurr;	// object current direction
	float			speed;

	AEMtx33			transform;	// object drawing matrix

	// pointer to custom data specific for each object type
	void*			pUserData;
};


/******************************************************************************/
/*!
	File globals
*/
/******************************************************************************/
// list of original objects
static GameObj			*sGameObjList;
static unsigned int		sGameObjNum;

// list of object instances
static GameObjInst		*sGameObjInstList;
static unsigned int		sGameObjInstNum;

// function to create/destroy a game object instance
static GameObjInst*		gameObjInstCreate (unsigned int type, 
										   float scale, 
										   AEVec2* pPos, 
										   AEVec2* pVel, 
										   float dir);
static void				gameObjInstDestroy(GameObjInst* pInst);

static AECircle			*sBallData = 0;
static AELineSegment	*sWallData = 0;
static AECircle			*sPillarData = 0;

u32	fontId = 0;

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageLoad(void)
{
	sGameObjList = (GameObj *)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
	sGameObjInstList = (GameObjInst *)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));
	sGameObjNum = 0;

	GameObj* pObj;

	//------------------------------------------

	//Creating the ball object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_BALL;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the ball shape
	int Parts = 12;
	for(float i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		cosf(i*2*PI/Parts)*1.0f,  sinf(i*2*PI/Parts)*1.0f, 0xFFFFFF00, 0.0f, 0.0f,
		cosf((i+1)*2*PI/Parts)*1.0f,  sinf((i+1)*2*PI/Parts)*1.0f, 0xFFFFFF00, 0.0f, 0.0f);
	}

	pObj->pMesh = AEGfxMeshEnd();

	//------------------------------------------

	// Creating the wall object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_WALL;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the wall shape
	AEGfxVertexAdd(-0.5f, 0.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxVertexAdd(0.5f, 0.0f, 0xFF00FF00, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();

	//------------------------------------------

	//Creating the pillar object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_PILLAR;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the ball shape
	Parts = 12;
	for(float i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
		0.0f, 0.0f, 0xFF00FF00, 0.0f, 0.0f,
		cosf(i*2*PI/Parts)*1.0f,  sinf(i*2*PI/Parts)*1.0f, 0xFF00FF00, 0.0f, 0.0f,
		cosf((i+1)*2*PI/Parts)*1.0f,  sinf((i+1)*2*PI/Parts)*1.0f, 0xFF00FF00, 0.0f, 0.0f);
	}

	pObj->pMesh = AEGfxMeshEnd();

	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);

	fontId = AEGfxCreateFont("Courier New", 24, true, false);
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageInit(void)
{
	GameObjInst *pInst;
	std::string str;
	std::ifstream inFile;

	if(EXTRA_CREDITS == 0)
		inFile.open("Resources\\LevelData - Original.txt");
	else if (EXTRA_CREDITS == 1)
		inFile.open("Resources\\LevelData - Extra - NoMass.txt");
	else if(EXTRA_CREDITS == 2)
		inFile.open("Resources\\LevelData - Extra - WithMass1.txt");
	else// if(EXTRA_CREDITS == 3)
		inFile.open("Resources\\LevelData - Extra - WithMass2.txt");
	
	if(inFile.is_open())
	{
		// read ball data
		float dir, speed, scale;
		unsigned int ballNum = 0;
		inFile>>ballNum;
		sBallData = new AECircle[ballNum];

		for(unsigned int i = 0; i < ballNum; ++i)
		{
			// read pos
			inFile>>str>>sBallData[i].m_center.x;
			inFile>>str>>sBallData[i].m_center.y;
			// read direction
			inFile>>str>>dir;
			// read speed
			inFile>>str>>speed;
			// read radius
			inFile>>str>>sBallData[i].m_radius;
			if (EXTRA_CREDITS >= 2)
			{
				// read radius
				inFile >> str >> sBallData[i].m_mass;
			}
			// create ball instance
			AEVec2 vel;
			AEVec2Set(&vel, cos(dir*PI_OVER_180)*speed, 
							sin(dir*PI_OVER_180)*speed);
			pInst = gameObjInstCreate(TYPE_OBJECT_BALL, sBallData[i].m_radius, 
										&sBallData[i].m_center, &vel, 0.0f);
			AE_ASSERT(pInst);
			pInst->speed = speed;
			pInst->pUserData = &sBallData[i];
		}

		// read wall data
		unsigned int wallNum = 0;
		AEVec2 pos;

		inFile>>wallNum;
		sWallData = new AELineSegment[wallNum];

		for(unsigned int i = 0; i < wallNum; ++i)
		{
			inFile>>str>>pos.x;
			inFile>>str>>pos.y;
			inFile>>str>>dir;
			inFile>>str>>scale;
			AEBuildLineSegment(sWallData[i], pos, scale, dir*PI_OVER_180);
			pInst = gameObjInstCreate(TYPE_OBJECT_WALL, scale, &pos, 0, dir*PI_OVER_180); 
			AE_ASSERT(pInst);
			pInst->pUserData = &sWallData[i];
		}

		// read pillar data
		unsigned int pillarNum = 0;
		
		inFile>>pillarNum;
		sPillarData = new AECircle[pillarNum];

		for(unsigned int i = 0; i < pillarNum; ++i)
		{
			// read pos
			inFile>>str>>sPillarData[i].m_center.x;
			inFile>>str>>sPillarData[i].m_center.y;
			// read radius
			inFile>>str>>sPillarData[i].m_radius;
			// create pillar instance
			pInst = gameObjInstCreate(TYPE_OBJECT_PILLAR, sPillarData[i].m_radius, 
										&sPillarData[i].m_center, 0, 0.0f);
			AE_ASSERT(pInst);
			pInst->speed = 0.0f;
			pInst->pUserData = &sPillarData[i];
		}

		inFile.clear();
		inFile.close();
	}
	else
	{
		//AE_ASSERT_MESG(inFile, "Failed to open the text file");
		printf("Failed to open the text file");
	}
}



/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageUpdate(void)
{
	AEVec2		interPtA, interPtB;
	AEVec2      normalAtCollision;
	float		interTime = 0.0f;

	f32 fpsT = (f32)AEFrameRateControllerGetFrameTime();

	//Update object instances positions
	for(unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pBallInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pBallInst->flag & FLAG_ACTIVE) ||
			pBallInst->pObject->type != TYPE_OBJECT_BALL)
			continue;

		AEVec2 posNext;
		posNext.x = pBallInst->posCurr.x + pBallInst->velCurr.x * fpsT;
		posNext.y = pBallInst->posCurr.y + pBallInst->velCurr.y * fpsT;

		// Update the latest ball data with the lastest ball's position
		AECircle &ballData = *((AECircle*)pBallInst->pUserData);
		ballData.m_center.x = pBallInst->posCurr.x;
		ballData.m_center.y = pBallInst->posCurr.y;

		// Check collision with walls
		for(unsigned int j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j)
		{
			GameObjInst *pInst = sGameObjInstList + j;

			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			switch(pInst->pObject->type)
			{
				case TYPE_OBJECT_WALL:
				{
					AELineSegment &lineSegData = *((AELineSegment*)pInst->pUserData);

					bool checkLineEdges = true;
					if (EXTRA_CREDITS == 2)
						checkLineEdges = false;

					if(AECollisionIntersection_CircleLineSegment(ballData, 
																   posNext, 
																   lineSegData, 
																   interPtA,
																   normalAtCollision,
																   interTime,
																   checkLineEdges))
					{
						AEVec2 reflectedVec;

						AECollisionResponse_CircleLineSegment(interPtA, 
															    normalAtCollision,
																posNext, 
																reflectedVec);

						pBallInst->velCurr.x = reflectedVec.x * pBallInst->speed;
						pBallInst->velCurr.y = reflectedVec.y * pBallInst->speed;
					}
				}
				break;
				
				case TYPE_OBJECT_PILLAR:
				{
					AECircle &pillarData = *((AECircle*)pInst->pUserData);

					AEVec2 velA, velB;
					AEVec2Scale(&velA, &pBallInst->velCurr, fpsT);
					AEVec2Scale(&velB, &pInst->velCurr, fpsT);
					
					if(AECollisionIntersection_CircleCircle(ballData, velA, pillarData, 
														velB, interPtA, interPtB, interTime))
					{
						AEVec2 normal;
						AEVec2Sub(&normal, &interPtA, &pInst->posCurr);
						AEVec2Normalize(&normal, &normal);

						AEVec2 reflectedVecNor;
						AECollisionResponse_CirclePillar(normal, interTime, pBallInst->posCurr, interPtA,
													   posNext, reflectedVecNor);

						pBallInst->velCurr.x = reflectedVecNor.x * pBallInst->speed;
						pBallInst->velCurr.y = reflectedVecNor.y * pBallInst->speed;
					}
				}
				break;

				case TYPE_OBJECT_BALL:
				{
					if ((EXTRA_CREDITS < 2) || (pInst == pBallInst))
						continue;

					AECircle &otherBallData = *((AECircle*)pInst->pUserData);

					AEVec2 velA, velB;
					
					AEVec2Scale(&velA, &pBallInst->velCurr, fpsT);
					AEVec2Scale(&velB, &pInst->velCurr, fpsT);

					if (AECollisionIntersection_CircleCircle(ballData, velA, otherBallData,
						velB, interPtA, interPtB, interTime))
					{

						if (EXTRA_CREDITS >= 2)
						{
							AEVec2 reflectedVecA, reflectedVecB;
							AEVec2 posNextB;//not used yet, even though computed below

							AEVec2 normal;
							AEVec2Sub(&normal, &interPtA, &interPtB);
							AEVec2Normalize(&normal, &normal);							

							AECollisionResponse_CircleCircle(normal, interTime, velA, ballData.m_mass, interPtA, velB, otherBallData.m_mass, interPtB,
								reflectedVecA, posNext, reflectedVecB, posNextB);

							pBallInst->speed = AEVec2Length(&reflectedVecA) / fpsT;//A: new speed
							AEVec2Normalize(&reflectedVecA, &reflectedVecA);//A: new speed direction
							
							pBallInst->velCurr.x = reflectedVecA.x * pBallInst->speed;
							pBallInst->velCurr.y = reflectedVecA.y * pBallInst->speed;


							pInst->speed = AEVec2Length(&reflectedVecB) / fpsT;//B: new speed
							AEVec2Normalize(&reflectedVecB, &reflectedVecB);//B: new speed direction

							pInst->velCurr.x = reflectedVecB.x * pInst->speed;
							pInst->velCurr.y = reflectedVecB.y * pInst->speed;
						}
						//else if (EXTRA_CREDITS == 1)//this is internal only - circle-circle no mass
						//{

						//	AEVec2 normal;
						//	AEVec2Sub(&normal, &interPtA, &pInst->posCurr);
						//	AEVec2Normalize(&normal, &normal);

						//	AEVec2 reflectedVecNor;
						//	AECollisionResponse_CirclePillar(normal, interTime, pBallInst->posCurr, interPtA,
						//		posNext, reflectedVecNor);

						//	pBallInst->velCurr.x = reflectedVecNor.x * pBallInst->speed;
						//	pBallInst->velCurr.y = reflectedVecNor.y * pBallInst->speed;

						//	//reflect the other ball
						//	AEVec2 posNextB;//not used yet, even though computed below
						//	AEVec2Set(&normal, -normal.x, -normal.y);
						//	AECollisionResponse_CirclePillar(normal, interTime, pInst->posCurr, interPtA,
						//		posNextB, reflectedVecNor);

						//	pInst->velCurr.x = reflectedVecNor.x * pInst->speed;
						//	pInst->velCurr.y = reflectedVecNor.y * pInst->speed;
						//}
					}
					break;
				}
			}
		}

		pBallInst->posCurr.x = posNext.x;
		pBallInst->posCurr.y = posNext.y;
	}

	
	//Computing the transformation matrices of the game object instances
	for(unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		AEMtx33 scale, rot, trans;
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		AEMtx33Rot(&rot, pInst->dirCurr);
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);

		AEMtx33Concat(&pInst->transform, &scale, &rot);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
	}

	if(AEInputCheckTriggered(AEVK_R))
		gGameStateNext = GS_RESTART;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageDraw(void)
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);
	//Drawing the object instances
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;
		
		AEGfxSetTransform(pInst->transform.m);

		if((pInst->pObject->type == TYPE_OBJECT_BALL) || (pInst->pObject->type == TYPE_OBJECT_PILLAR))
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		else if(pInst->pObject->type == TYPE_OBJECT_WALL)
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_LINES_STRIP);
	}

	char strBuffer[100];
	memset(strBuffer, 0, 100*sizeof(char));
	sprintf_s(strBuffer, "Frame Time:  %.6f", AEFrameRateControllerGetFrameTime());
	
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);	
	AEGfxPrint(fontId, strBuffer, 100, 270, 1.0f, 0.0f, 1.0f);
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageFree(void)
{
	// kill all object in the list
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		gameObjInstDestroy(sGameObjInstList + i);

	delete []sBallData;
	sBallData = NULL;
	
	delete []sWallData;
	sWallData = NULL;

	delete []sPillarData;
	sPillarData = NULL;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateCageUnload(void)
{
	// free all CREATED mesh
	for (u32 i = 0; i < sGameObjNum; i++)
		AEGfxMeshFree(sGameObjList[i].pMesh);

	free(sGameObjList);
	free(sGameObjInstList);


	AEGfxDestroyFont(fontId);
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned int type, 
							   float scale, 
							   AEVec2* pPos, 
							   AEVec2* pVel, 
							   float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if ((pInst->flag & FLAG_ACTIVE) == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject			 = sGameObjList + type;
			pInst->flag				 = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale			 = scale;
			pInst->posCurr			 = pPos ? *pPos : zero;
			pInst->velCurr			 = pVel ? *pVel : zero;
			pInst->dirCurr			 = dir;
			pInst->pUserData		 = 0;
			
			// return the newly created instance
			return pInst;
		}
	}

	return 0;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}