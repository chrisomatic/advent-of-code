#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "common/util.h"
#include "common/stack.h"
#include "common/queue.h"
#include "common/directory.h"

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

    char line[100+1] = {0};
    int elf_totals[1000] = {0};
    int elf_count = 0;

    int total = 0;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        if(strlen(line) == 1)
        {
            // newline
            elf_totals[elf_count++] = total;
            total = 0;
        }
        else
        {
            int val = atoi(line);
            total += val;
        }
    }

    fclose(fp);

    util_sort(elf_totals,elf_count);

    printf("1) Largest Total: %d\n",elf_totals[elf_count-1]);
    printf("2) Sum of top 3 totals: %d\n",
            elf_totals[elf_count-1]+elf_totals[elf_count-2]+elf_totals[elf_count-3]
    );

}

enum
{
    GAME_DRAW = 0,
    GAME_WIN  = 1,
    GAME_LOSE = 2,
};


void day2()
{
    util_print_day(2);

    char* input_file = "inputs/2.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char col1, col2;

    int points = 0;

    for(;;)
    {
        int num_matches = fscanf(fp,"%c %c\n",&col1,&col2);

        if(num_matches != 2)
            break;

        points += (col2 - 'X' + 1);

        if((col2 == 'X' && col1 == 'A') || (col2 == 'Y' && col1 == 'B') || (col2 == 'Z' && col1 == 'C'))
        {
            points += 3;
        }
        else if((col2 == 'X' && col1 == 'C') || (col2 == 'Y' && col1 == 'A') || (col2 == 'Z' && col1 == 'B'))
        {
            points += 6;
        }
        else
        {
            points += 0;
        }
    }

    printf("1) Total Points: %d\n",points);

    points = 0;
    fseek(fp,0,SEEK_SET);

    for(;;)
    {
        int num_matches = fscanf(fp,"%c %c\n",&col1,&col2);

        if(num_matches != 2)
            break;

        points += ((col2 - 'X') * 3);

        if((col1 == 'A' && col2 == 'Y') || (col1 == 'B' && col2 == 'X') || (col1 == 'C' && col2 == 'Z'))
        {
            // need to choose rock
            points += 1;
        }
        else if((col1 == 'A' && col2 == 'Z') || (col1 == 'B' && col2 == 'Y') || (col1 == 'C' && col2 == 'X'))
        {
            // need to choose paper
            points += 2;
        }
        else
        {
            // need to choose scissors
            points += 3;
        }
    }

    printf("2) Total Points: %d\n",points);

    fclose(fp);

}

typedef struct
{
    char items[100+1];
    int len;
} RuckSack_t;

int day3_calc_priority(char c)
{
    int priority = 0;

    if(c >= 'a' && c <= 'z')
    {
        priority = c - 'a' + 1;
    }
    else if(c >= 'A' && c <= 'Z')
    {
        priority = c - 'A' + 27;
    }

    return priority;

}

void day3()
{
    util_print_day(3);

    char* input_file = "inputs/3.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    RuckSack_t rs = {0};

    int total_priority = 0;

    for(;;)
    {
        int c = fgetc(fp);
        if(c == EOF)
            break;

        if(c == '\n')
        {
            // finished ruck sack
            int halfsack = rs.len / 2.0;

            char common = '?';

            // get common letter in each compartment
            for(int i = 0; i < halfsack; ++i)
            {
                char first = rs.items[i];
                for(int j = halfsack; j < rs.len; ++j)
                {
                    char second = rs.items[j];
                    if(first == second)
                    {
                        common = first;
                        goto found_common;
                    }
                }
            }

found_common:

            int priority = day3_calc_priority(common);

            total_priority += priority;

            memset(rs.items,0,100);
            rs.len = 0;
        }
        else
        {
            rs.items[rs.len] = (char)c;
            rs.len++;
        }
    }

    printf("1) Total Priority: %d\n",total_priority);


    total_priority = 0;
    RuckSack_t sacks[3] = {0};

    fseek(fp,0,SEEK_SET);

    char line[100+1] = {0};
    int line_count = 0;

    bool should_break = false;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            should_break = true;

        int priority = 0;

        if(line_count > 0 && line_count % 3 == 0)
        {
            char badge = '?';

            // process last 3 ruck sacks
            for(int i = 0; i < sacks[0].len; ++i)
            {
                char item = sacks[0].items[i];

                for(int j = 0; j < sacks[1].len; ++j)
                {
                    if(sacks[1].items[j] == item)
                    {
                        for(int k = 0; k < sacks[2].len; ++k)
                        {
                            if(sacks[2].items[k] == item)
                            {
                                badge = item;
                                priority = day3_calc_priority(badge);
                                total_priority += priority;
                                goto found_badge;
                            }
                        }
                    }
                }
            }
found_badge:
        }

        RuckSack_t* s = &sacks[line_count%3];

        memcpy(s->items,line,100);
        s->len = strlen(s->items);

        line_count++;

        if(should_break)
            break;

    }

    printf("2) Total Priority: %d\n",total_priority);

    fclose(fp);

}

typedef struct
{
    int min;
    int max;
} Section_t;

void day4()
{
    util_print_day(4);

    char* input_file = "inputs/4.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Section_t s1,s2;


    int num_contained = 0;
    int num_overlap = 0;

    for(;;)
    {
        int num_matches = fscanf(fp,"%d-%d,%d-%d\n",&s1.min,&s1.max,&s2.min,&s2.max);

        if(num_matches != 4)
            break;

        bool contained = (s2.min >= s1.min && s2.max <= s1.max) || (s1.min >= s2.min && s1.max <= s2.max);

        if(contained)
        {
            num_contained++;
        }

        bool exclusive = (s1.max < s2.min || s1.min > s2.max) || (s2.max < s1.min || s2.min > s1.max);

        if(!exclusive)
        {
            num_overlap++;
        }

    }

    printf("1) Num contained: %d\n",num_contained);
    printf("2) Num overlap: %d\n",num_overlap);

}

#define NUM_STACKS 9

void day5()
{
    util_print_day(5);

    char* input_file = "inputs/5.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int col_index = 0;

    Stack stacks[NUM_STACKS] = {0};

    // parse out stacks
    for(;;)
    {
        int c = fgetc(fp);

        if(c == '1')
        {
            // end of stacks, read instructions
            char line[100+1];
            // read two lines
            fgets(line,100,fp); 
            fgets(line,100,fp);
            break;
        }

        if(c == '\n')
        {
            col_index = 0;
            continue;
        }

        if(c == '[')
        {
            // found a crate
            // throw it in stack
            int v = fgetc(fp); // get crate value
            int stack_index = (col_index / 4);
            stack_push(&stacks[stack_index], v);
            col_index++;
        }

        col_index++;
    }

    // reverse stacks to proper order
    for(int i = 0; i < NUM_STACKS; ++i)
    {
        Stack* s = &stacks[i];

        stack_reverse(s);
        //stack_print(s);
    }

    // copy stacks before operations to use in part two
    Stack stacks2[NUM_STACKS] = {0};
    memcpy(stacks2,stacks,sizeof(Stack)*NUM_STACKS);

    int instructions_begin = ftell(fp);

    int stack_src,stack_dst,count;

    // part 1
    for(;;)
    {
        int num_matches = fscanf(fp,"move %d from %d to %d\n", &count, &stack_src, &stack_dst);

        if(num_matches != 3)
            break;

        for(int i = 0; i < count; ++i)
        {
            Stack* src = &stacks[stack_src-1];
            Stack* dst = &stacks[stack_dst-1];

            int item = stack_pop(src);
            stack_push(dst,item);
        }
    }

    printf("1) Stack message: ");

    for(int i = 0; i < NUM_STACKS; ++i)
    {
        int c = stack_peek(&stacks[i],0);
        printf("%c", c);
    }
    printf("\n");

    // return to instructions again
    fseek(fp,instructions_begin, SEEK_SET);

    // part 2
    int items[32] = {0};
    for(;;)
    {
        int num_matches = fscanf(fp,"move %d from %d to %d\n", &count, &stack_src, &stack_dst);

        if(num_matches != 3)
            break;

        Stack* src = &stacks2[stack_src-1];
        Stack* dst = &stacks2[stack_dst-1];

        stack_pop_many(src,items,count);
        stack_push_many(dst,items,count);
    }

    printf("2) Stack message: ");

    for(int i = 0; i < NUM_STACKS; ++i)
    {
        int c = stack_peek(&stacks2[i],0);
        printf("%c", c);
    }
    printf("\n");

}

bool day6_is_stream_unique(Queue* s, int count)
{
    if(s->item_count < count)
        return false;

    // check 4 characters are different
    for(int i = 0; i < count; ++i)
    {
        char ci = s->items[i];
        for(int j = 0; j < count; ++j)
        {
            if(i == j)
                continue;

            char cj = s->items[j];

            if(ci == cj)
            {
                return false;
            }
        }
    }
    
    return true;

}

void day6()
{
    util_print_day(6);

    char* input_file = "inputs/6.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int stream_index = 0;

    Queue recent = {0};

    // part 1
    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(recent.item_count == 4)
            dequeue(&recent); // shift out first

        enqueue(&recent,c);
        //queue_print(&recent); // for testing

        bool unique = day6_is_stream_unique(&recent, 4);

        if(unique)
        {
            printf("1) Stream Index: %d\n",stream_index+1);
            break;
        }

        stream_index++;
    }

    fseek(fp,0,SEEK_SET);
    stream_index = 0;

    queue_reset(&recent);

    // part 2
    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(recent.item_count == 14)
            dequeue(&recent); // shift out first

        enqueue(&recent,c);
        //queue_print(&recent); // for testing

        bool unique = day6_is_stream_unique(&recent, 14);

        if(unique)
        {
            printf("2) Stream Index: %d\n",stream_index+1);
            break;
        }

        stream_index++;
    }
}


void day7()
{
    util_print_day(7);

    char* input_file = "inputs/7.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    FileNode root = {.name = "/", .dir = true};
    FileNode* curr_node = &root;

    // build directory hierarchy
    for(;;)
    {
        char line[100+1] = {0};

        if(fgets(line,100,fp) == NULL)
            break;

        // remove newline
        int line_len = strlen(line);
        line[line_len-1] = 0;

        if(line[0] == '$')
        {
            // command
            if(line[2] == 'c' && line[3] == 'd')
            {
                // cd
                char name[32+1] = {0};
                strcpy(name,&line[5]);

                //printf("cd -> %s\n",name);

                if(strcmp(name,"..") == 0)
                {
                    curr_node = curr_node->parent;
                }
                else
                {
                    curr_node = dir_goto(&root,name);
                }
                if(!curr_node)
                {
                    printf("Could not find node %s\n",name);
                }
            }
        }
        else
        {
            char name[32+1] = {0};

            // parse directory or file
            if(line[0] == 'd' && line[1] == 'i' && line[2] == 'r')
            {
                // dir
                strcpy(name,&line[4]);
                dir_add_node(curr_node,name,true);
            }
            else
            {
                int size = 0;

                int matches = sscanf(line, "%d %s",&size,name);

                // file
                FileNode* f = dir_add_node(curr_node,name,false);
                f->size = size;
            }
        }
    }

    // go through structure, and set total sizes on directory nodes
    int sum = 0;
    dir_get_total_sizes(&root,&sum);
    dir_print(&root,0);
}

int main(int argc, char* args[])
{
    printf("\n===================== AOC 2022 =======================\n");
    day1();
    day2();
    day3();
    day4();
    day5();
    day6();
    day7();
    printf("\n======================================================\n");
    return 0;
}
