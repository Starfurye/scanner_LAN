#include "common.h"

Host hosts[MAX_HOST_NUM];
int hostCounter = 0;
int portOpenning[65538] = {0};

void showUsage(int isSimple) {
    printf("\nA simple scanner on LAN.\n\n");
    printf("Usage: ./scanner [OPTION] [ip] <start port> <end port>\n");
    printf("or just type: './scanner' to scan all hosts on LAN.\n");
    if (isSimple == 1) {
        printf("Try './scanner --help' for more information.\n");
    } else if (isSimple == 0) {
        printf("\nExample:\n");  
        printf("  ./scanner -i 192.168.0.102 1 2048 # Scan host that ip address is 192.168.0.102 from port 1 to 2048.\n");
        printf("  ./scanner -i 192.168.0.102        # Scan host that ip address is 192.168.0.102 on ALL ports.");
        printf("\n\n Options:\n\n");
        printf("     -h      show simple usage\n");
        printf("     -i      choose an host with ip address\n");
        printf("     --help  show detailed usage\n");
    }
}

void getHosts() {
    FILE *hostsLog;
    char ahost[BUFFER_SIZE];
    int i = 0;

    printf("\nSearching LAN hosts...\n");
    system("fping -g 192.168.1.0/24 -aq > hosts.log");
    hostsLog = fopen("./hosts.log", "r");
    if (hostsLog == NULL) {
        printf("opening log file failed, please try again\n");
        assert(0);
    }
    CLRSCREEN();
    while (fgets(ahost, BUFFER_SIZE, hostsLog)) {
        if (ahost[strlen(ahost) - 1] == '\n') ahost[strlen(ahost) - 1] = '\0';
        strcpy(hosts[hostCounter].ip, ahost);
        ++hostCounter;
    }
    fclose(hostsLog);

    printf("Host list:\n");
    for(i = 0; i < hostCounter; ++i) {
        printf("%s  ", hosts[i].ip);
        if (i % 9 == 0 && i != 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int startScanning(char* ip, int stPort, int edPort) {

    printf("\n\nHost IP: %s\n", ip);
    int i = 0, pthreadNum = THREADS_NUM;
    pthread_t *pthreads;
    Threadsp thp;

    pthreads = (pthread_t*)malloc(sizeof(pthread_t) * pthreadNum);
    thp = (Threadsp)malloc(sizeof(Threads) * THREADS_NUM);

    for (i = 0; i < pthreadNum; ++i) {
        memset(&thp[i], 0, sizeof(thp[i]));
        thp[i].pthreadNum = pthreadNum;
        thp[i].ip = ip;
        thp[i].stp = stPort + i;
        thp[i].edp = edPort;

        if (pthread_create(&pthreads[i], NULL, threadChange, (void*)&thp[i]) == -1) {
            return 0;
        }
    }

    sleep(1);

    for (i = 0; i < pthreadNum; ++i) {
        pthread_join(pthreads[i], NULL);
    }

    free(pthreads);
    free(thp);

    return 0;
}

void* threadChange(void* threadp) {
    Threadsp thrp;
    thrp = (Threadsp)threadp;

    while (thrp->stp <= thrp->edp) {
        if (tcpScan(thrp->ip, thrp->stp)) {
            portOpenning[thrp->stp] = 1;
        }
        thrp->stp += thrp->pthreadNum;
    }
}

int tcpScan(char* ip, int port) {
    struct sockaddr_in saddr;
    struct timeval timeout;
    int sd, fcntlStat, result;
    fd_set fdr, fdw;

    memset(&saddr, 0, sizeof(saddr));
    memset(&timeout, 0, sizeof(timeout));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(port);

    // set socket to nonblock
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
        return 0;
    }
    fcntlStat = fcntl(sd, F_GETFL, 0);
    if (fcntlStat < 0) {
        close(sd);
        return 0;
    }
    fcntlStat |= O_NONBLOCK;
    if (fcntl(sd, F_SETFL, fcntlStat) < 0) {
        close(sd);
        return 0;
    }

    result = connect(sd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (result != 0) {
        if (errno == EINPROGRESS) {
            FD_ZERO(&fdr);
            FD_ZERO(&fdw);
            FD_SET(sd, &fdr);
            FD_SET(sd, &fdw);

            // set timeout to 1s
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            result = select(sd + 1, &fdr, &fdw, NULL, &timeout);

            if (result <= 0 || result == 2) {
                close(sd);
                return 0;
            }
            if (result == 1 && FD_ISSET(sd, &fdw)) {
                close(sd);
                return 1;
            }
            close(sd);
            return 0;
        }
    }
}

void printOpenningPorts(int st, int ed) {
    int i = st;
    int counter = 0;
    printf("Opening Ports:\n");
    for (; i <= ed; ++i) {
        if (portOpenning[i] == 1) {
            printf("%8d", i);
            ++counter;
        }
        if (counter % 15 == 0 && counter != 0) printf("\n");
    }
    printf("\n%d in %d port(s)\n", counter, ed - st + 1);
}
