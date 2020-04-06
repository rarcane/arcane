#include "tsfoundation.h"

internal void
LinuxDispatchJob(TsWorkQueueJob *job)
{
	// IMPLEMENT THIS AND HAVE IT ACTUALLY DISPATCH ON ANOTHER THREAD
	job->DoWork(job->job_data);
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
	i32 job_index = -1;
	// NOTE(rjf): Add job to queue.
	{
		job_index = global_platform.work_queue_free_indices[0];
		if (global_platform.work_queue_free_index_count > 1)
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

	return job_index;
}

internal b32
LinuxWaitForJob(i32 index, u32 milliseconds)
{
	// IMPLEMENT THIS
	global_platform.work_queue_jobs[index].DoWork(global_platform.work_queue_jobs[index].job_data);
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
