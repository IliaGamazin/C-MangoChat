#include "../inc/libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (!sub || !str || !replace) {
        return NULL;
    }
    int str_len = mx_strlen(str);
    int rep_len = mx_strlen(replace);
    int sub_len = mx_strlen(sub);
    if (!sub_len) {
        return mx_strdup(str);
    }
    int substr_count = mx_count_substr(str, sub);
    int res_size = str_len + substr_count * (rep_len - mx_strlen(sub));
    char *res = mx_strnew(res_size);
    if (!res) {
        return NULL;
    }
    char *temp_sub = NULL;
    char *temp_str = (char*)str;
    char *temp_res = res;
    while ((temp_sub = mx_strstr(temp_str, sub))) {
        int delta = temp_sub - temp_str;
        mx_strncpy(temp_res, temp_str, delta);
        temp_res += delta;
        mx_strcpy(temp_res, replace);
        temp_res += rep_len;        
        temp_str += delta + sub_len;
    }
    mx_strcpy(temp_res, temp_str);
    return res;
}


