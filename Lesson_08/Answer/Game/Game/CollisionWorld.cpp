/*!
 *@brief	�R���W�������[���h�B
 */

#include "stdafx.h"
#include "CollisionWorld.h"

CollisionWorld* collisionWorld = NULL;
/*!
 *@brief	�R���X�g���N�^�B
 */
CollisionWorld::CollisionWorld()
{
}
/*!
 *@brief	�f�X�g���N�^�B
 */
CollisionWorld::~CollisionWorld()
{
}
/*!
 *@brief	�X�V�B
 */
void CollisionWorld::Update()
{
	auto it = m_collisionList.begin();
	while( it != m_collisionList.end() ){
		if ((*it)->life < 0.0f) {
			//�R���W�������S�B
			it = m_collisionList.erase(it);
		}
		else {
			it++;
		}
	}
}