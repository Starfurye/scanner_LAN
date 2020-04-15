#include "common.h"

extern Host hosts[MAX_HOST_NUM];
extern int hostCounter;
extern int portOpenning[65538];

int main(int argc, char* argv[]) {

    int i = 0;
    int stPort = DEFAULT_START_PORT, edPort = DEFAULT_END_PORT;

    
    if (argc == 2) {
        if (strlen(argv[1]) >= 2) {
            char str[7];
            sscanf(argv[1], "%6s", str);
            if (strcmp(str, "-h") == 0) {
                showUsage(1);
            } else if (strcmp(str, "--help") == 0) {
                showUsage(0);
            }
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            char* ch = argv[2];
            while (*ch != ' ') {
                if (*ch == '\0') {
                    break;
                }
                ++ch;
            }
            *ch = '\0';
            startScanning(argv[2], stPort, edPort);
            printOpenningPorts(stPort, edPort);
        }
    } else if (argc == 5) {
        if (strcmp(argv[1], "-i") == 0) {
            stPort = atoi(argv[3]);
            edPort = atoi(argv[4]);
        }
        startScanning(argv[2], stPort, edPort);
        printOpenningPorts(stPort, edPort);
    } else if (argc == 1) {
        getHosts();
        for (i = 0; i < hostCounter; ++i) {
            memset(portOpenning, 0, sizeof(portOpenning));
            startScanning(hosts[i].ip, stPort, edPort);
            printOpenningPorts(stPort, edPort);
        }
        printf("\ntype './scanner -i [ip] [start port] [end port]' to do a specific scan.\n");
    } else {
        showUsage(0);
    }

    return EXIT_SUCCESS;
}
