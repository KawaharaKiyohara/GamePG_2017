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
	
	for(auto it = m_collisionList.begin(); it != m_collisionList.end(); it++){
		(*it)->life -= GameTime().GetFrameDeltaTime();
	}
	auto func = [](SphereCollisionPtr colli)-> bool
	{
		return colli->life < 0.0f;
	};
	
	auto it = std::remove_if(m_collisionList.begin(), m_collisionList.end(), func);
	m_collisionList.erase(it, m_collisionList.end());
}