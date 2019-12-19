#ifndef CTHREAD
#define CTHREAD

#include <pthread.h>

class CThread

{

protected:
    pthread_t mThread;

    bool threadRunFlag;

public:
    CThread();

    virtual ~CThread();

    virtual void run() = 0;

    static void *run_(void *);

    void Start();

    void Stop();

    bool IsRun();
};

CThread::CThread()

{

    this->threadRunFlag = false;
}

CThread::~CThread()

{

    if (this->IsRun())

    {

        this->Stop();
    }
}

bool CThread::IsRun()

{

    return this->threadRunFlag;
}

void *CThread::run_(void *aThis)

{

    CThread *pThis = (CThread *)aThis;

    while (pThis->IsRun())

    {

        pThis->run();
    }

    return 0;
}

void CThread::Start()

{

    this->threadRunFlag = true;

    pthread_create(&mThread, NULL, &CThread::run_, (void *)this);
}

void CThread::Stop()

{

    this->threadRunFlag = false;

    pthread_join(mThread, NULL);
}

#endif // CTHREAD

