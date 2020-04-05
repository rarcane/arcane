#include "tsfoundation.h"

internal void
LinuxDispatchJob(TsWorkQueueJob *job)
{
	job->DoWork(job->job_data);
	// IMPLEMENT THIS
	// // NOTE(rjf): Get a free thread.
	// Win32WorkerThreadData *thread_data = 0;
	// {
	//     for(u32 i = 0; i < global_win32_worker_thread_max; ++i)
	//     {
	//         if(global_win32_worker_threads[i].status == WIN32_WORKER_THREAD_STATUS_WAITING)
	//         {
	//             thread_data = global_win32_worker_threads + i;
	//             break;
	//         }
	//     }
	// }

	// // NOTE(rjf): Dispatch thread.
	// Win32DispatchJobOnThread(job, thread_data);
}

internal i32
LinuxQueueJob(void *job_data, TsWorkerThreadDoJobWorkCallback *DoWork, TsWorkerThreadJobCompleteCallback *JobComplete)
{
	// IMPLEMENT THIS
	return 0;
}

internal b32
LinuxWaitForJob(i32 index, u32 milliseconds)
{
	// IMPLEMENT THIS
	return 0;
}

internal void
LinuxUpdateWorkQueue(void)
{
	for (i32 i = 0; i < TS_MAX_WORK_QUEUE_JOBS; ++i)
	{
		if (platform->work_queue_jobs[i].status == TS_JOB_STATUS_QUEUED &&
			platform->work_queue_jobs[i].DoWork)
		{
			LinuxDispatchJob(platform->work_queue_jobs + i);
		}
	}
}
