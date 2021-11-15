#ifndef CFRIENDLY_H
#define CFRIENDLY_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include "CCollider.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CFriendly : public CCharacter {
public:
	int mFireCount;

	CVector mPoint;	//�ڕW�n�_

	int mHp;	//�q�b�g�|�C���g

	CCharacter* mpEnemy;	//�G�l�~�[�̃|�C���^
	CCharacter* mpFriendly;
	//���f���f�[�^
	static CModel mModel;

	//�R���C�_
	CCollider mCollider;
	CCollider mColSearch;	//�T�[�`�p�R���C�_

	//�R���X�g���N�^
	CFriendly();
	//CFriendly(�ʒu, ��], �g�k)
	CFriendly(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collision(�R���C�_1, �R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	void TaskCollision();
};

#endif
