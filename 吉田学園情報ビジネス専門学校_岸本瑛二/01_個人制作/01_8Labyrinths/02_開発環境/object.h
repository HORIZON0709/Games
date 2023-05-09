//================================================
//
//3D�Q�[��(��)[object.h]
//Author:Kishimoto Eiji
//
//================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//***************************
//�C���N���[�h
//***************************
#include <d3dx9.h>

//***************************
//�I�u�W�F�N�g�N���X�̒�`
//***************************
class CObject
{/* ��{�N���X */
public: /* �񋓌^�̒�` */
	//�I�u�W�F�N�g�̎��
	enum OBJ_TYPE
	{
		NONE = -1,
		PLAYER = 0,	//�v���C���[
		ENEMY,		//�G
		BG,			//�w�i
		MODEL,		//���f��
		ITEM,		//�A�C�e��
		GIMMICK,	//�M�~�b�N
		DOOR,		//�h�A
		POLYGON_2D,	//2D�|���S��
		POLYGON_3D,	//3D�|���S��
		SCORE,		//�X�R�A
		NUMBER,		//�i���o�[
		MAX
	};

	//�`��̗D�揇��
	enum PRIORITY
	{
		PRIO_NONE = -1,
		PRIO_BG = 0,		//�w�i
		PRIO_MESH,			//���b�V��
		PRIO_MODEL,			//���f��
		PRIO_POLYGON_3D,	//3D�|���S��
		PRIO_POLYGON_2D,	//2D�|���S��
		PRIO_MAX
	};

public: /* �ÓI�����o�֐� */
	static void ReleaseAll();	//�S�Ẳ��
	static void UpdateAll();	//�S�Ă̍X�V
	static void DrawAll();		//�S�Ă̕`��

public: /* �R���X�g���N�^�E�f�X�g���N�^ */
	CObject();
	CObject(const PRIORITY &priority);
	virtual ~CObject();

public: /* �������z�֐� */
	virtual HRESULT Init() = 0;	//������
	virtual void Uninit() = 0;	//�I��
	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�`��

public: /* �����o�֐� */
	void SetDeathFlag();	//���S�t���O�̐ݒ�

public:	/* ObjType */
	/*
		�^�C�v�̐ݒ�
		const OBJ_TYPE &type ---> �C�ӂ̃^�C�v
	*/
	void SetObjType(const OBJ_TYPE &type);

	//�^�C�v�̎擾
	OBJ_TYPE GetObjType();

private:
	void Release();	//���

private: /* �ÓI�����o�ϐ� */
	static CObject* m_apTop[PRIORITY::PRIO_MAX];		//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurrent[PRIORITY::PRIO_MAX];	//����(����)�̃I�u�W�F�N�g�̃|�C���^

	static int m_nNumAll;	//�I�u�W�F�N�g�̑���
	
private: /* �����o�ϐ� */
	CObject* m_pPrev;	//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNext;	//���̃I�u�W�F�N�g�ւ̃|�C���^

	CObject::OBJ_TYPE m_objType;	//���

	bool m_bDeath;	//���S�t���O
};

#endif