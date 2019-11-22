
typedef enum LinuxWorkerThreadStatus LinuxWorkerThreadStatus;
enum LinuxWorkerThreadStatus
{
    LINUX_WORKER_THREAD_STATUS_WAITING,
    LINUX_WORKER_THREAD_STATUS_WORKING,
    LINUX_WORKER_THREAD_STATUS_DONE,
};

typedef struct LinuxWorkerThreadData LinuxWorkerThreadData;
struct LinuxWorkerThreadData
{
    volatile i32 status;
    pthread_t thread_handle;
    pthread_mutex_t job_semaphore;
    TsWorkerThreadDoJobWorkCallback *DoWork;
    void *job_data;
};

internal void LinuxWorkerThreadDataInit(LinuxWorkerThreadData *thread_data);
internal void *LinuxWorkerThreadDoWork(void *param);
internal b32 LinuxWaitForJob(i32 index, u32 milliseconds);
internal i32 LinuxQueueJob(void *job_data, TsWorkerThreadDoJobWorkCallback *DoWork, TsWorkerThreadJobCompleteCallback *JobComplete);