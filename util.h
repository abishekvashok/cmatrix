static inline int max(int a, int b) {
    return (a > b) ? a : b;
}

static inline int min(int a, int b) {
    return (a < b ) ? a : b;
}

void cleanup();

void c_die(const char *msg, ...) {
    cleanup();

    va_list ap;
    fprintf(stderr, "cmatrix: error: ");
    va_start(ap, msg);
    vfprintf(stderr, _(msg), ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

/* nmalloc from nano by Big Gaute */
void *nmalloc(size_t howmuch) {
    static const char* NOMEM_MSG = _("malloc(…): out of memory!");

    void *r;

    if (!(r = malloc(howmuch))) {
        c_die(NOMEM_MSG);
    }

    return r;
}

int va_system(char *str, ...) {
    va_list ap;
    static char buf[133];

    va_start(ap, str);
    vsnprintf(buf, sizeof(buf), str, ap);
    va_end(ap);
    return system(buf);
}

void cmd_or_die(char* cmd_name, char* cmd) {
    if (va_system(cmd) != 0) {
        static const char* NOPROGRAM_MSG = 
             "There was an error running %s. Please make sure the\n"
             "consolechars program is in your $PATH. Try running \"%s\" by hand.\n";
        c_die(NOPROGRAM_MSG, cmd_name, cmd);
    }
}
