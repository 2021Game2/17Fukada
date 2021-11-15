#include "CFriendly.h"
#include "CEffect.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define OBJ "f14.obj"	//���f���̃t�@�C��
#define MTL "f14.mtl"	//���f���̃}�e���A���t�@�C��

#define HP 3	//�ϋv�l
#define VELOCITY 0.11f	//���x

CModel CFriendly::mModel;	//���f���f�[�^�쐬

#define FIRECOUNT 10	//���ˊԊu


CFriendly::CFriendly()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	, mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 100.0f), 30.0f)
	, mpEnemy(0)
	, mHp(HP)
	, mFireCount(0)
{
	//mTag = EFRIENDLY;
	mTag = EPLAYER;
	mColSearch.mTag = CCollider::ESEARCH;	//�^�O�ݒ�

	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
}


//�R���X�g���N�^
//CEnemy(�ʒu, ��], �g�k)
CFriendly::CFriendly(const CVector& position, const CVector& rotation, const CVector& scale)
	: CFriendly()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;	//�g�k�̐ݒ�
	CTransform::Update();	//�s��̍X�V
	//�D��x��1�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //�폜����
	CTaskManager::Get()->Add(this); //�ǉ�����
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 200.0f) * mMatrixRotate;
}

//�X�V����
void CFriendly::Update() {
	if (CPlayer::mStart > 0)
	{
		//HP��0�ȉ��̎��@���j
		if (mHp <= 0)
		{
			mHp--;
			//15�t���[�����ɃG�t�F�N�g
			if (mHp % 15 == 0)
			{
				//�G�t�F�N�g����
				new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
			//���~������
			mPosition.mY -= 0.03f;
			CTransform::Update();	//�s��X�V
			return;	//�Ăь��֖߂�
		}


		//�������iX���j�̃x�N�g�������߂�
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//������iY���j�̃x�N�g�������߂�
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//�O�����iZ���j�̃x�N�g�������߂�
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;


		//�G�l�~�[�̃|�C���^��0�ȊO�̎�
		if (mpEnemy)
		{
			//�v���C���[�܂ł̃x�N�g�������߂�
			CVector vp = mpEnemy->mPosition - mPosition;
			float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
			float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
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
			//X���̃Y����2.0�ȉ�
			if (-2.0f < dx && dx < 2.0f)
			{
				//Y���̃Y����2.0�ȉ�
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
							//�e�𔭎˂��܂�
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
		//�ڕW�n�_�܂ł̃x�N�g�������߂�
		CVector vp = mPoint - mPosition;
		float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
		float margin = 0.1f;
		//���E�����։�]
		if (dx > margin)
		{
			mRotation.mY += 2.0f;
		}
		else if (dx < -margin)
		{
			mRotation.mY -= 2.0f;
		}
		//�㉺�����։�]
		if (dy > margin)
		{
			mRotation.mX -= 1.0f;
		}
		else if (dy < -margin)
		{
			mRotation.mX += 1.0f;
		}

		//�ړ�����
		//mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;



		//���悻3�b���ɖڕW�n�_���X�V
		int r = rand() % 180;	//rand()�͐����̗�����Ԃ�
								//% 180 ��180�Ŋ������]������߂�
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
		CTransform::Update();	//�s��X�V
		//mpEnemy = 0;
	}
}
//�Փˏ���
//Collision(�R���C�_1, �R���C�_2)
void CFriendly::Collision(CCollider* m, CCollider* o) {
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH)
	{
		//���肪���R���C�_�̎�
		if (o->mType == CCollider::ESPHERE)
		{
			//���肪�v���C���[�̎�
			if (o->mpParent->mTag == EENEMY)
			{
				//�Փ˂��Ă��鎞
				if (CCollider::Collision(m, o))
				{
					//�v���C���[�̃|�C���^�ݒ�
					mpEnemy = o->mpParent;
				}
			}
		}
		return;
	}

	switch (o->mType)
	{
	case CCollider::ESPHERE:
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)) {
			//�G�t�F�N�g����
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			mHp--;	//�q�b�g�|�C���g�̌��Z
		}
		break;
	case CCollider::ETRIANGLE:	//�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//���j�Œn�ʂɏՓ˂���Ɩ���
		if (mHp <= 0)
		{
			mEnabled = false;
		}
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
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
				//�R���C�_��m��y���Փ˂��Ă��邩����
				if (CCollider::Collision(m, o))
				{
					//�v���C���[�̃|�C���^��ݒ�
					mpEnemy = o->mpParent;
				}
			}
		}
		break;
	default:	//�{�̂̎�
		switch (o->mType)
		{
		case CCollider::ESPHERE:
			if (o->mTag == CCollider::EBODY)
			{
				//�R���C�_��m��y���Փ˂��Ă��邩����
				if (CCollider::Collision(m, o)) {
					//�G�t�F�N�g����
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				}
			}
			break;
		case CCollider::ETRIANGLE:	//�O�p�R���C�_�̎�
			CVector adjust; //�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
			}
			break;
		}
	}

	return;


	if (o->mTag == CCollider::ESEARCH)
		return;
	//����̃R���C�_�^�C�v�̔���
	switch (o->mType)
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)) {
			switch (m->mTag)
			{
			case CCollider::ESEARCH:
				if (o->mpParent->mTag == EENEMY)
				{
					//�v���C���[�̃|�C���^��ݒ�
					mpEnemy = o->mpParent;
				}
				break;
			default:
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		if (m->mTag == CCollider::EBODY)
		{
			CVector adjust; //�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{	//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
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
