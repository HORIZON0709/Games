//================================================
//
//3D�Q�[��(��)[door.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _DOOR_H_
#define _DOOR_H_

//***************************
//�C���N���[�h
//***************************
#include "objectX.h"
#include "stage.h"

//***************************
//�O���錾
//***************************
class CLine;

//***************************
//�v���C���[�N���X�̒�`
//***************************
class CDoor : public CObjectX
{/* CObjectX�̔h���N���X */
private: /* �萔�̒�` */
	static const int MAX_LINE = 12;	//���C���̍ő吔

public: /* �ÓI�����o�֐� */
	/*
		����
		char* pFileName ---> �t�@�C����
	*/
	static CDoor* Create(char* pFileName);

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CDoor();
	~CDoor() override;

public: /* �I�[�o�[���C�h�֐� */
	HRESULT Init() override;	//������
	void Uninit() override;		//�I��
	void Update() override;		//�X�V
	void Draw() override;		//�`��

public: /* �����o�֐� */
	/*
		�����̐ݒ�
		CStage::DIRECTION dir ---> ����
	*/
	void SetDir(CStage::DIRECTION dir);

	CStage::DIRECTION GetDir();	//�����̎擾

private:
	void SetLines();	//���C���̐ݒ�܂Ƃ�

private: /* �����o�ϐ� */
	CLine* m_apLine[MAX_LINE];	//���C���̃|�C���^

	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X

	CStage::DIRECTION m_dir;	//����
};

#endif