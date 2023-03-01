#pragma once

#include <assert.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x) < 0 ? -1*(x) : (x));

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

void util_sort_desc(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

char* util_str_goto_char(char* line, int len, char c)
{
    char* p = line;
    for(int i = 0; i < len; ++i)
    {
        if(*p++ == c)
        {
            return p;
        }
    }

    return NULL;

}

char** util_str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Count how many elements will be extracted.
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for trailing token.
    count += last_comma < (a_str + strlen(a_str) - 1);

    // Add space for terminating null string
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int util_get_prime_factors(int num, int* factors, int factors_len)
{
    int c = 2;
    int fi = 0;
    while (num > 1) {
    if (num % c == 0) {
        factors[fi++] = c;
        if(fi >= factors_len)
        {
            printf("Too many factors to fit in array1\n");
            break;
        }
        num /= c;
    }
    else
        c++;
    }

    return fi;
}

void util_wait_until_key_press()
{
    getchar();
}
