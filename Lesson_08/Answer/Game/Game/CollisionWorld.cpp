/*!
 *@brief	コリジョンワールド。
 */

#include "stdafx.h"
#include "CollisionWorld.h"

CollisionWorld* collisionWorld = NULL;
/*!
 *@brief	コンストラクタ。
 */
CollisionWorld::CollisionWorld()
{
}
/*!
 *@brief	デストラクタ。
 */
CollisionWorld::~CollisionWorld()
{
}
/*!
 *@brief	更新。
 */
void CollisionWorld::Update()
{
	auto it = m_collisionList.begin();
	while( it != m_collisionList.end() ){
		if ((*it)->life < 0.0f) {
			//コリジョン死亡。
			it = m_collisionList.erase(it);
		}
		else {
			it++;
		}
	}
}