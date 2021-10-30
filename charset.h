enum CHARSET {
    CHARSET_INVALID = -2,
    CHARSET_USERDEFINED = -1,
    CHARSET_DEFAULT = 0, CHARSET_ASCII = 0,
    CHARSET_KATAKANA, CHARSET_SYMBOLS, CHARSET_LAMBDA, CHARSET_FONT,
    CHARSET_MAX
};

//                                                DEFAULT/ASCII KATAKANA           SYMBOLS          LAMBDA          FONT
//                                                (!    {)      (ｦ       ﾝ)        (、     〾)      (λ      λ)      (¦    Ù)
static const uint32_t CHARSETS[CHARSET_MAX][2] = {{0x21, 0x7B}, {0xFF66, 0xFF9D}, {0x3001, 0x303E}, {0x3BB, 0x3BB}, {0xA6, 0xD9}};

char* codepoint_to_str(uint32_t ch) {
    short len = 1;
    if (ch > 0x80) len++;
    if (ch > 0x800) len++;
    if (ch > 0x10000) len++;
    if (ch > 0x110000) return NULL;

    char* dest = calloc(len+1, sizeof(char));

    switch (len) {
        case 1:
            dest[0] = (char)ch;
            break;
        case 2:
            dest[0] = (ch>>6) | 0xC0;
            dest[1] = (ch & 0x3F) | 0x80;
            break;
        case 3:
            dest[0] = (ch>>12) | 0xE0;
            dest[1] = ((ch>>6) & 0x3F) | 0x80;
            dest[2] = (ch & 0x3F) | 0x80;
            break;
        case 4:
            dest[0] = (ch>>18) | 0xF0;
            dest[1] = ((ch>>12) & 0x3F) | 0x80;
            dest[2] = ((ch>>6) & 0x3F) | 0x80;
            dest[3] = (ch & 0x3F) | 0x80;
            break;
    }

    return dest;
}

char** expand_charset(char* charset) {
    char** ret = calloc(strlen(charset)+1, sizeof(char**));
    char** pt = ret;
    char* tok = strtok(charset, " ");
    while (tok != NULL) {
        char* new_tok = calloc(5, sizeof(char*));
        strcpy(new_tok, tok);
        *pt = new_tok;
        pt++;
        tok = strtok(NULL, " ");
    }
    if (pt == ret) {
        char* new_tok = calloc(5, sizeof(char*));
        strcpy(new_tok, charset);
        *pt = new_tok;
        pt++;
    }
    *pt = "";
    return ret;
}

char* charset_to_string(const uint32_t charset[2]) {
    const uint32_t minc = charset[0];
    const uint32_t maxc = charset[1];
    uint32_t maxlen = maxc - minc;
    maxlen *= 5;
    maxlen += 2;
    char* ret = calloc(maxlen, sizeof(char));
    char* pt = ret;
    for (uint32_t i = minc; i <= maxc; i++) {
        char* cp = codepoint_to_str(i);
        strcpy(pt, cp);
        while (*pt != '\0') pt++;
        if (i != maxc - 1) {
            *pt++ = ' ';
        }
    }
    return ret;
}

void usage_charsets() {
    printf("\nCHARSETS:\n");
    printf(" -A: Use ASCII (default)\n");
    printf("\t%s\n", charset_to_string(CHARSETS[CHARSET_DEFAULT]));
    printf(" -c: Use Japanese half-width katakana, similar to original matrix. Requires appropriate fonts\n");
    printf("\t%s\n", charset_to_string(CHARSETS[CHARSET_KATAKANA]));
    printf(" -l: Linux mode (uses Linux tty font)\n");
    printf(" -m: lambda mode (equivalent to -u λ)\n");
    printf(" -S: CJK symbols mode. Requires appropriate fonts\n");
    printf("\t%s\n", charset_to_string(CHARSETS[CHARSET_SYMBOLS]));
    printf(" -U [charset]: user-defined (space separated values, e.g. 'あ い お')\n");
}
