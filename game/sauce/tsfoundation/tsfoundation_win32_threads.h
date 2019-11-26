/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

typedef enum Win32WorkerThreadStatus Win32WorkerThreadStatus;
enum Win32WorkerThreadStatus
{
    WIN32_WORKER_THREAD_STATUS_WAITING,
    WIN32_WORKER_THREAD_STATUS_WORKING,
    WIN32_WORKER_THREAD_STATUS_DONE,
};

typedef struct Win32WorkerThreadData Win32WorkerThreadData;
struct Win32WorkerThreadData
{
    volatile i32 status;
    HANDLE thread_handle;
    DWORD thread_id;
    HANDLE job_semaphore;
    TsWorkerThreadDoJobWorkCallback *DoWork;
    void *job_data;
};

internal void Win32WorkerThreadDataInit(Win32WorkerThreadData *thread_data);
internal DWORD Win32WorkerThreadDoWork(void *param);
internal b32 Win32WaitForJob(i32 index, u32 milliseconds);
internal i32 Win32QueueJob(void *job_data, TsWorkerThreadDoJobWorkCallback *DoWork, TsWorkerThreadJobCompleteCallback *JobComplete);