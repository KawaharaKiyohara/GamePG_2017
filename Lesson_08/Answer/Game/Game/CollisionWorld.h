/*!
 *@brief	�R���W�������[���h�B
 */

#pragma once

//�R���W���������B
enum ECollisionAttr {
	enCollisionAttr_PlayerAttack,		//�v���C���[�̍U���R���W�����B
};
/*!
 *@brief	���̃R���W�����B
 */
struct SphereCollision{
	CVector3		pos;		//���W�B
	float			radius;		//���a�B
	float			life;		//����(�P�ʁF�b)�B
	ECollisionAttr	attr;		//�R���W���������B
};
/*!
 *@brief	�R���W�������[���h�B
 */
class CollisionWorld : public IGameObject{
public:
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	CollisionWorld();
	/*!
	 *@brief	�f�X�g���N�^�B
	 */
	~CollisionWorld();
	/*!
	 *@brief	���̃R���W������ǉ��B
	 *@param[in]	pos		�ʒu�B
	 *@param[in]	radius	���a�B
	 *@param[in]	life	�����B
	 *@param[in]	attr	�R���W�����̑����B
	 */
	void Add( CVector3 pos, float radius, float life, ECollisionAttr attr)
	{
		SphereCollisionPtr collisionPtr = std::make_shared<SphereCollision>();
		collisionPtr->pos = pos;
		collisionPtr->radius = radius;
		collisionPtr->life = life;
		collisionPtr->attr = attr;
	}
	void Update() override;

	typedef std::shared_ptr<SphereCollision>	SphereCollisionPtr;
	std::list<SphereCollisionPtr>		m_collisionList;		//!<�R���W�����̃��X�g�B
};

extern CollisionWorld* collisionWorld;