#ifndef CFRIENDLY_H
#define CFRIENDLY_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CFriendly : public CCharacter {
public:
	int mFireCount;

	CVector mPoint;	//目標地点

	int mHp;	//ヒットポイント

	CCharacter* mpEnemy;	//エネミーのポインタ
	CCharacter* mpFriendly;
	//モデルデータ
	static CModel mModel;

	//コライダ
	CCollider mCollider;
	CCollider mColSearch;	//サーチ用コライダ

	//コンストラクタ
	CFriendly();
	//CFriendly(位置, 回転, 拡縮)
	CFriendly(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1, コライダ2)
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};

#endif
