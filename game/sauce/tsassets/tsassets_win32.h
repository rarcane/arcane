struct TsAssetsWatchDirectory
{
    char *path;
    HANDLE directory_handle;
    
    union
    {
        FILE_NOTIFY_INFORMATION file_notify_information;
        unsigned char data[sizeof(FILE_NOTIFY_INFORMATION) + MAX_PATH*2];
    };
    
    DWORD bytes_returned;
    HANDLE overlapped_io_event;
    OVERLAPPED overlapped_data;
};