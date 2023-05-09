//================================================
//
//3D�Q�[��(��)[object.cpp]
//Author:Kishimoto Eiji
//
//================================================
//***************************
//�C���N���[�h
//***************************
#include "object.h"
#include "renderer.h"

//***************************
//�ÓI�����o�ϐ�
//***************************
CObject* CObject::m_apTop[PRIORITY::PRIO_MAX] = {};		//�擪�̃I�u�W�F�N�g�̃|�C���^
CObject* CObject::m_apCurrent[PRIORITY::PRIO_MAX] = {};	//����(����)�̃I�u�W�F�N�g�̃|�C���^

int CObject::m_nNumAll = 0;	//�ő吔

//================================================
//�S�Ẳ��
//================================================
void CObject::ReleaseAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//�擪�̃I�u�W�F�N�g��ۑ�
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObj��nullptr�ɂȂ�܂�
			//�u�ۑ������I�u�W�F�N�g�v�́u���̃I�u�W�F�N�g�v��ۑ�
			CObject* pNext = pObject->m_pNext;

			//���
			pObject->Release();

			//�u���̃I�u�W�F�N�g�v�ɕύX
			pObject = pNext;
		}

		//�擪�̃I�u�W�F�N�g��ۑ�
		pObject = m_apTop[i];

		while (pObject)
		{
			//�u�ۑ������I�u�W�F�N�g�v�́u���̃I�u�W�F�N�g�v��ۑ�
			CObject* pNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{//���S�t���O�������Ă���ꍇ
				//�I��
				pObject->Uninit();
			}

			//�u���̃I�u�W�F�N�g�v�ɕύX
			pObject = pNext;
		}
	}
}

//================================================
//�S�Ă̍X�V
//================================================
void CObject::UpdateAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//�擪�̃I�u�W�F�N�g��ۑ�
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObj��nullptr�ɂȂ�܂�
			//�u�ۑ������I�u�W�F�N�g�v�́u���̃I�u�W�F�N�g�v��ۑ�
			CObject* pNext = pObject->m_pNext;

			//�X�V
			pObject->Update();

			//�u���̃I�u�W�F�N�g�v�ɕύX
			pObject = pNext;
		}

		//�擪�̃I�u�W�F�N�g��ۑ�
		pObject = m_apTop[i];

		while (pObject)
		{
			//�u�ۑ������I�u�W�F�N�g�v�́u���̃I�u�W�F�N�g�v��ۑ�
			CObject* pNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{//���S�t���O�������Ă���ꍇ
				//�I��
				pObject->Uninit();

				//���
				pObject->Release();
			}

			//�u���̃I�u�W�F�N�g�v�ɕύX
			pObject = pNext;
		}
	}
}

//================================================
//�S�Ă̕`��
//================================================
void CObject::DrawAll()
{
	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		//�擪�̃I�u�W�F�N�g��ۑ�
		CObject* pObject = m_apTop[i];

		while (pObject)
		{//pObj��nullptr�ɂȂ�܂�
			//�u�ۑ������I�u�W�F�N�g�v�́u���̃I�u�W�F�N�g�v��ۑ�
			CObject* pNext = pObject->m_pNext;

			//�`��
			pObject->Draw();

			//�u���̃I�u�W�F�N�g�v�ɕύX
			pObject = pNext;
		}
	}
}
//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CObject::CObject()
{
}

//================================================
//�R���X�g���N�^
//================================================
CObject::CObject(const PRIORITY &priority)
{
	if (m_apTop[priority] == nullptr)
	{//�I�u�W�F�N�g����������ꍇ
		//���g��擪�Ƃ��ēo�^
		m_apTop[priority] = this;

		//�����Ɏ��g(�擪)������
		m_apCurrent[priority] = m_apTop[priority];

		//����������₷
		m_nNumAll++;
		return;
	}

	/* �I�u�W�F�N�g��1�ȏ゠��ꍇ */

	//�����̎��Ɏ��g������
	m_apCurrent[priority]->m_pNext = this;

	//���g����Ԍ��ɂȂ�
	this->m_pPrev = m_apCurrent[priority];

	//�����Ɏ��g������
	m_apCurrent[priority] = this;

	//����������₷
	m_nNumAll++;
	return;
}

//================================================
//�f�X�g���N�^
//================================================
CObject::~CObject()
{
}

//================================================
//���
//================================================
void CObject::Release()
{
	if (this == nullptr)
	{//NULL�`�F�b�N
		return;
	}

	/* nullptr�ł͂Ȃ��ꍇ */

	for (int i = 0; i < PRIORITY::PRIO_MAX; i++)
	{
		if (this->m_pPrev != nullptr)
		{//���g�̑O�����݂��Ă���ꍇ
			//�u���g�̑O�����A���̏��v�ɁA�u���g�̎��̏��v��ڑ�
			this->m_pPrev->m_pNext = this->m_pNext;
		}

		if (this->m_pNext != nullptr)
		{//���g�̎������݂��Ă���ꍇ
			//�u���g�̎������A�O�̏��v�ɁA�u���g�̑O�̏��v��ڑ�
			this->m_pNext->m_pPrev = this->m_pPrev;
		}

		if (this->m_pPrev == nullptr)
		{//���g�̑O�ɉ��������ꍇ(���g���擪)
			//�擪�ɁA���g�̎��̏���ݒ�
			m_apTop[i] = this->m_pNext;
		}

		if (this->m_pNext == nullptr)
		{//���g�̎��ɉ��������ꍇ(���g������)
			//�����ɁA���g�̑O�̏���ݒ�
			m_apCurrent[i] = this->m_pPrev;
		}
	}

	//����������炷
	m_nNumAll--;
}

//================================================
//���S�t���O�̐ݒ�
//================================================
void CObject::SetDeathFlag()
{
	//���S�t���O�𗧂Ă�
	this->m_bDeath = true;
}

//================================================
//�^�C�v�̐ݒ�
//================================================
void CObject::SetObjType(const CObject::OBJ_TYPE &type)
{
	m_objType = type;
}

//================================================
//�^�C�v�̎擾
//================================================
CObject::OBJ_TYPE CObject::GetObjType()
{
	return m_objType;
}