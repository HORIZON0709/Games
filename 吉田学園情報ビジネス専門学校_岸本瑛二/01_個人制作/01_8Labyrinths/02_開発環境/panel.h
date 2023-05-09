//================================================
//
//3D�Q�[��(��)[panel.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _PANEL_H_
#define _PANEL_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

#include "stage.h"

//***************************
//�O���錾
//***************************
class CObject2D;
class CObjectX;

//***************************
//�v���C���[�N���X�̒�`
//***************************
class CPanel
{/* ��{�N���X */
public: /* �萔�̒�` */
	static const int GRID_X = 3;	//�}�X�ڂ̐�(X��)
	static const int GRID_Y = 3;	//�}�X�ڂ̐�(Y��)
private:
	static const int MAX_PANEL = 9;	//�p�l���̍ő吔

	static const float PANEL_SIZE;	//�p�l���̃T�C�Y

	static const D3DXVECTOR3 PANELSTAND_POS;	//�p�l���X�^���h�̈ʒu

public: /* �\���̂̒�` */
	struct PANEL_INFO	//�p�l�����
	{
		CObject2D* m_pPanel;	//�p�l���̃|�C���^
		CStage::STAGE stage;	//�X�e�[�W
	};

public: /* �ÓI�����o�֐� */
	static CPanel* Create();	//����

private: /* �ÓI�����o�ϐ� */
	static PANEL_INFO m_aPanelInfo[GRID_Y][GRID_X];	//�p�l�����

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CPanel();
	~CPanel();

public: /* �����o�֐� */
	HRESULT Init();	//������
	void Uninit();	//�I��
	void Update();	//�X�V
	void Draw();	//�`��

	bool GetIsPanel();	//�p�l�����쒆���ǂ������擾

	/*
		�p�l�����̎擾
		int Y ---> �ʒu(X��)
		int X ---> �ʒu(Y��)
	*/
	PANEL_INFO GetPanelInfo(int Y, int X);

	CObjectX* GetPanelStand();	//�p�l���X�^���h�̎擾

	/*
		�v���C���[���������Ă��邩�ǂ�����ݒ�
		bool bCollPlayer ---> �v���C���[���������Ă��邩�ǂ���
	*/
	void SetCollPlayer(bool bCollPlayer);

private:
	void SelectPanel();		//�p�l���̑I��
	void MoveSelect();		//�I��p�p�l���̈ړ�
	void SetSelectColor();	//�I��p�p�l���̐F�̐ݒ�
	void MovePanel();		//�p�l���̈ړ�

private: /* �����o�ϐ� */
	CObjectX* m_pPanelStand;	//�p�l���X�^���h�̃|�C���^

	CObject2D* m_pBg;		//�w�i
	CObject2D* m_pSelect;	//�I��p�p�l��

	D3DXVECTOR3 m_aPos[GRID_Y][GRID_X];	//�p�l���̈ʒu

	int m_nPosX;	//�ʒu(X��)
	int m_nPosY;	//�ʒu(Y��)

	bool m_bCollPlayer;	//�v���C���[���������Ă��邩�ǂ���
	bool m_bIsPanel;	//�p�l�����쒆���ǂ���
	bool m_bIsSelect;	//�I�𒆂��ǂ���
};

#endif