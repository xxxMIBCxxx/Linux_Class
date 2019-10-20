//*****************************************************************************
// Threadクラス
// ※リンカオプションに「-pthread」を追加すること
//*****************************************************************************
#include "CThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CThread::CThread(const char* pszId)
{
	int			iRet = 0;


	m_strId = "";
	m_bInitFlag = false;
	m_iError = 0;
	m_hThread = NULL;
	
	// クラス名を保持
	if (pszId != NULL)
	{
		m_strId = pszId;
	}

	// ミューテック属性の初期化
	pthread_mutexattr_init(&m_tMutexAttr);
	iRet = pthread_mutexattr_settype(&m_tMutexAttr,PTHREAD_PROCESS_PRIVATE);		// スレッド間の排他
	if (iRet != 0)
	{
		return;
	}

	m_bInitFlag = true;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CThread::~CThread()
{
	// スレッドが停止していないことを考慮
	this->Stop();

	// ミューテックス属性オブジェクトを破棄
	pthread_mutexattr_destroy(&m_tMutexAttr);
}


//-----------------------------------------------------------------------------
// スレッド開始
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Start()
{
	int			iRet = 0;


	// 初期化処理で失敗している場合
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// 既に動作している場合
	if (m_hThread != NULL)
	{
		return CTHREAD_RET_ERROR_AlREADY_STARTED;
	}

	// スレッド開始
	iRet = pthread_create(&m_hThread, NULL,ThreadLauncher, this);
	if (iRet != 0)
	{
		m_iError = errno;
		printf("pthread_create Error. [errno：%d]\n", m_iError);
		printf("-> %s\n", strerror(m_iError));
		return CTHREAD_RET_ERROR_START;
	}

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// スレッド停止
//-----------------------------------------------------------------------------
CTHREAD_RET_ENUM CThread::Stop()
{
	// 初期化処理で失敗している場合
	if (m_bInitFlag == false)
	{
		return CTHREAD_RET_ERROR_INIT;
	}

	// 既に停止している場合
	if (m_hThread == NULL)
	{
		return CTHREAD_RET_SUCCESS;
	}

	// スレッドを停止させる
	pthread_cancel(m_hThread);
	pthread_join(m_hThread, NULL);
	m_hThread = NULL;

	return CTHREAD_RET_SUCCESS;
}


//-----------------------------------------------------------------------------
// スレッド呼び出し
//-----------------------------------------------------------------------------
void* CThread::ThreadLauncher(void* pUserData)
{
	// スレッド処理呼び出し
	reinterpret_cast<CThread*>(pUserData)->ThreadProc();

	return (void *)NULL;
}



//-----------------------------------------------------------------------------
// スレッド処理（※サンプル※）
//-----------------------------------------------------------------------------
void CThread::ThreadProc()
{
	DWORD			dwCount = 1;
	timespec		tTimeSpec;
	timespec		tTimeSpec2;


	printf("-- Thread %s Start --\n", m_strId.c_str());

	// Sleep時間を生成
	tTimeSpec2.tv_sec = 0;
	tTimeSpec2.tv_nsec = (((rand() % 9) + 1) * 1000 * 1000);

	// pthread_testcancelが呼ばれるまで処理を続ける
	while (1)
	{
		// Sleep時間を生成
		tTimeSpec.tv_sec = (rand() % 5);
		tTimeSpec.tv_nsec = (((rand() % 9) + 1) * 1000 * 1000);

		pthread_mutex_lock(&(this->m_hMutex));
		printf("[%s] Count : %lu \n", this->m_strId.c_str(), dwCount++);
		nanosleep(&tTimeSpec, NULL);
		pthread_mutex_unlock(&(this->m_hMutex));

		nanosleep(&tTimeSpec2, NULL);

	}
}










