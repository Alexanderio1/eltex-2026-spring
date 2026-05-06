#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define ERROR_IP 0

uint32_t matchIP(char* ip) {
    int a, b, c, d;
    char extra;

    if (sscanf(ip, "%d.%d.%d.%d%c", &a, &b, &c, &d, &extra) != 4)
        return ERROR_IP;

    if (a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255)
        return ERROR_IP;

    return ((uint32_t)a << 24) |
           ((uint32_t)b << 16) |
           ((uint32_t)c << 8)  |
           (uint32_t)d;
}

uint32_t generateIP() {
    uint32_t random_ip = 0;

    for (int i = 0; i < 4; i++) {
        random_ip = (random_ip << 8) | (rand() % 256);
    }

    return random_ip;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        return -1;
    }

    uint32_t gw_ip_addr = matchIP(argv[1]);
    uint32_t mask_ip_addr = matchIP(argv[2]);

    if (gw_ip_addr == ERROR_IP || mask_ip_addr == ERROR_IP) {
        return -1;
    }

    uint64_t N = (uint64_t)atoll(argv[3]);

    if (N == 0) {
        return -1;
    }

    uint32_t gw_network = gw_ip_addr & mask_ip_addr;

    uint64_t local_count = 0;
    uint64_t foreign_count;

    srand((unsigned int)time(NULL));

    for (uint64_t i = 0; i < N; i++) {
        uint32_t random_ip_addr = generateIP();
        uint32_t random_network = random_ip_addr & mask_ip_addr;

        if (random_network == gw_network) {
            local_count++;
        }
    }

    foreign_count = N - local_count;

    double local_percent = 100.0 * local_count / N;
    double foreign_percent = 100.0 * foreign_count / N;

    printf("Обработано пакетов: %llu\n", (unsigned long long)N);
    printf("Своей подсети: %llu (%.2f%%)\n",
           (unsigned long long)local_count, local_percent);
    printf("Других сетей:  %llu (%.2f%%)\n",
           (unsigned long long)foreign_count, foreign_percent);

    return 0;
}
