/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

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