//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
//�L�[���̓N���X�̃C���N���[�h
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
#define BULLET 30	//�c�e
CPlayer *CPlayer::spThis = 0;

#define FIRECOUNT 15	//���ˊԊu
float CPlayer::mHp = 30;
float CPlayer::mStart = 0;
CPlayer::CPlayer()

	: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
	, mLine2(this, &mMatrix, CVector(0.0f, 5.0f, -8.0f), CVector(0.0f, -3.0f, -8.0f))
	, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
	, mFireCount(0)
	, mZandan(BULLET)	//�c�e
	, mFly(0)
{
	mTag = EPLAYER;	//�^�O�̐ݒ�
	spThis = this;
	//�e�N�X�`���t�@�C���̓ǂݍ��݁i1�s64��j
	mText.LoadTexture("FontWhite.tga", 1, 64);
}

//�X�V����
void CPlayer::Update() {
	
	//�����L�[���͂ŉ�]
	if (CKey::Push(VK_LEFT)){
		//Y���̉�]�l�𑝉�
		mRotation.mY += 1;
	}
	//�E���L�[�ŉ�]
	if (CKey::Push(VK_RIGHT)) {
		//Y���̉�]�l�𑝉�
		mRotation.mY -= 1;
	}
	//W�L�[���͂őO�i
	if (CKey::Push('W')) {
		//Z��������1�i�񂾒l����]�ړ�������
		mPosition = CVector(0.0f, 0.0f, 2.0f) * mMatrix;
	}
	//S�L�[���͂ŏ����
	if (CKey::Push(VK_UP)) {
		//X���̉�]�l�����Z
		mRotation.mX -= 1;
	}
	//�����L�[���͂ŏ����
	if (CKey::Push(VK_DOWN)) {
		//X���̉�]�l�����Z
		mRotation.mX += 1;
	}

	if (CKey::Push('D')) {
		//X��������1�i�񂾒l����]�ړ�������
		mPosition = CVector(-1.5f, 0.0f, 0.0f) * mMatrix;
	}

	//A�L�[���͂ŉ�]
	if (CKey::Push('A')) {
		//X��������1�i�񂾒l����]�ړ�������
		mPosition = CVector(1.5f, 0.0f, 0.0f) * mMatrix;
	}
	//S�L�[���͂ŏ����
	if (CKey::Push('S')) {
		//Z��������1�i�񂾒l����]�ړ�������
		mPosition = CVector(0.0f, 0.0f, -1.0f) * mMatrix;
	}

	//H�L�[���͂œG���ړ��J�n
	if (CKey::Push('H')) {
		mStart++;
	}

	if (mFireCount > 0)
	{
		mFireCount--;
	}

	//�X�y�[�X�L�[���͂Œe����
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
	//R�L�[�Ń����[�h
	if (CKey::Push('R')) {
		mFireCount = 300;
		mZandan = 30;
	}

	if (CKey::Push('G')) {
		//�������Ɉړ�������
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



	//CTransform�̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o) {
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->mType) {
	case CCollider::ELINE://�����R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE) {
			CVector adjust;//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			//�s��̍X�V
			CTransform::Update();
		}
		break;
	case CCollider::ESPHERE:
		//����̃R���C�_�����R���C�_�̎�
		if (o->mType == CCollider::ESPHERE) {
			if (CCollider::Collision(m, o))
			{
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				mHp--;
			
			}
		}
		break;
	}
}
//�Փˏ���
void CPlayer::TaskCollision()
{
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//�e�̕`�揈��
	CCharacter::Render();

	//2D�̕`��J�n
	CUtil::Start2D(-400, 400, -300, 300);
	//�`��F�̐ݒ�i�ΐF�̔������j
	glColor4f(0.0f, 1.0f, 0.0f, 0.99f);
	//������ҏW�G���A�̍쐬
	char buf[256];

	sprintf(buf, "BULLET:%2.0f", mZandan);
	
	//������̕`��
	mText.DrawString(buf, 100, 30, 8, 16);

	sprintf(buf, "HP:%2.0f", mHp);
	mText.DrawString(buf, 100, 0, 8, 16);
	/*
	//Y���W�̕\��
	//������̐ݒ�
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//������̕`��
	mText.DrawString(buf, 100, 30, 8, 16);

	//X����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//������̕`��
	mText.DrawString(buf, 100, 0, 8, 16);
	//X����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "FX:%7.2f", (CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate).GetRotationX(CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate));
	//������̕`��
	mText.DrawString(buf, 100, -20, 8, 16);



	//Y����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RY:%7.2f", mRotation.mY);
	//������̕`��
	mText.DrawString(buf, 100, -100, 8, 16);

	//Y����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "FY:%7.2f", (CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate).GetRotationY());
	//������̕`��
	mText.DrawString(buf, 100, -120, 8, 16);
	*/

	//2D�̕`��I��
	CUtil::End2D();

}