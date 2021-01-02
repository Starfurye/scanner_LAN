#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

#include <netinet/in.h>

#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <assert.h>

#include <arpa/inet.h>

#define MAX_HOST_NUM 254
#define BUFFER_SIZE 8192
#define DEFAULT_THREAD_NUM 8
#define DEFAULT_START_PORT 1
#define DEFAULT_END_PORT 1024
#define THREADS_NUM 8

#define CLRSCREEN() printf("\033[H\033[2J")

typedef struct {
    char ip[40];
} Host;
typedef struct {
    int pthreadNum;
    char* ip;
    int stp;
    int edp;
} Threads, *Threadsp;

void showUsage(int isSimple);
int getHosts();
int startScanning(char* ip, int stPort, int edPort);
void* threadChange(void* threadp);
int tcpScan(char* ip, int port);
void printOpenningPorts(int st, int ed);

#endif // !__COMMON_H__
