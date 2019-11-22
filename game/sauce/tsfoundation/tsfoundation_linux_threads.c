internal void
LinuxWorkerThreadDataInit(LinuxWorkerThreadData *thread_data)
{
    thread_data->job_semaphore = CreateSemaphoreA(0, 0, 1, 0);
    thread_data->thread_handle = CreateThread(0, 0, Win32WorkerThreadDoWork, thread_data, 0, &thread_data->thread_id);
}

internal DWORD
LinuxWorkerThreadDoWork(void *param)
{
    LinuxWorkerThreadData *thread_data = param;
    while(!global_platform.quit)
    {
        if(thread_data->status != LINUX_WORKER_THREAD_STATUS_DONE &&
           WaitForSingleObject(thread_data->job_semaphore, INFINITE) == WAIT_OBJECT_0)
        {
            if(thread_data->DoWork)
            {
                thread_data->DoWork(thread_data->job_data);
            }
            InterlockedExchange((LONG *)&thread_data->status, (LONG)LINUX_WORKER_THREAD_STATUS_DONE);
            ReleaseSemaphore(thread_data->job_semaphore, 1, 0);
        }
    }
    return 0;
}

internal void
LinuxDispatchJobOnThread(TsWorkQueueJob *job, LinuxWorkerThreadData *thread_data)
{
    if(thread_data && thread_data->status == LINUX_WORKER_THREAD_STATUS_WAITING)
    {
        job->status = TS_JOB_STATUS_WORKING;
        job->thread_index = (i32)(((uintptr_t)thread_data - (uintptr_t)global_linux_worker_threads) / sizeof(global_linux_worker_threads[0]));
        thread_data->DoWork = job->DoWork;
        thread_data->job_data = job->job_data;
        InterlockedExchange((LONG *)&thread_data->status, (LONG)LINUX_WORKER_THREAD_STATUS_WORKING);
        ReleaseSemaphore(thread_data->job_semaphore, 1, 0);
    }
}

internal void
LinuxDispatchJob(TsWorkQueueJob *job)
{
    // NOTE(rjf): Get a free thread.
    LinuxWorkerThreadData *thread_data = 0;
    {
        for(u32 i = 0; i < global_linux_worker_thread_max; ++i)
        {
            if(global_linux_worker_threads[i].status == LINUX_WORKER_THREAD_STATUS_WAITING)
            {
                thread_data = global_linux_worker_threads + i;
                break;
            }
        }
    }
    
    // NOTE(rjf): Dispatch thread.
    LinuxDispatchJobOnThread(job, thread_data);
}

internal b32
LinuxWaitForJob(i32 index, u32 milliseconds)
{
    b32 job_complete = 0;
    i32 thread_index = platform->work_queue_jobs[index].thread_index;
    
    // NOTE(rjf): In this case, we are waiting on a job that needs to be completed,
    // but the job has not been sent to a thread yet. We need to ensure this is
    // dispatched, and wait for it.
    if(thread_index == -1)
    {
        // NOTE(rjf): Try a normal dispatch first.
        LinuxDispatchJob(platform->work_queue_jobs + index);
        thread_index = platform->work_queue_jobs[index].thread_index;
        
        // NOTE(rjf): Normal dispatch didn't work and we are waiting forever 
        // (which implies that this job needs to be done ASAP), so we'll force
        // wait on the first thread.
        if(thread_index == -1 && milliseconds == TS_WAIT_FOREVER)
        {
            LinuxWorkerThreadData *thread_data = global_linux_worker_threads + 0;
            thread_index = 0;
            platform->work_queue_jobs[index].thread_index = thread_index;
            if(WaitForSingleObject(thread_data->job_semaphore, INFINITE) == WAIT_OBJECT_0)
            {
                LinuxDispatchJobOnThread(platform->work_queue_jobs + index, thread_data);
            }
        }
    }
    
    if(thread_index != -1)
    {
        LinuxWorkerThreadData *thread_data = global_linux_worker_threads + thread_index;
        
        if(thread_data->status == LINUX_WORKER_THREAD_STATUS_DONE && WaitForSingleObject(thread_data->job_semaphore, milliseconds == TS_WAIT_FOREVER ? INFINITE : milliseconds) == WAIT_OBJECT_0)
        {
            // NOTE(rjf): Job is complete. Add index to the free index list.
            job_complete = 1;
            platform->work_queue_jobs[index].status = TS_JOB_STATUS_DONE;
            if(platform->work_queue_free_index_count < TS_MAX_WORK_QUEUE_JOBS)
            {
                platform->work_queue_free_indices[platform->work_queue_free_index_count++] = index;
            }
            InterlockedExchange((LONG *)(&thread_data->status), (LONG)LINUX_WORKER_THREAD_STATUS_WAITING);
        }
    }
    
    return job_complete;
}

internal i32
LinuxQueueJob(void *job_data, TsWorkerThreadDoJobWorkCallback *DoWork, TsWorkerThreadJobCompleteCallback *JobComplete)
{
    i32 job_index = -1;
    
    HardAssert(global_platform.work_queue_free_index_count > 0);
    
    // NOTE(rjf): Add job to queue.
    {
        job_index = global_platform.work_queue_free_indices[0];
        if(global_platform.work_queue_free_index_count > 1)
        {
            global_platform.work_queue_free_indices[0] = global_platform.work_queue_free_indices[global_platform.work_queue_free_index_count - 1];
        }
        --global_platform.work_queue_free_index_count;
        
        global_platform.work_queue_jobs[job_index].job_data = job_data;
        global_platform.work_queue_jobs[job_index].DoWork = DoWork;
        global_platform.work_queue_jobs[job_index].JobComplete = JobComplete;
        global_platform.work_queue_jobs[job_index].thread_index = -1;
        global_platform.work_queue_jobs[job_index].status = TS_JOB_STATUS_QUEUED;
    }
    
    LinuxDispatchJob(global_platform.work_queue_jobs + job_index);
    
    return job_index;
}

internal void
LinuxUpdateWorkQueue(void)
{
    for(i32 i = 0; i < TS_MAX_WORK_QUEUE_JOBS; ++i)
    {
        if(platform->work_queue_jobs[i].status == TS_JOB_STATUS_QUEUED &&
           platform->work_queue_jobs[i].DoWork)
        {
            LinuxDispatchJob(platform->work_queue_jobs + i);
        }
    }
}