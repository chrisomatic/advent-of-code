#pragma once

#define ABS(x) (x < 0 ? -1*x : x);

void util_print_day(int day)
{
    printf("\n");
    printf("--------------------------\n");
    printf("---       Day %d       ---\n",day);
    printf("--------------------------\n");
    printf("\n");
}


void util_sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
