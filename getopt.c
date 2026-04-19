#ifdef _WIN32

#include <stdio.h>
#include <string.h>

extern int optind = 1;
extern char *optarg = NULL;
extern int optopt = 0;
int opterr = 1;

int getopt(int argc, char *const argv[], const char *optstring)
{
    static int charidx = 0;
    int option;
    
    if (charidx == 0 || charidx == 1) {
        if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '-') {
            return -1;
        }
        charidx++;
        option = argv[optind][charidx];
        if (option == ':' || option == '?') {
            optopt = option;
            if (opterr) {
                fprintf(stderr, "illegal option -- %c\n", option);
            }
            optind++;
            charidx = 0;
            return '?';
        }
        option = argv[optind][charidx];
        optopt = option;
        char *s = strchr(optstring, option);
        if (s == NULL) {
            if (opterr) {
                fprintf(stderr, "illegal option -- %c\n", option);
            }
            optind++;
            charidx = 0;
            return '?';
        }
        if (s[1] == ':') {
            if (optind + 1 < argc && argv[optind + 1][0] != '-') {
                optarg = argv[optind + 1];
                optind += 2;
            } else {
                optarg = NULL;
                if (opterr) {
                    fprintf(stderr, "option requires an argument -- %c\n", option);
                }
                optind++;
                charidx = 0;
                return '?';
            }
        } else {
            optind++;
        }
        charidx = 0;
        return option;
    }
    return -1;
}

#endif