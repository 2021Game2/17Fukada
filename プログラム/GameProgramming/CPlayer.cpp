//プレイヤークラスのインクルード
#include "CPlayer.h"
//キー入力クラスのインクルード
#include "CKey.h"
//
#include "CBullet.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
//
#include "CUtil.h"
//
#include "CEffect.h"

#include"CFriendly.h"
CModel mModelC;
#define OBJ "f16.obj"
#define MTL "f16.mtl"
#define BULLET 30	//残弾
CPlayer *CPlayer::spThis = 0;

#define FIRECOUNT 15	//発射間隔
float CPlayer::mHp = 30;
float CPlayer::mStart = 0;
CPlayer::CPlayer()

	: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
	, mLine2(this, &mMatrix, CVector(0.0f, 5.0f, -8.0f), CVector(0.0f, -3.0f, -8.0f))
	, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
	, mFireCount(0)
	, mZandan(BULLET)	//残弾
	, mFly(0)
{
	mTag = EPLAYER;	//タグの設定
	spThis = this;
	//テクスチャファイルの読み込み（1行64列）
	mText.LoadTexture("FontWhite.tga", 1, 64);
}

//更新処理
void CPlayer::Update() {
	
	//左矢印キー入力で回転
	if (CKey::Push(VK_LEFT)){
		//Y軸の回転値を増加
		mRotation.mY += 1;
	}
	//右矢印キーで回転
	if (CKey::Push(VK_RIGHT)) {
		//Y軸の回転値を増加
		mRotation.mY -= 1;
	}
	//Wキー入力で前進
	if (CKey::Push('W')) {
		//Z軸方向に1進んだ値を回転移動させる
		mPosition = CVector(0.0f, 0.0f, 2.0f) * mMatrix;
	}
	//Sキー入力で上向き
	if (CKey::Push(VK_UP)) {
		//X軸の回転値を減算
		mRotation.mX -= 1;
	}
	//下矢印キー入力で上向き
	if (CKey::Push(VK_DOWN)) {
		//X軸の回転値を加算
		mRotation.mX += 1;
	}

	if (CKey::Push('D')) {
		//X軸方向に1進んだ値を回転移動させる
		mPosition = CVector(-1.5f, 0.0f, 0.0f) * mMatrix;
	}

	//Aキー入力で回転
	if (CKey::Push('A')) {
		//X軸方向に1進んだ値を回転移動させる
		mPosition = CVector(1.5f, 0.0f, 0.0f) * mMatrix;
	}
	//Sキー入力で上向き
	if (CKey::Push('S')) {
		//Z軸方向に1進んだ値を回転移動させる
		mPosition = CVector(0.0f, 0.0f, -1.0f) * mMatrix;
	}

	//Hキー入力で敵が移動開始
	if (CKey::Push('H')) {
		mStart++;
	}

	if (mFireCount > 0)
	{
		mFireCount--;
	}

	//スペースキー入力で弾発射
	if (CKey::Push(VK_SPACE) && mFireCount == 0 &&mZandan > 0) {
		mFireCount = FIRECOUNT;
		CBullet *bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
		bullet->mRotation = mRotation;
		bullet->Update();
		mZandan--;
//		TaskManager.Add(bullet);
	}
	//Rキーでリロード
	if (CKey::Push('R')) {
		mFireCount = 300;
		mZandan = 30;
	}

	if (CKey::Push('G')) {
		//軸方向に移動させる
		mPosition = CVector(0.0f, 2.0f, 0.0f) *mMatrix ;
		mFly = mFly + 2;
	}
	else if (mFly > 0)
	{
		mPosition = CVector(0.0f, -1.0f, 0.0f) * mMatrix ;
		mFly--;
	}

	if (CKey::Push('Z')) {
		//mModelC.Load(OBJ, MTL);
		new CFriendly(CVector(0.0f, 0.0f, 30.0f) * mMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	}



	//CTransformの更新
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o) {
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//自身のコライダタイプの判定
	switch (m->mType) {
	case CCollider::ELINE://線分コライダ
		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			//行列の更新
			CTransform::Update();
		}
		break;
	case CCollider::ESPHERE:
		//相手のコライダが球コライダの時
		if (o->mType == CCollider::ESPHERE) {
			if (CCollider::Collision(m, o))
			{
				//エフェクト生成
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				mHp--;
			
			}
		}
		break;
	}
}
//衝突処理
void CPlayer::TaskCollision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//親の描画処理
	CCharacter::Render();

	//2Dの描画開始
	CUtil::Start2D(-400, 400, -300, 300);
	//描画色の設定（緑色の半透明）
	glColor4f(0.0f, 1.0f, 0.0f, 0.99f);
	//文字列編集エリアの作成
	char buf[256];

	sprintf(buf, "BULLET:%2.0f", mZandan);
	
	//文字列の描画
	mText.DrawString(buf, 100, 30, 8, 16);

	sprintf(buf, "HP:%2.0f", mHp);
	mText.DrawString(buf, 100, 0, 8, 16);
	/*
	//Y座標の表示
	//文字列の設定
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//文字列の描画
	mText.DrawString(buf, 100, 30, 8, 16);

	//X軸回転値の表示
	//文字列の設定
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//文字列の描画
	mText.DrawString(buf, 100, 0, 8, 16);
	//X軸回転値の表示
	//文字列の設定
	sprintf(buf, "FX:%7.2f", (CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate).GetRotationX(CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate));
	//文字列の描画
	mText.DrawString(buf, 100, -20, 8, 16);



	//Y軸回転値の表示
	//文字列の設定
	sprintf(buf, "RY:%7.2f", mRotation.mY);
	//文字列の描画
	mText.DrawString(buf, 100, -100, 8, 16);

	//Y軸回転値の表示
	//文字列の設定
	sprintf(buf, "FY:%7.2f", (CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate).GetRotationY());
	//文字列の描画
	mText.DrawString(buf, 100, -120, 8, 16);
	*/

	//2Dの描画終了
	CUtil::End2D();

}