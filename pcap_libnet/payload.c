#include "stdio.h"
#include "stdio.h"

typedef int bool;
#define true  1
#define false 0

#define DEFAULT_QUERY_STRING_LENGTH 128

int main() {
    char link[] = "www.baidu.com";

    char payload[DEFAULT_QUERY_STRING_LENGTH];

    char *former = link;
    char *latter = link;
    char *result = payload;

    int count = 0;

    while (true) {
        if (*former == '\0') {
            *result++ = (char) (count + '0');
            while (*latter != *former) {
                *result++ = *latter++;
            }
            *result = '0';
            break;
        } else if (*former == '.') {
            *result++ = (char) (count + '0');
            count = 0;
            while (*latter != *former) {
                *result++ = *latter++;
            }
            former++;
            latter++;
            continue;
        }
        count++;
        former++;
    }

    printf("%s", payload);

}
