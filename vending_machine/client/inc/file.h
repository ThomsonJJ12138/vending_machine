#ifndef     __FILE_H
#define     __FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/input.h>




class File
{
public:
    File();
    ~File();
    int file_open(const char* filePath);

private:

};




#endif