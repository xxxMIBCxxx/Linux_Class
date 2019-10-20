//*****************************************************************************
// Thread�N���X
// �������J�I�v�V�����Ɂu-pthread�v��ǉ����邱��
//*****************************************************************************
#include "CThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CThread::CThread(const char* pszId)
{
	int			iRet = 0;


	m_strId = "";
	m_bInitFlag = false;
	m_iError = 0;
	m_hThread = NULL;
	
	// �N���X����ێ�
	if (pszId != NULL)
	{
		m_strId = pszId;
	}

	// �~���[�e�b�N�����̏�����
	pthread_mutexattr_init(&m_tMutexAttr);
	iRet = pthread_mutexattr_settype(&m_tMutexAttr,PTHREAD_PROCESS_PRIVATE);		// �X���b�h�Ԃ̔r��
	if (iRet != 0)
	{
		return;
	}

	m_bInitFlag = true;
}


//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CThread::~CThread()
{
	// �X���b�h����~���Ă��Ȃ����Ƃ��l��
	this->Stop();

	// �~���[�e�b�N�X�����I�u�W�F�N�g��j��
	pthread_mutexattr_destroy(&m_tMutexAttr);
}


//-----------------------------------------------------------------------------
// �X���b�h�J�n
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Start()
{
	int			iRet = 0;


	// �����������Ŏ��s���Ă���ꍇ
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// ���ɓ��삵�Ă���ꍇ
	if (m_hThread != NULL)
	{
		return CTHREAD_RET_ERROR_AlREADY_STARTED;
	}

	// �X���b�h�J�n
	iRet = pthread_create(&m_hThread, NULL,ThreadLauncher, this);
	if (iRet != 0)
	{
		m_iError = errno;
		printf("pthread_create Error. [errno�F%d]\n", m_iError);
		printf("-> %s\n", strerror(m_iError));
		return CTHREAD_RET_ERROR_START;
	}

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// �X���b�h��~
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Stop()
{
	// �����������Ŏ��s���Ă���ꍇ
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// ���ɒ�~���Ă���ꍇ
	if (m_hThread == NULL)
	{
		return CTHREAD_RET_SUCCESS;
	}

	// �X���b�h���~������
	pthread_cancel(m_hThread);
	pthread_join(m_hThread, NULL);
	m_hThread = NULL;

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// �X���b�h�Ăяo��
//-----------------------------------------------------------------------------
void* CThread::ThreadLauncher(void* pUserData)
{
	// �X���b�h�����Ăяo��
	reinterpret_cast<CThread*>(pUserData)->ThreadProc();

	return (void *)NULL;
}



//-----------------------------------------------------------------------------
// �X���b�h�����i���T���v�����j
//-----------------------------------------------------------------------------
void CThread::ThreadProc()
{
	DWORD			dwCount = 1;
	timespec		tTimeSpec;
	timespec		tTimeSpec2;


	printf("-- Thread %s Start --\n", m_strId.c_str());

	// Sleep���Ԃ𐶐�
	tTimeSpec2.tv_sec = 0;
	tTimeSpec2.tv_nsec = (((rand() % 9) + 1) * 1000 * 1000);

	// pthread_testcancel���Ă΂��܂ŏ����𑱂���
	while (1)
	{
		// Sleep���Ԃ𐶐�
		tTimeSpec.tv_sec = (rand() % 5);
		tTimeSpec.tv_nsec = (((rand() % 9) + 1) * 1000 * 1000);

		pthread_mutex_lock(&(this->m_hMutex));
		printf("[%s] Count : %lu \n", this->m_strId.c_str(), dwCount++);
		nanosleep(&tTimeSpec, NULL);
		pthread_mutex_unlock(&(this->m_hMutex));

		nanosleep(&tTimeSpec2, NULL);

	}
}










