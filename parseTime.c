#include <stdio.h>
#include <string.h>
#include <time.h>

void parseTime(char * sec) {
    struct tm tm;
    char buf[25];

    memset(&tm, 0, sizeof(struct tm));
    strptime(sec, "%s", &tm);
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", &tm);
    puts(buf);
}

int main(int argc, char *argv[]) {
    char secString[0];
    
    // for loop i++
    sprintf(secString, "%d", 1679046032);
    parseTime(secString);
    // }

    return 0;
}
