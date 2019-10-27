#pragma once
//*****************************************************************************
// Thread�N���X
// �������J�I�v�V�����Ɂu-pthread�v��ǉ����邱��
//*****************************************************************************
#include <cstdio>
#include <pthread.h>
#include <errno.h> 
#include <string.h>
#include "string"
#include "Type.h"
#include "CEvent.h"


typedef enum
{
	CTHREAD_RET_SUCCESS = 0x00000000,					// ����I��
	CTHREAD_RET_ERROR_INIT = 0xE00000001,				// ���������Ɏ��s���Ă���
	CTHREAD_RET_ERROR_AlREADY_STARTED = 0xE00000002,	// ���ɃX���b�h���J�n���Ă���
	CTHREAD_RET_ERROR_START = 0xE00000003,				// �X���b�h�J�n�Ɏ��s���܂���

	CTHREAD_RET_ERROR_SYSTEM = 0xE9999999				// �V�X�e���ُ�
} CTHREAD_RET_ENUM;





class CThread
{
private:
	std::string					m_strId;				// ���ʖ�
	bool						m_bInitFlag;			// �������t���O
	int							m_iError;				// �G���[�ԍ�
	pthread_t					m_hThread;				// �X���b�h�n���h��
	pthread_mutexattr_t			m_tMutexAttr;			// �~���[�e�b�N�X����
	pthread_mutex_t				m_hMutex;				// �~���[�e�b�N�X�n���h��
	CEvent						m_cThreadEndEvent;		// �X���b�h�I���C�x���g

public:
	CThread(const char* pszId = NULL);
	~CThread();
	CTHREAD_RET_ENUM Start();
	CTHREAD_RET_ENUM Stop();

	virtual void ThreadProc();

private:
	static void* ThreadLauncher(void* pUserData);

};

