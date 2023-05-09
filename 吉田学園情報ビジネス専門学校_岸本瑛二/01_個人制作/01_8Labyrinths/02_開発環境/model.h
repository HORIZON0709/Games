//================================================
//
//3D�Q�[��(��)[model.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>
#include <stdio.h>

//***************************
//���f���N���X�̒�`
//***************************
class CModel
{/* ��{�N���X */
private: /* �萔�̒�` */
	static const int MAX_WORD;			//�ő啶����
	static const int MAX_PARTS = 16;	//�ő�p�[�c��
	static const int NUM_PARTS = 5;		//�p�[�c��

	static const char* FILE_NAME;	//�t�@�C����

private: /* �\���̂̒�` */
	struct PARTS_SET	//�p�[�c���
	{
		int nIndex;			//�C���f�b�N�X��
		int nParent;		//�e�p�[�c�ԍ�
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//����
	};

	struct CHARACTER_SET	//�L�����N�^�[���
	{
		int nNumParts;					//�p�[�c��
		PARTS_SET aPartsSet[NUM_PARTS];	//�p�[�c���
	};
public:
	struct MODEL_INFO	//���f���̊e���
	{
		LPD3DXMESH pMesh;		//���b�V�����̃|�C���^
		LPD3DXBUFFER pBuffMat;	//�}�e���A�����̃|�C���^
		DWORD numMat;			//�}�e���A�����̐�
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//����
		D3DXVECTOR3 vtxMax;		//�ő�l
		D3DXVECTOR3 vtxMin;		//�ŏ��l
	};

public: /* �ÓI�����o�֐� */
	static CModel* Create();	//����

	/*
		���f���̊e���̎擾
		int nNum ---> �ԍ�
	*/
	static MODEL_INFO GetModelInfo(int nNum);

	/*
		���f���̈ʒu����ݒ�
		int nNum ---> �ԍ�
		const D3DXVECTOR3 &pos ---> �ʒu
	*/
	static void SetModelInfo_Pos(int nNum, const D3DXVECTOR3 &pos);

	/*
		���f���̌�������ݒ�
		int nNum ---> �ԍ�
		const D3DXVECTOR3 &rot ---> ����
	*/
	static void SetModelInfo_Rot(int nNum, const D3DXVECTOR3 &rot);

public: /* �ÓI�����o�ϐ� */
	static CHARACTER_SET m_characterSet;	//�L�����N�^�[���

	static MODEL_INFO m_aModelInfo[NUM_PARTS];	//���f���̊e���

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CModel();
	~CModel();

public: /* �����o�֐� */
	HRESULT Init();	//������
	void Uninit();	//�I��
	void Update();	//�X�V
	void Draw();	//�`��

	D3DXMATRIX GetMtxWorld();	//���[���h�}�g���b�N�X�̎擾

private:
	void DrawShadow();	//�e�̕`��
	void Load();		//�ǂݍ���

	/*
		�L�����N�^�[�ݒ�
		FILE* pFile ---> �t�@�C���|�C���^
	*/
	void Set_CharacterSet(FILE* pFile);

	/*
		�p�[�c�ݒ�
		FILE* pFile ---> �t�@�C���|�C���^
		PARTS_SET* pParts ---> �p�[�c���̃|�C���^
	*/
	void Set_PartsSet(FILE* pFile, PARTS_SET* pParts);

private: /* �����o�ϐ� */
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	
	int m_nNumModel;	//���f����
};

#endif