#include "../inc/libmx.h"

void mx_swap_string (char **str1, char **str2) {
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

int mx_partition(char **arr, int left, int right, int* res) {
    int mid = (right + left) / 2;
    while(left <= right) {
        while (mx_strlen(arr[left]) < mx_strlen(arr[mid]))
            left++;
        while (mx_strlen(arr[right]) > mx_strlen(arr[mid]))
            right--;
        if (left == right) {
            left++;
            right--;
        }
        else if (left < right) {
            if (mx_strlen(arr[left]) != mx_strlen(arr[right])) {
                mx_swap_string(&arr[left], &arr[right]);
                (*res)++;
            }
            left++;
            right--;
        }
    }
    return left;
}

int mx_quicksort(char **arr, int left, int right) {
    if (!arr) {
        return -1;
    }
    int res = 0;
    if (left < right) {
        int part_index = mx_partition(arr, left, right, &res);
        if (left < part_index - 1) {
            res += mx_quicksort(arr, left, part_index - 1);
        }
        if (part_index < right) {
            res += mx_quicksort(arr, part_index, right);
        }
    }
    return res;
}


int main(void){
	char* arr[] = {"DMC", "Clint Eastwood", "Dr Brown", "Einstein", "Jessica", "Biff Tannen"};
	printf("%d\n", mx_quicksort(arr, 0, 5));
	return 0;
}
