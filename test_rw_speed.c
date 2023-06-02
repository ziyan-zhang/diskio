/*
 * @Descripttion: https://blog.csdn.net/weixin_54891898/article/details/120933822
 * @Author: 张子岩 Ziyan ZHANG
 * @version: 
 * @Date: 2023-06-02 10:52:58
 * @LastEditors: 张子岩 Ziyan ZHANG
 * @LastEditTime: 2023-06-02 16:15:48
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/time.h>

#define MB 1048576

int main(int argc, char* argv[])
{
    char filename[] = "myfile05";
    // 100MB
    char* buffer = (char*)malloc(sizeof(char) * 100 * MB);
    printf("%ld", sizeof(char) * 100 * MB);
    buffer[MB] = '\0';
    size_t bufSize = 100 * MB * 5;  // todo: 乘5是干啥？

    struct timeval startWriteTime, endWriteTime;

    gettimeofday(&startWriteTime, NULL);

    int fd;
    fd = open(filename, O_RDWR | O_CREAT, 0777);
    if (fd < 0)
    {
        perror("Unable to creat file");
        return -1;
    }

    size_t sizeWrited = write(fd, (void*)buffer, bufSize);
    printf("write: %ld bytes to %s\n", sizeWrited, filename);
    gettimeofday(&endWriteTime, NULL);
    close(fd);
    // 微秒为单位的写入时间
    double writeTimeElapse = (double)(endWriteTime.tv_sec - startWriteTime.tv_sec) * 1000.0 + \
        (double)(endWriteTime.tv_usec - startWriteTime.tv_usec) / 1000.0;
    // MB每秒
    double Vol_Write = sizeWrited * 1.0f * 1000 / writeTimeElapse / MB;
    printf(" %lfMB/s\n", Vol_Write);

    fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        perror("Unable to open file");
        return -1;
    }

    struct timeval readStime, readEtime;
    gettimeofday(&readStime, NULL);
    size_t sizeRead = read(fd, (void*)buffer, bufSize);
    gettimeofday(&readEtime, NULL);
    printf("Read:%ld bytes from %s, read content:%s\n", sizeWrited, filename, buffer);
    double ReadTime = (double)(readEtime.tv_sec - readStime.tv_sec) * 1000.0 \
        + (double)(readEtime.tv_usec - readStime.tv_usec) / 1000.0;
    double Vol_read = sizeRead * 1.0f * 1000 / ReadTime / MB;
    printf("%lfMB/s\n", Vol_read);
    return 0;
}