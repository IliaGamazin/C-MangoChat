#include "../inc/libmx.h"

char **mx_strsplit(const char *s, char c) {
    if (!s) {
        return NULL;
    }
    int count_words = mx_count_words(s, c);
    char **res = (char**)malloc(sizeof(char*) * (count_words + 1));
    if (!res) {
        return NULL;
    }
    res[count_words] = NULL;
    int j = 0;
    for (int i = 0; i < count_words; i++) {
        while (s[j] == c) {
            j++;
        }
        int start = j;
        while (s[j] != c && s[j] != '\0') {
            j++;
        }
        int word_length = j - start;
        res[i] = mx_strnew(word_length);
        if (!res[i]) {
            return NULL;
        }
        mx_strncpy(res[i], s + start, word_length);
    }
    return res;
}


