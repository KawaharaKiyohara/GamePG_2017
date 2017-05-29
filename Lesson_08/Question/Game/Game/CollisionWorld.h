/*!
 *@brief	コリジョンワールド。
 */

#pragma once

//コリジョン属性。
enum ECollisionAttr {
	enCollisionAttr_PlayerAttack,		//プレイヤーの攻撃コリジョン。
	enCollisionAttr_EnemyAttack,		//敵の攻撃コリジョン。
};
/*!
 *@brief	球体コリジョン。
 */
struct SphereCollision{
	CVector3		pos;		//座標。
	float			radius;		//半径。
	float			life;		//寿命(単位：秒)。
	ECollisionAttr	attr;		//コリジョン属性。
};
/*!
 *@brief	コリジョンワールド。
 */
class CollisionWorld : public IGameObject{
public:
	/*!
	 *@brief	コンストラクタ。
	 */
	CollisionWorld();
	/*!
	 *@brief	デストラクタ。
	 */
	~CollisionWorld();
	/*!
	 *@brief	球体コリジョンを追加。
	 *@param[in]	pos		位置。
	 *@param[in]	radius	半径。
	 *@param[in]	life	寿命。
	 *@param[in]	attr	コリジョンの属性。
	 */
	void Add( CVector3 pos, float radius, float life, ECollisionAttr attr)
	{
		SphereCollisionPtr collisionPtr = std::make_shared<SphereCollision>();
		collisionPtr->pos = pos;
		collisionPtr->radius = radius;
		collisionPtr->life = life;
		collisionPtr->attr = attr;
		m_collisionList.push_back(collisionPtr);
	}
	void Update() override;

	typedef std::shared_ptr<SphereCollision>	SphereCollisionPtr;
	std::vector<SphereCollisionPtr>		m_collisionList;		//!<コリジョンのリスト。
};

extern CollisionWorld* collisionWorld;