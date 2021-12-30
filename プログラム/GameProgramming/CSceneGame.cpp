#include "CSceneGame.h"
//OpenGL
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CKey.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CBullet.h"
#include "CTaskManager.h"
//CEnemy�N���X�̃C���N���[�h
#include "CEnemy.h"
#include "CEnemy2.h"
//
#include "CCollisionManager.h"
//
#include "CBillBoard.h"
//
#include "CCamera.h"
//
#include "CUtil.h"

#include"CBlock.h"

void CSceneGame::Init() {
	mText.LoadTexture("FontWhite.tga",1,64);

	mBackGroundMatrix.Translate(0.0f, 0.0f, -500.0f);

	//�O�p�R���C�_�̊m�F
//	mColliderTriangle.Set(NULL, NULL, CVector(-50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, -50.0f));
//	mColliderTriangle2.Set(NULL, NULL, CVector(50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, 50.0f));

	//C5���f���̓ǂݍ���
	mModelC5.Load("c5.obj", "c5.mtl");

	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���̓���
	mModel.Load("f14.obj", "f14.mtl");
	
	mBackGround.Load("sky.obj", "sky.mtl");

	CMatrix matrix;
	matrix.Print();

	//

	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(0.1f, 0.1f, 0.1f);
	//
	mPlayer.mPosition = CVector(0.0f, 0.0f, -3.0f) * mBackGroundMatrix;
	//mPlayer.mPosition = CVector(0.0f, 0.0f, -503.0f);
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f);

	//�G�@�̃C���X�^���X�쐬
	//new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -600.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -630.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));

//	new CEnemy2(CVector(-15.0f, 15.0f, -90.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
//	new CEnemy2(CVector(15.0f, 15.0f, -150.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CEnemy2(CVector(-10.0f, 1.0f, -80.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy2(CVector(10.0f, 1.0f, -80.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CEnemy2(CVector(-35.0f, 1.0f, -50.0f) * mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy2(CVector(35.0f, 1.0f, -50.0f) * mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CEnemy2(CVector(-15.0f, 1.0f, -40.0f) * mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy2(CVector(15.0f, 1.0f, -100.0f) * mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CBlock(CVector(-20.0f,-1.0f, -60.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 10.0f, 3.0f));
	new CBlock(CVector(30.0f, -1.0f, -50.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 10.0f, 3.0f));
	new CBlock(CVector(0.0f, -1.0f, -1.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	new CBlock(CVector(-45.0f, -1.0f,0.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));

	new CBlock(CVector(-30.0f, -1.0f, -20.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	new CBlock(CVector(80.0f, -1.0f, -60.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	new CBlock(CVector(15.0f, -1.0f, -10.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	new CBlock(CVector(-2.0f, -1.0f, -30.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	new CBlock(CVector(10.0f, -1.0f, -45.0f) * mBackGroundMatrix, CVector(), CVector(4.0f, 5.0f, 3.0f));
	//�r���{�[�h�̐���
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�ƍs��͂Ȃ�
	//mColliderMesh.Set(NULL, NULL, &mBackGround);
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGround);
}

void CSceneGame::Update() {

	CTaskManager::Get()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
//�폜	CCollisionManager::Get()->Collision();
	CTaskManager::Get()->TaskCollision();


	if (CKey::Push('L'))
	{
		mEye.mX += 0.1f;
	}
	if (CKey::Push('J'))
	{
		mEye.mX -= 0.1f;
	}
	if (CKey::Push('I'))
	{
		mEye.mZ -= 0.1f;
	}
	if (CKey::Push('K'))
	{
		mEye.mZ += 0.1f;
	}
	if (CKey::Push('O'))
	{
		mEye.mY += 0.1f;
	}
	if (CKey::Push('M'))
	{
		mEye.mY -= 0.1f;
	}

	//���_�̐ݒ�
	//gluLookAt(���_X, ���_Y, ���_Z, ���SX, ���SY, ���SZ, ���X, ���Y, ���Z)
	//gluLookAt(mEye.mX, mEye.mY, mEye.mZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

//	mPlayer.Update();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	/*
					//���_�����߂�
	if (CKey::Push(VK_RIGHT))
	{
		Camera.mRotation.mY += 2.0f;
	}
	if (CKey::Push(VK_LEFT))
	{
		Camera.mRotation.mY -= 2.0f;
	}
	*/
	//	e = CVector(-2.0f, 10.0f, -30.0f) * mPlayer.mMatrix;
	e = CVector(-2.0f, 200.0f, -130.0f) * CMatrix().RotateY(Camera.mRotation.mY) * mPlayer.mMatrix;
	//���Xy��50
	//�����_�����߂�
	c = mPlayer.mPosition;
	//����������߂�
	u = CVector(0.0f, 10.0f, 0.0f) * mPlayer.mMatrixRotate;
	//�J�����̐ݒ�
	//gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();
	//	mPlayer.Render();

	mBackGround.Render(mBackGroundMatrix);

//	mPlayer.bullet.Update();
//	mPlayer.bullet.Render();

	//�^�X�N���X�g�̍폜
	CTaskManager::Get()->Delete();
	CTaskManager::Get()->Render();

#ifdef _DEBUG
	//�R���C�_�̕`��
	CCollisionManager::Get()->Render();
#endif
	//�G�S�łŃN���A
	if (CEnemy2::sCount == 0)
	{
		//2D�̕`��J�n
		CUtil::Start2D(-400, 400, -300, 300);
		//�`��F�̐ݒ�i�ΐF�̔������j
		glColor4f(239.0f / 256.0f, 175.0f / 256.0f, 0.0f, 1.0f);

		//������̕`��
		mText.DrawString("MISSION CLEAR", -200, 100, 16, 32);

		//2D�̕`��I��
		CUtil::End2D();
	}
	//�v���C���[��HP0�ŃQ�[���I�[�o�[
	if (CPlayer::mHp <= 0)
	{
		//2D�̕`��J�n
		CUtil::Start2D(-400, 400, -300, 300);
		//�`��F�̐ݒ�i�ΐF�̔������j
		glColor4f(239.0f / 256.0f, 175.0f / 256.0f, 0.0f, 1.0f);

		//������̕`��
		mText.DrawString("GAME OVER", -200, 100, 16, 32);
		CUtil::End2D();
	}
	//H�L�[���͂ŃX�^�[�g
	if (CPlayer::mStart == 0)
	{
		//2D�̕`��J�n
		CUtil::Start2D(-400, 400, -300, 300);
		//�`��F�̐ݒ�i�ΐF�̔������j
		glColor4f(239.0f / 256.0f, 175.0f / 256.0f, 0.0f, 1.0f);

		//������̕`��
		mText.DrawString("H KEY PUSH START ", -200,250, 16, 16);
		CUtil::End2D();
	}

}

