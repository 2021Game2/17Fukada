#include "CFriendly.h"
#include "CEffect.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define OBJ "f14.obj"	//モデルのファイル
#define MTL "f14.mtl"	//モデルのマテリアルファイル

#define HP 10	//耐久値
#define VELOCITY 0.11f	//速度

CModel CFriendly::mModel;	//モデルデータ作成

#define FIRECOUNT 20	//発射間隔


CFriendly::CFriendly()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 100.0f), 30.0f)
	, mpEnemy(0)
	, mHp(HP)
	, mFireCount(0)
{
	//mTag = EFRIENDLY;
	mTag = EPLAYER;
	mColSearch.mTag = CCollider::ESEARCH;	//タグ設定

	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
}


//コンストラクタ
//CEnemy(位置, 回転, 拡縮)
CFriendly::CFriendly(const CVector& position, const CVector& rotation, const CVector& scale)
	: CFriendly()
{
	//位置、回転、拡縮を設定する
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;	//拡縮の設定
	CTransform::Update();	//行列の更新
	//優先度を1に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //削除して
	CTaskManager::Get()->Add(this); //追加する
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 200.0f) * mMatrixRotate;
}

//更新処理
void CFriendly::Update() {
	if (CPlayer::mStart > 0 && CPlayer::mHp > 0)
	{
		//HPが0以下の時　撃破
		if (mHp <= 0)
		{
			mHp--;
			//15フレーム毎にエフェクト
			if (mHp % 15 == 0)
			{
				//エフェクト生成
				new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
			//下降させる
			mPosition.mY -= 0.03f;
			CTransform::Update();	//行列更新
			return;	//呼び元へ戻す
		}


		//左向き（X軸）のベクトルを求める
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//上向き（Y軸）のベクトルを求める
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//前方向（Z軸）のベクトルを求める
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;


		//エネミーのポインタが0以外の時
		if (mpEnemy)
		{
			//プレイヤーまでのベクトルを求める
			CVector vp = mpEnemy->mPosition - mPosition;
			float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
			float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
			float dz = vp.Dot(vz);
			if (dx > 0.0f)
			{
				mRotation.mY++;
			}
			else {
				mRotation.mY--;
			}
			if (dy > 0.0f)
			{
				mRotation.mX--;
			}
			else {
				mRotation.mX++;
			}
			//X軸のズレが2.0以下
			if (-2.0f < dx && dx < 2.0f)
			{
				//Y軸のズレが2.0以下
				if (-2.0f < dy && dy < 2.0f)
				{
					if (dz > 0.0f) {
						if (mFireCount > 0)
						{
							mFireCount--;
						}
						else
						{
							mFireCount = FIRECOUNT;
							//弾を発射します
							CBullet* bullet = new CBullet();
							bullet->Set(0.1f, 1.5f);
							bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
							bullet->mRotation = mRotation;
							bullet->Update();
						}
					}
				}
			}
		}
		if (CPlayer::mHp <= 0)
		{
			mEnabled = false;
		}

		/*
		//目標地点までのベクトルを求める
		CVector vp = mPoint - mPosition;
		float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
		float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
		float margin = 0.1f;
		//左右方向へ回転
		if (dx > margin)
		{
			mRotation.mY += 2.0f;
		}
		else if (dx < -margin)
		{
			mRotation.mY -= 2.0f;
		}
		//上下方向へ回転
		if (dy > margin)
		{
			mRotation.mX -= 1.0f;
		}
		else if (dy < -margin)
		{
			mRotation.mX += 1.0f;
		}

		//移動する
		//mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;



		//およそ3秒毎に目標地点を更新
		int r = rand() % 180;	//rand()は整数の乱数を返す
								//% 180 は180で割った余りを求める
		if (r == 0)
		{
			if (mpEnemy)
			{
				mPoint = mpEnemy->mPosition;
			}
			else
			{
				mPoint = mPoint * CMatrix().RotateY(45);
			}
		}
		*/
		CTransform::Update();	//行列更新
		//mpEnemy = 0;
	}
}
//衝突処理
//Collision(コライダ1, コライダ2)
void CFriendly::Collision(CCollider* m, CCollider* o) {
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//自分がサーチ用の時
	if (m->mTag == CCollider::ESEARCH)
	{
		//相手が球コライダの時
		if (o->mType == CCollider::ESPHERE)
		{
			//相手がプレイヤーの時
			if (o->mpParent->mTag == EENEMY)
			{
				//衝突している時
				if (CCollider::Collision(m, o))
				{
					//プレイヤーのポインタ設定
					mpEnemy = o->mpParent;
				}
			}
		}
		return;
	}

	switch (o->mType)
	{
	case CCollider::ESPHERE:
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o)) {
			//エフェクト生成
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			mHp--;	//ヒットポイントの減算
		}
		break;
	case CCollider::ETRIANGLE:	//三角コライダの時
		CVector adjust; //調整値
		//撃破で地面に衝突すると無効
		if (mHp <= 0)
		{
			mEnabled = false;
		}
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{	//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
			
		}
		break;
	}

	return;

	switch (m->mTag)
	{
	case CCollider::ESEARCH:
		if (o->mType == CCollider::ESPHERE)
		{
			if (o->mpParent->mTag == EENEMY)
			{
				//コライダのmとyが衝突しているか判定
				if (CCollider::Collision(m, o))
				{
					//プレイヤーのポインタを設定
					mpEnemy = o->mpParent;
				}
			}
		}
		break;
	default:	//本体の時
		switch (o->mType)
		{
		case CCollider::ESPHERE:
			if (o->mTag == CCollider::EBODY)
			{
				//コライダのmとyが衝突しているか判定
				if (CCollider::Collision(m, o)) {
					//エフェクト生成
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				}
			}
			break;
		case CCollider::ETRIANGLE:	//三角コライダの時
			CVector adjust; //調整値
			//三角コライダと球コライダの衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
			}
			break;
		}
	}

	return;


	if (o->mTag == CCollider::ESEARCH)
		return;
	//相手のコライダタイプの判定
	switch (o->mType)
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o)) {
			switch (m->mTag)
			{
			case CCollider::ESEARCH:
				if (o->mpParent->mTag == EENEMY)
				{
					//プレイヤーのポインタを設定
					mpEnemy = o->mpParent;
				}
				break;
			default:
				//エフェクト生成
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時
		if (m->mTag == CCollider::EBODY)
		{
			CVector adjust; //調整値
			//三角コライダと球コライダの衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
			}
		}
		break;
	}
}

void CFriendly::TaskCollision()
{
	mCollider.ChangePriority();
	mColSearch.ChangePriority();
	//	mCollider3.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	//	CCollisionManager::Get()->Collision(&mCollider3, COLLISIONRANGE);
}
