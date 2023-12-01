#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "common/util.h"
#include "common/list.h"
#include "common/stack.h"
#include "common/queue.h"
#include "common/directory.h"
#include "common/path.h"

const char* named_numbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
bool debug = false;

bool day1_is_potential_match(char* s, int len)
{
    for(int i = 0; i < 9; ++i)
    {
        const char* cmp = named_numbers[i];
        int cmp_len = strlen(cmp);

        bool match = true;
        for(int j = 0; j < len; ++j)
        {
            if(j >= cmp_len)
            {
                match = false;
                break;
            }

            if(s[j] != cmp[j])
            {
                match = false;
                break;
            }
        }

        if(match)
        {
            if(debug)
                printf("%s partial match to %s\n", s,cmp);

            return true;
        }
    }
    return false;
}

int day1_is_number(char* s)
{
    for(int i = 0; i < 9; ++i)
    {
        if(strcmp(s,named_numbers[i]) == 0)
        {
            if(debug)
                printf("Match with %s", named_numbers[i]);

            return (i+1);
        }
    }

    return -1;
}

// part 2
// 53391 (too high)

void day1()
{
    util_print_day(1);

    char* input_file = "inputs/1.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int number1 = -1;
    int number2 = -1;

    int sum = 0;

    int c;

    for(;;)
    {
        c = fgetc(fp);

        if(feof(fp))
            break;

        if(c == '\n')
        {
            // next line

            if(number1 == -1)
                number1 = 0;

            if(number2 == -1)
                number2 = number1;
            
            // we got both numbers
            // add to sum
            int num = (number1*10 + number2);
            sum += num;

            number1 = -1;
            number2 = -1;
            continue;
        }

        if(c >= '0' && c <= '9')
        {
            // number!
            if(number1 > -1)
                number2 = c - '0';
            else
                number1 = c - '0';
        }
    }

    printf("1) Sum is %d\n",sum);

    // reset file seek
    fseek(fp,0,SEEK_SET);

    number1 = -1;
    number2 = -1;
    sum = 0;

    char line[100+1] = {0};
    int line_no = 0;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        int line_len = strlen(line);
        line[line_len-1] = 0; // remove newline

        // move forward through line to find first number
        for(int i = 0; i < line_len; ++i)
        {
            char c = line[i];
            //printf("c: %c (%d/%d)\n",c,i,line_len);

            if(c >= '0' && c <= '9')
            {
                if(number1 > -1)
                    number2 = c - '0';
                else
                    number1 = c - '0';
            }
            else
            {
                char check_buf[32] = {0};

                check_buf[0] = c;
                int checki = 1;

                int j = 0;
                while(day1_is_potential_match(check_buf, checki))
                {
                    j++;
                    if(i+j >= line_len)
                    {
                        break;
                    }

                    check_buf[checki++] = line[i+j];

                    int res = day1_is_number(check_buf);
                    if(res > -1)
                    {
                        if(number1 > -1)
                            number2 = res;
                        else
                            number1 = res;
                        break;
                    }
                }
            }
        }

        if(number1 == -1)
            number1 = 0;
        if(number2 == -1)
            number2 = number1;

        int num = number1*10 + number2;

        sum += num;

        number1 = -1;
        number2 = -1;
    }

    printf("2) Sum is %d\n",sum);

}

int main(int argc, char* args[])
{
    printf("\n===================== AOC 2023 =======================\n");

    day1();

    printf("\n======================================================\n");
    return 0;
}
