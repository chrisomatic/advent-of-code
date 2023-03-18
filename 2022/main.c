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

bool day6_is_stream_unique(IntQueue_t* s, int count)
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

    IntQueue_t recent = {0};

    // part 1
    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(recent.item_count == 4)
            dequeue_int(&recent); // shift out first

        enqueue_int(&recent,c);
        //queue_int_print(&recent); // for testing

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

    queue_int_reset(&recent);

    // part 2
    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(recent.item_count == 14)
            dequeue_int(&recent); // shift out first

        enqueue_int(&recent,c);
        //queue_int_print(&recent); // for testing

        bool unique = day6_is_stream_unique(&recent, 14);

        if(unique)
        {
            printf("2) Stream Index: %d\n",stream_index+1);
            break;
        }

        stream_index++;
    }
}

void day7_get_sum_directories(FileNode* r, int* sum)
{
    if(r->dir)
    {
        if(r->size <= 100000)
        {
            *sum += r->size;
        }

        for(int i = 0; i < r->node_count; ++i)
        {
            day7_get_sum_directories(r->nodes[i],sum);
        }
    }

}

void day7_get_min_directory_to_delete(FileNode* r,const int threshold, int* curr_min)
{
    if(r->dir)
    {
        if(r->size >= threshold)
        {
            if(r->size < *curr_min)
                *curr_min = r->size;
        }

        for(int i = 0; i < r->node_count; ++i)
        {
            day7_get_min_directory_to_delete(r->nodes[i],threshold,curr_min);
        }
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

                FileNode* prev = curr_node;

                if(strcmp(name,"/") == 0)
                    curr_node = &root;
                else if(strcmp(name,"..") == 0)
                    curr_node = curr_node->parent;
                else
                    curr_node = dir_goto_1level(curr_node,name);

                if(!curr_node)
                {
                    printf("Could not find node %s\n",name);
                    curr_node = prev;
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
    dir_calc_sizes(&root);

    // print directory structure
    //dir_print(&root,0);

    int sum_p1 = 0;
    day7_get_sum_directories(&root, &sum_p1);

    printf("1) Sum: %d\n",sum_p1);

    // part 2
    const int total_space = 70000000;
    const int needed_space = 30000000;

    int used_space = root.size;
    int unused_space = total_space - used_space;
    int threshold_size = needed_space - unused_space;

    int min = INT_MAX;
    day7_get_min_directory_to_delete(&root,(const int)threshold_size, &min);

    printf("2) Minimum size: %d\n",min);
}

#define GRID_SIZE 99

void day8_print_grid(int grid[GRID_SIZE][GRID_SIZE])
{
    for(int i = 0; i < GRID_SIZE; ++i)
    {
        for(int j = 0; j < GRID_SIZE; ++j)
        {
            printf("%d",grid[i][j]);
        }
        printf("\n");
    }
}

void day8()
{
    util_print_day(8);

    char* input_file = "inputs/8.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int grid[100][100] = {0};
    int row = 0;
    int col = 0;

    for(;;)
    {
        int c = fgetc(fp);
        if(c == EOF)
            break;

        if(c == '\n')
        {
            row++;
            col = 0;
            continue;
        }

        int x = ((char)c - '0');
        grid[row][col] = x;
        col++;
    }

    //day8_print_grid(grid);

    int visible_tree_count = 4*(GRID_SIZE-1); // include all trees on the perimeter

    // part 1
    // traverse through inner 'trees'
    for(int i = 1; i < GRID_SIZE -1; ++i)
    {
        for(int j = 1; j < GRID_SIZE -1; ++j)
        {
            int tree = grid[i][j];
            
            bool left = true;
            bool right = true;
            bool up = true;
            bool down = true;

            // check left
            for(int l = i-1; l >= 0; --l)
            {
                int check = grid[l][j];
                if(check >= tree)
                {
                    left = false;
                    break;
                }
            }

            // check right
            for(int r = i+1; r <= GRID_SIZE-1; ++r)
            {
                int check = grid[r][j];
                if(check >= tree)
                {
                    right = false;
                    break;
                }
            }

            // check up
            for(int u = j-1; u >= 0; --u)
            {
                int check = grid[i][u];
                if(check >= tree)
                {
                    up = false;
                    break;
                }
            }

            // check down
            for(int d = j+1; d <= GRID_SIZE-1; ++d)
            {
                int check = grid[i][d];
                if(check >= tree)
                {
                    down = false;
                    break;
                }
            }

            bool visible = left || right || up || down;

            if(visible)
            {
                visible_tree_count++;
            }
        }
    }

    printf("1) Visible trees: %d\n",visible_tree_count);

    int max_scenic_score = 0;

    for(int i = 0; i < GRID_SIZE; ++i)
    {
        for(int j = 0; j < GRID_SIZE; ++j)
        {
            int tree = grid[i][j];

            int left = 0;
            int right = 0;
            int up = 0;
            int down = 0;

            // check left
            for(int l = i-1; l >= 0; --l)
            {
                left++;
                int check = grid[l][j];
                if(check >= tree)
                    break;
            }

            // check right
            for(int r = i+1; r <= GRID_SIZE-1; ++r)
            {
                right++;
                int check = grid[r][j];
                if(check >= tree)
                    break;
            }

            // check up
            for(int u = j-1; u >= 0; --u)
            {
                up++;
                int check = grid[i][u];
                if(check >= tree)
                    break;
            }

            // check down
            for(int d = j+1; d <= GRID_SIZE-1; ++d)
            {
                down++;
                int check = grid[i][d];
                if(check >= tree)
                    break;
            }

            int scenic_score = up * down * left * right;

            if(scenic_score > max_scenic_score)
            {
                max_scenic_score = scenic_score;
            }
        }
    }

    printf("2) Max Scenic Score: %d\n",max_scenic_score);
}

typedef struct
{
    int x;
    int y;
} Knot_t;

void day9_update_tail(Knot_t* tail,Knot_t* head)
{
    // move tail

    int diff_x = head->x - tail->x;
    int diff_y = head->y - tail->y;

    if(diff_y == 0) // aligned horizontally
    {
        if(diff_x == -2)
            tail->x--;
        else if(diff_x == 2)
            tail->x++;
    }
    else if(diff_x == 0) // aligned vertically
    {
        if(diff_y == -2)
            tail->y--;
        else if(diff_y == 2)
            tail->y++;
    }
    else
    {
        // not in same row or column
        
        float dist = sqrt(diff_x*diff_x + diff_y*diff_y);

        if(dist >= 2.0)
        {
            // not touching, move tail diagonally
            if(diff_x < 0 && diff_y < 0)
            {
                tail->x--;
                tail->y--;
            }
            else if(diff_x < 0 && diff_y > 0)
            {
                tail->x--;
                tail->y++;
            }
            else if(diff_x > 0 && diff_y < 0)
            {
                tail->x++;
                tail->y--;
            }
            else if(diff_x > 0 && diff_y > 0)
            {
                tail->x++;
                tail->y++;
            }
        }
    }
}

void day9()
{
    util_print_day(9);

    char* input_file = "inputs/9.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Knot_t visited_places[10000] = {0};
    int num_visited_places = 0;

    Knot_t head = {0};
    Knot_t tail = {0};

    for(;;)
    {

        char dir;
        int num;

        int num_matches = fscanf(fp,"%c %d\n",&dir,&num);
        if(num_matches != 2)
            break;

        for(int i = 0; i < num; ++i)
        {
            // move head
            switch(dir)
            {
                case 'L': head.x--; break;
                case 'R': head.x++; break;
                case 'U': head.y--; break;
                case 'D': head.y++; break;
            }

            day9_update_tail(&tail,&head);

            bool is_pos_unique = true;
            for(int j = 0; j < num_visited_places; ++j)
            {
                if(visited_places[j].x == tail.x && visited_places[j].y == tail.y)
                {
                    is_pos_unique = false;
                }
            }

            if(is_pos_unique)
            {
                visited_places[num_visited_places].x = tail.x;
                visited_places[num_visited_places].y = tail.y;
                num_visited_places++;
            }
        }
    }

    printf("1) Unique positions: %d\n",num_visited_places);

    // reset for part 2
    fseek(fp,0,SEEK_SET);
    num_visited_places = 0;

    Knot_t knots[10] = {0};

    Knot_t* head2 = &knots[0];
    Knot_t* tail2 = &knots[9];

    for(;;)
    {
        char dir;
        int num;

        int num_matches = fscanf(fp,"%c %d\n",&dir,&num);
        if(num_matches != 2)
            break;

        for(int i = 0; i < num; ++i)
        {
            // move head
            switch(dir)
            {
                case 'L': head2->x--; break;
                case 'R': head2->x++; break;
                case 'U': head2->y--; break;
                case 'D': head2->y++; break;
            }

            // update tailing knots
            for(int j = 0; j < 9; ++j)
            {
                day9_update_tail(&knots[j+1],&knots[j]);
            }

            bool is_pos_unique = true;
            for(int j = 0; j < num_visited_places; ++j)
            {
                if(visited_places[j].x == tail2->x && visited_places[j].y == tail2->y)
                {
                    is_pos_unique = false;
                }
            }

            if(is_pos_unique)
            {
                visited_places[num_visited_places].x = tail2->x;
                visited_places[num_visited_places].y = tail2->y;
                num_visited_places++;
            }
        }
    }

    printf("2) Unique positions: %d\n",num_visited_places);
}

typedef enum
{
    INST_NONE,
    INST_NOOP,
    INST_ADDX,
} Instruction;

void day10_incr_cycle(int* cycle,char crt[40][6], int x)
{
    int c = *cycle;
    int cx = c % 40;
    int cy = c / 40;
    (*cycle)++;

    if(( x-1 == cx) || (x == cx) || (x+1 == cx))
    {
        crt[cx][cy] = '#';
    }
    else
    {
        crt[cx][cy] = '.';
    }
}

void day10()
{
    util_print_day(10);

    char* input_file = "inputs/10.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int cycle = 0;
    int x = 1;
    int curr_signal_str = 0;

    int signal_strengths[10] = {0};
    int num_signal_strengths = 0;

    char line[10+1] = {0};

    for(;;)
    {
        if(fgets(line,10,fp) == NULL)
            break;

        if(line[0] == 'n' && line[1] == 'o' && line[2] == 'o' && line[3] == 'p')
        {
            // noop
            // printf("noop\n");
            cycle++;
            curr_signal_str = x*cycle;
            if(cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
            {
                signal_strengths[num_signal_strengths++] = curr_signal_str;
            }
            continue;
        }

        char instruction[5] = {0};
        int operand;
        int matches = sscanf(line,"%s %d",instruction, &operand);
        if(matches != 2)
        {
            printf("matches: %d\n",matches);
            continue;
        }

        if(strcmp(instruction,"addx") == 0)
        {
            //printf("addx %d\n",operand);
            cycle++;
            curr_signal_str = x*cycle;
            if(cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
            {
                signal_strengths[num_signal_strengths++] = curr_signal_str;
            }
            cycle++;
            curr_signal_str = x*cycle;
            if(cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
            {
                signal_strengths[num_signal_strengths++] = curr_signal_str;
            }
            x += operand;
        }
    }

    int sum = 0;
    for(int i = 0; i < num_signal_strengths; ++i)
    {
        sum += signal_strengths[i];
    }
    
    printf("1) Sum: %d\n",sum);

    // part 2

    fseek(fp,0,SEEK_SET);
    cycle = 0;
    x = 1;

    char crt[40][6] = {0};
    int crt_x = 0,crt_y = 0;

    for(;;)
    {
        if(fgets(line,10,fp) == NULL)
            break;

        if(line[0] == 'n' && line[1] == 'o' && line[2] == 'o' && line[3] == 'p')
        {
            // noop
            // printf("noop\n");
            day10_incr_cycle(&cycle,crt,x);
        }
        else
        {
            char instruction[5] = {0};
            int operand;
            int matches = sscanf(line,"%s %d",instruction, &operand);
            if(matches != 2)
            {
                printf("matches: %d\n",matches);
                continue;
            }

            if(strcmp(instruction,"addx") == 0)
            {
                //printf("addx %d\n",operand);
                day10_incr_cycle(&cycle,crt,x);
                day10_incr_cycle(&cycle,crt,x);
                x += operand;
            }
        }

        // determine pixel value
    }

    printf("2)\n");

    // print CRT screen
    for(int j = 0; j < 6; ++j)
    {
        printf("   ");
        for(int i = 0; i < 40; ++i)
        {
            printf("%c",crt[i][j]);
        }
        printf("\n");
    }
}

typedef struct
{
    IntQueue_t items;
    char operation;
    int operand;
    int test_divisor;
    int true_monkey;
    int false_monkey;
    int total_inspections;
} Monkey_t;
typedef struct
{
    Int64Queue_t items;
    char operation;
    int operand;
    int test_divisor;
    int true_monkey;
    int false_monkey;
    int total_inspections;
} Monkey2_t;

void day11_print_monkey(Monkey_t* m)
{
    printf("=========== Monkey ==============\n");
    printf("Items (%d): ", m->items.item_count);
    for(int i = 0; i < m->items.item_count; ++i)
    {
        printf("%d", m->items.items[i]);
        if(i < m->items.item_count-1)
            printf(", ");
    }
    printf("\n");
    printf("Operation: %c %d\n", m->operation, m->operand);
    printf("Test Divisor: %d\n",m->test_divisor);
    printf("True Case: throw to monkey %d\n",m->true_monkey);
    printf("False Case: throw to monkey %d\n",m->false_monkey);
}

void day11()
{
    util_print_day(11);

    char* input_file = "inputs/11.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char line[100+1] = {0};

    Monkey_t monkeys[8] = {0};
    int num_monkeys;

    // parse out monkeys

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        int num = -1;
        int num_matches = sscanf(line,"Monkey %d:\n",&num);

        if(num_matches == 1)
        {
            Monkey_t* monkey = &monkeys[num];

            char* p;

            // items
            fgets(line,100,fp);
            p = util_str_goto_char(line,100,':');

            char** tokens = util_str_split(p, ',');
            if(tokens)
            {
                for (int i = 0; *(tokens + i); ++i)
                {
                    int v = atoi(*(tokens+i));
                    enqueue_int(&monkey->items,v);
                    free(*(tokens + i));
                }
                free(tokens);
            }

            // operation
            fgets(line,100,fp);
            p = util_str_goto_char(line,100,':');
            sscanf(p+1,"new = old %c %d\n",&monkey->operation, &monkey->operand);

            // test
            fgets(line,100,fp);
            p = util_str_goto_char(line,100,':');
            sscanf(p+1,"divisible by %d\n",&monkey->test_divisor);

            // true case
            fgets(line,100,fp);
            p = util_str_goto_char(line,100,':');
            sscanf(p+1,"throw to monkey %d\n",&monkey->true_monkey);

            // false case
            fgets(line,100,fp);
            p = util_str_goto_char(line,100,':');
            sscanf(p+1,"throw to monkey %d\n",&monkey->false_monkey);

            //day11_print_monkey(monkey);

            num_monkeys++;
        }
    }

    // copy monkey to monkeys2 for part 2 later
    Monkey2_t monkeys2[8] = {0};

    for(int i = 0; i < num_monkeys; ++i)
    {
        for(int j = 0; j < monkeys[i].items.item_count; ++j)
        {
            enqueue_int64(&monkeys2[i].items,(int64_t)monkeys[i].items.items[j]);
        }
        monkeys2[i].operation         = monkeys[i].operation;
        monkeys2[i].operand           = monkeys[i].operand;
        monkeys2[i].test_divisor      = monkeys[i].test_divisor;
        monkeys2[i].true_monkey       = monkeys[i].true_monkey;
        monkeys2[i].false_monkey      = monkeys[i].false_monkey;
        monkeys2[i].total_inspections = monkeys[i].total_inspections;
    }

    // part 1

    // Go bananas
    for(int r = 0; r < 20; ++r)
    {
        // begin round
        for(int i = 0; i < num_monkeys; ++i)
        {
            Monkey_t* m = &monkeys[i];

            int initial_item_count = m->items.item_count;

            for(int j = 0; j < initial_item_count; ++j)
            {
                int item = dequeue_int(&m->items);

                switch(m->operation)
                {
                    case '+':
                        item = item + m->operand;
                        break;
                    case '*':
                        if(m->operand == 0)
                        {
                            item = item * item;
                            break;
                        }
                        item = item * m->operand;
                        break;
                    default:
                        break;
                }

                item /= 3;

                m->total_inspections++;

                int remainder = item % m->test_divisor;

                if(remainder == 0)
                {
                    // true
                    enqueue_int(&monkeys[m->true_monkey].items,item);
                }
                else
                {
                    // false
                    enqueue_int(&monkeys[m->false_monkey].items,item);
                }
            }
        }
        // end round
    }

    int inspection_list[8] = {0};

    for(int i = 0; i < num_monkeys; ++i)
    {
        inspection_list[i] = monkeys[i].total_inspections;
    }

    // sort to get top 2 total inspections
    util_sort_desc(inspection_list, num_monkeys);

    int monkey_business = inspection_list[0] * inspection_list[1];

    printf("1) Monkey Business: %d\n", monkey_business);
    
    // part 2

    int common_factor = 1;

    for(int i = 0; i < num_monkeys; ++i)
    {
        common_factor *= monkeys[i].test_divisor;
    }

    // Go bananas
    for(int r = 0; r < 10000; ++r)
    {
        // begin round
        for(int i = 0; i < num_monkeys; ++i)
        {
            Monkey2_t* m = &monkeys2[i];

            int initial_item_count = m->items.item_count;

            for(int j = 0; j < initial_item_count; ++j)
            {
                int64_t item = dequeue_int64(&m->items);

                switch(m->operation)
                {
                    case '+':
                        item = item + m->operand;
                        break;
                    case '*':
                        if(m->operand == 0)
                        {
                            item = item * item;
                            break;
                        }
                        item = item * m->operand;
                        break;
                    default:
                        break;
                }

                m->total_inspections++;

                item %= common_factor;

                int64_t remainder = item % m->test_divisor;

                if(remainder == 0)
                {
                    // true
                    enqueue_int64(&monkeys2[m->true_monkey].items,item);
                }
                else
                {
                    // false
                    enqueue_int64(&monkeys2[m->false_monkey].items,item);
                }
            }
        }
        // end round
    }

    for(int i = 0; i < num_monkeys; ++i)
    {
        inspection_list[i] = monkeys2[i].total_inspections;
    }

    // sort to get top 2 total inspections
    util_sort_desc(inspection_list, num_monkeys);

    int64_t monkey_business2 = (int64_t)inspection_list[0] * inspection_list[1];

    printf("2) Monkey Business: %lld\n", monkey_business2);
}

void day12(bool test)
{
    util_print_day(12);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/12_test.txt" : "inputs/12.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int grid[256*256] = {0};

    int grid_width = 0, grid_height = 0;
    int row = 0, col = 0;

    PathPos start = {0},end = {0};

    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(c == '\n')
        {
            grid_width = col;
            row++;
            col = 0;
            continue;
        }

        int index = row*grid_width + col;
        if(c == 'S')
        {
            start.x = col;
            start.y = row;
            c = 'a';
        }
        else if(c == 'E')
        {
            end.x = col;
            end.y = row;
            c = 'z';
        }
        grid[index] = c;
        col++;
    }

    grid_height = row;

    //printf("Grid (%d x %d)\n", grid_width, grid_height);
    //printf("S [%d,%d] -> E [%d,%d]\n",start.x,start.y,end.x,end.y);

#if 0
    for(int i = 0; i < grid_height; ++i)
    {
        for(int j = 0; j < grid_width; ++j)
        {
            int index = i*grid_width + j;
            printf("%c",grid[index]);
        }
        printf("\n");
    }
    
#endif

    path_map_set(grid,grid_width,grid_height);
    int part1_path_len = path_find(start.x, start.y, end.x, end.y);

    printf("1) Shortest path: %d\n",part1_path_len);

    // part 2

    // gather all potential starting points
    PathPos starting_points[3000] = {0};
    int starting_points_count = 0;
    
    for(int i = 0; i < grid_height; ++i)
    {
        for(int j = 0; j < grid_width; ++j)
        {
            int c = grid[i*grid_width+j];
            if(c == 'a')
            {
                starting_points[starting_points_count].x = j;
                starting_points[starting_points_count].y = i;
                starting_points_count++;
            }
        }
    }

    // test all starting points
    // store minimum path length
    int minimum_path_len = INT_MAX;
    
    for(int i = 0; i < starting_points_count; ++i)
    {
        //printf("Checking %d/%d...\n",i,starting_points_count);
        int path_len = path_find(starting_points[i].x, starting_points[i].y, end.x, end.y);
        if(path_len > 0 && path_len < minimum_path_len)
        {
            minimum_path_len = path_len;
        }
    }

    printf("2) Absolute Shortest Path: %d\n",minimum_path_len);
    path_map_free();
    
}


typedef struct ListNode_t ListNode_t;
struct ListNode_t
{
    bool has_value;
    bool is_divider; // for part 2
    int value;
    struct ListNode_t* parent;

    int num_children;
    struct ListNode_t* children[20];
};

void print_packet(ListNode_t* r, int depth)
{
    //printf("depth: %d\n");
    if(r->has_value)
    {
        for(int i = 0 ; i < depth; ++i)
            printf("-");

        printf("[%d]\n",r->value);
    }

    for(int i = 0; i < r->num_children; ++i)
    {
        print_packet(r->children[i],depth+1);
    }
}

int day13_check_values(ListNode_t* l, ListNode_t* r)
{
    /*
        If both values are integers, the lower integer should come first. If the left integer is lower than the right integer, the inputs are in the right order. If the left integer is higher than the right integer, the inputs are not in the right order. Otherwise, the inputs are the same integer; continue checking the next part of the input.
        If both values are lists, compare the first value of each list, then the second value, and so on. If the left list runs out of items first, the inputs are in the right order. If the right list runs out of items first, the inputs are not in the right order. If the lists are the same length and no comparison makes a decision about the order, continue checking the next part of the input.
        If exactly one value is an integer, convert the integer to a list which contains that integer as its only value, then retry the comparison. For example, if comparing [0,0,0] and 2, convert the right value to [2] (a list containing 2); the result is then found by instead comparing [0,0,0] and [2].
    */

    if(l->has_value && r->has_value)
    {
        // both values are integers
        //printf("Comparing [%d] and [%d]\n",l->value,r->value);

        if(l->value < r->value)
        {
            // right order
            return 1;
        }
        else if(l->value > r->value)
        {
            // not right order
            return 2;
        }
        else
        {
            // continue
            return 0;
        }
    }

    if(!l->has_value && !r->has_value)
    {
        //printf("Both values are lists!\n");

        // both values are lists
        for(int i = 0; i < l->num_children; ++i)
        {
            if(i >= r->num_children)
            {
                // right side runs out of items first
                // not right order
                return 2;
            }

            //printf("Checking child %d in both lists\n",i);
            int val = day13_check_values(l->children[i], r->children[i]);
            //printf("Check returned value of %d\n",val);
            if(val == 0)
                continue;

            return val;
        }

        if(r->num_children > l->num_children)
        {
            // right order
            return 1;
        }

        // continue
        return 0;
    }

    // one value is integer, the other is list

    if(l->has_value)
    {
        // convert to list
        ListNode_t* new_node = calloc(sizeof(ListNode_t),1);
        memcpy(new_node, l, sizeof(ListNode_t));

        l->value = 0;
        l->has_value = false;
        l->children[l->num_children++] = new_node;

        int val = day13_check_values(l,r);
        return val;
    }

    if(r->has_value)
    {
        // convert to list
        ListNode_t* new_node = calloc(sizeof(ListNode_t),1);
        memcpy(new_node, r, sizeof(ListNode_t));

        r->value = 0;
        r->has_value = false;
        r->children[r->num_children++] = new_node;

        int val = day13_check_values(l,r);
        return val;
    }

    return 0;
}

ListNode_t* day13_parse_packet(char* str, int len)
{
    ListNode_t* root = calloc(sizeof(ListNode_t),1);
    ListNode_t* current = root;

    char val_str[5] = {0};
    int val_str_index = 0;

    for(int i = 0; i < len; ++i)
    {
        int c = str[i];
        if(c == '\n' || c == '\0')
            break;

        if(c == '[')
        {
            // start of list
            ListNode_t* node = calloc(sizeof(ListNode_t),1);
            node->parent = current;
            current->children[current->num_children++] = node;
            current = node;
        }
        else if(c == ']')
        {
            // end of list
            if(val_str_index > 0)
            {
                int val = atoi(val_str);
                memset(val_str,0,5);
                val_str_index = 0;

                ListNode_t* node = calloc(sizeof(ListNode_t),1);
                node->has_value = true;
                node->value = val;
                node->parent = current;

                current->children[current->num_children++] = node;
            }
            current = current->parent;
        }
        else if(c == ',')
        {
            if(val_str_index > 0)
            {
                int val = atoi(val_str);
                memset(val_str,0,5);
                val_str_index = 0;

                ListNode_t* node = calloc(sizeof(ListNode_t),1);
                node->has_value = true;
                node->value = val;
                node->parent = current;

                current->children[current->num_children++] = node;
            }
        }
        else
        {
            // presumably some number between 0-9
            val_str[val_str_index++] = c;
        }
    }
    return root;

}

void day13_sort_packets(ListNode_t* arr[], int n)
{
    int i, j;
    ListNode_t* key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && day13_check_values(key,arr[j]) == 1) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void day13()
{
    util_print_day(13);

    char* input_file = "inputs/13.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    // parse input
    char pkt1[300+1] = {0};
    char pkt2[300+1] = {0};

    int pair_count = 0;

    int right_indices[100] = {0};
    int right_indices_count = 0;

    ListNode_t* all_packets[302];
    int packet_count = 0;

    for(;;)
    {
        if(fgets(pkt1,300,fp) == NULL) break;
        if(fgets(pkt2,300,fp) == NULL) break;

        pair_count++;

        //printf("%s",pkt1);
        ListNode_t* l = day13_parse_packet(pkt1,100);
        all_packets[packet_count++] = l;
        
        //printf("%s",pkt2);
        ListNode_t* r = day13_parse_packet(pkt2,100);
        all_packets[packet_count++] = r;

        int result = day13_check_values(l,r);
        if(result == 1)
        {
            right_indices[right_indices_count++] = pair_count;
        }

        // eat empty line between
        char line[100+1] = {0};
        if(fgets(line,100,fp) == NULL) break;

        //util_wait_until_key_press();
    }

    int sum = 0;
    for(int i = 0; i < right_indices_count; ++i)
    {
        sum += right_indices[i];
    }

    printf("1) Sum of Right-Order indices: %d\n", sum);


    // part 2
    // go through all packets and order them...

    // todo: add in divider packets
    char* divider1 = "[[2]]\n";
    char* divider2 = "[[6]]\n";

    ListNode_t* d1 = day13_parse_packet(divider1,5);
    ListNode_t* d2 = day13_parse_packet(divider2,5);

    d1->is_divider = true;
    d2->is_divider = true;

    all_packets[packet_count++] = d1;
    all_packets[packet_count++] = d2;
    
    day13_sort_packets(all_packets, packet_count);

    int decoder_signal = 1;
    for(int i = 0; i < packet_count; ++i)
    {
        int index = i-packet_count+2;
        //printf("%d/%d\n",i+1,packet_count);
        //print_packet(all_packets[i],0);
        if(all_packets[i]->is_divider)
        {
            decoder_signal *= (i+1);
        }
        //util_wait_until_key_press();
    }
    
    printf("2) decoder_signal: %d\n",decoder_signal);
}

typedef struct
{
    Vec2i points[32];
    int point_count;
} Path;

bool is_point_rock(int x, int y, Path* paths, int path_count, int y_floor)
{
    if(y_floor > -1)
    {
        if(y >= y_floor)
        {
            return true;
        }
    }

    for(int i = 0; i < path_count; ++i)
    {
        for(int j = 0; j < paths[i].point_count; ++j)
        {
            Vec2i* p1 = &paths[i].points[j];
            Vec2i* p2 = j == paths[i].point_count - 1 ? p1 : &paths[i].points[j+1];

            //printf("p1: %d, %d; p2: %d, %d\n",p1->x, p1->y, p2->x, p2->y);

            int delta_x = p2->x - p1->x;
            int delta_y = p2->y - p1->y;

            int x_dir = delta_x < 0 ? -1 : +1;
            int y_dir = delta_y < 0 ? -1 : +1;

            int abs_x = ABS(delta_x);
            int abs_y = ABS(delta_y);

            for(int k = 0; k <= abs_x; ++k)
            {
                for(int l = 0; l <= abs_y; ++l)
                {
                    Vec2i check = {
                        .x = p1->x + x_dir*k,
                        .y = p1->y + y_dir*l
                    };

                    //printf("checking %d,%d\n",check.x,check.y);

                    if(x == check.x && y == check.y)
                    {
                        return true;
                    }
                }

            }
        }
    }

    return false;
}

bool is_point_sand(int x, int y, Vec2i* sand, int sand_count)
{
    for(int i = 0; i < sand_count; ++i)
    {
        if(sand[i].x == x && sand[i].y == y)
        {
            return true;
        }
    }
    return false;
}

bool is_point_solid(int x, int y, Path* paths, int path_count, Vec2i* sand, int sand_count, int floor)
{
    if(is_point_rock(x,y,paths,path_count,floor) || is_point_sand(x,y,sand,sand_count))
    {
        return true;
    }

    return false;
}

int day14_simulate_sand(Path* paths, int path_count, int floor)
{
    Vec2i sand[100000] = {0};
    int sand_count = 0;
    
    Vec2i s = {500,0};

    for(;;)
    {
        if(!is_point_solid(s.x,s.y+1,paths,path_count,sand,sand_count, floor))
        {
            // down
            s.y++;

            if(floor == -1 && s.y >= 1000)
            {
                // ABYSS
                break;
            }

            continue;
        }

        if(!is_point_solid(s.x-1, s.y+1, paths,path_count,sand,sand_count, floor))
        {
            // down left
            s.x--;
            s.y++;
            continue;

        }

        if(!is_point_solid(s.x+1, s.y+1, paths,path_count,sand,sand_count, floor))
        {
            // down right
            s.x++;
            s.y++;
            continue;
        }


        // come to rest
        // add sand to list
        memcpy(&sand[sand_count++], &s, sizeof(Vec2i));

        if(floor > -1)
        {
            // check if still at origin
            if(s.x == 500 && s.y == 0)
            {
                // done
                break;
            }
        }


        s.x = 500;
        s.y = 0;
    }

#if 0
        // print grid
        for(int i = 0; i <= 9; ++i)
        {
            for(int j = 494; j <= 503; ++j)
            {
                if(is_point_rock(j,i,paths, path_count,floor))
                {
                    printf("#"); // rock
                }
                else if(is_point_sand(j,i,sand, sand_count))
                {
                    printf("o"); // landed sand
                }
                else if(s.x == j && s.y == i)
                {
                    printf("+"); // current sand
                }
                else
                {
                    printf("."); // air
                }
            }
            printf("\n");
        }

        util_wait_until_key_press();
#endif

    return sand_count;
}

void day14(bool test)
{
    util_print_day(14);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/14_test.txt" : "inputs/14.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Path all_paths[200] = {0};
    int path_count = 0;

    // parse rock paths
    char line[300+1] = {0};
    for(;;)
    {
        if(fgets(line,300,fp) == NULL)
            break;

        Path path = {0};

        char* token = strtok(line, "->");

        while(token)
        {
            Vec2i p = {0};
            int matches = sscanf(token,"%d,%d",&p.x,&p.y);
            if(matches == 2)
            {
                path.points[path.point_count].x = p.x;
                path.points[path.point_count].y = p.y;
                path.point_count++;
            }

            token = strtok(NULL, "->");
        }

        memcpy(&all_paths[path_count++],&path, sizeof(Path));
    }

    int grid_x_min = INT_MAX;
    int grid_x_max = 0;

    int grid_y_min = 0;
    int grid_y_max = 0;

    for(int i = 0; i < path_count; ++i)
    {
        for(int j = 0; j < all_paths[i].point_count; ++j)
        {
            Vec2i* p = &all_paths[i].points[j];

            if(p->x < grid_x_min)
                grid_x_min = p->x;

            if(p->x > grid_x_max)
                grid_x_max = p->x;

            if(p->y > grid_y_max)
                grid_y_max = p->y;
        }
    }

    int sand_count;

    // part 1
    sand_count = day14_simulate_sand(all_paths, path_count,-1);
    printf("1) Sand Count: %d\n",sand_count);

    // part 2
    sand_count = day14_simulate_sand(all_paths, path_count,grid_y_max+2);
    printf("2) Sand Count: %d\n",sand_count);

}

typedef struct
{
    Vec2i sensor;
    Vec2i beacon;
    int distance;
} SensorReading;

int manhatten_dist(int x1, int y1, int x2, int y2)
{
    return (ABS(x2 - x1) + ABS(y2 - y1));
}

void day15(bool test)
{
    util_print_day(15);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/15_test.txt" : "inputs/15.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char line[100+1] = {0};

    SensorReading readings[100] = {0};
    int num_readings = 0;

    // parse out sensors and beacons
    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        SensorReading r;

        sscanf(line,"Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n",&r.sensor.x, &r.sensor.y, &r.beacon.x, &r.beacon.y);

        memcpy(&readings[num_readings++],&r, sizeof(SensorReading));
    }

    // get ranges
    int x_min = INT_MAX;
    int x_max = 0;

    int y_min = INT_MAX;
    int y_max = 0;

    for(int i = 0; i < num_readings; ++i)
    {
        SensorReading* r = &readings[i];
        r->distance = manhatten_dist(r->sensor.x, r->sensor.y, r->beacon.x, r->beacon.y);

        if(r->sensor.x < x_min) x_min = r->sensor.x;
        if(r->sensor.y < y_min) y_min = r->sensor.y;
        if(r->sensor.x > x_max) x_max = r->sensor.x;
        if(r->sensor.y > y_max) y_max = r->sensor.y;

        if(r->beacon.x < x_min) x_min = r->beacon.x;
        if(r->beacon.y < y_min) y_min = r->beacon.y;
        if(r->beacon.x > x_max) x_max = r->beacon.x;
        if(r->beacon.y > y_max) y_max = r->beacon.y;
    }

    // build visual grid
#if 0
    char visual_grid[100][100] = {0};

    for(int j = y_min; j <= y_max; ++j)
    {
        for(int i = x_min; i <= x_max; ++i)
        {
            bool has_object = false;
            char c = '.';

            for(int k = 0; k < num_readings; ++k)
            {
                SensorReading* r = &readings[k];
                if(r->sensor.x == i && r->sensor.y == j)
                {
                    c = 'S';
                    break;
                }

                if(r->beacon.x == i && r->beacon.y == j)
                {
                    c = 'B';
                    break;
                }
            }

            visual_grid[i-x_min][j-y_min] = c;
        }
    }

    for(int j = 0; j <= y_max-y_min; ++j)
    {
        for(int i = 0; i <= x_max-x_min; ++i)
        {
            printf("%c",visual_grid[i][j]);
        }
        printf("\n");
    }
#endif

    // part 1
    // scan y=10 for a decent range of x
    int num_cannot_contain_beacon = 0;
    int y;
    int x_min1, x_max1;

    if(test)
    {
        y = 20;
        x_min1 = -100;
        x_max1 = 100;
    }
    else
    {
        y = 2000000;
        x_min1 = -10000000;
        x_max1 = 10000000;
    }
    for(int x = x_min1; x <= x_max1; ++x)
    {
        for(int i = 0; i < num_readings; ++i)
        {
            SensorReading* r = &readings[i];
            int dist = manhatten_dist(x,y,r->sensor.x,r->sensor.y);

            if(x == r->beacon.x && y == r->beacon.y)
                continue;
            
            if(dist <= r->distance)
            {
                num_cannot_contain_beacon++;
                //printf("x=%d\n",x);
                break;
            }
        }
    }

    printf("1) # Cannot contain beacon: %d\n",num_cannot_contain_beacon);

    // part 2
    // find the distress beacon
    // x,y can be no lower than 0 and no larger than 4,000,000
    
    /*
    // printing out points for plotting data
    for(int i = 0; i < num_readings; ++i)
    {
        SensorReading* r = &readings[i];

        float x   = r->sensor.x/1000000.0;
        float y   = r->sensor.y/1000000.0;
        float rad = r->distance/1000000.0;

        printf("(%f,%f) (%f,%f) (%f,%f) (%f,%f)\n",x-rad,y,x,y+rad,x+rad,y,x,y-rad);
    }
    */


    long tuning_freq = 0;
    int x_min2,x_max2;
    int y_min2,y_max2;

    if(test)
    {
        x_min2 = 0; x_max2 = 20;
        y_min2 = 0; y_max2 = 20;
    }
    else
    {
        // this range looked promising after plotting the sensors areas
        // So focusing on it
        // I think a programatic solution would involve finding all intesections
        // between line segments of beacon perimeters, and then searching a small
        // range around each intersection found

        x_min2 = 2550000; x_max2 = 2580000;
        y_min2 = 3250000; y_max2 = 3280000;
    }

    for(int x = x_min2; x <= x_max2; ++x)
    {
        for(int y = y_min2; y <= y_max2; ++y)
        {
            bool possible_beacon = true;

            for(int i = 0; i < num_readings; ++i)
            {
                SensorReading* r = &readings[i];
                int dist = manhatten_dist(x,y,r->sensor.x,r->sensor.y);

                if(dist <= r->distance)
                {
                    //printf("dist %d not greater than radius of %d, sensor @ %d, %d, r= %d\n", dist, r->distance, r->sensor.x,r->sensor.y, r->distance);
                    possible_beacon = false;
                    break;
                }

            }

            if(possible_beacon)
            {
                // 8355556222683
                //printf("Possible Beacon at %d, %d (%ld)\n",x,y,(long)x*4000000 + y);
                tuning_freq = (long)x*4000000+y;
                goto found_beacon;
            }
        }
    }

found_beacon:
    printf("2) Tuning Frequency: %ld\n",tuning_freq);

}

typedef struct Valve Valve;
struct Valve
{
    char name[3];
    char tunnel_str[20];
    int flow_rate;
    int tunnel_count;
    struct Valve* tunnels[8];
    bool open;
    bool sim_open; // for projected simulation
};

int evaluate_valve_route(Valve* v, int* total_flow_rate, int* minutes_left, int* depth, const int max_depth, bool open_the_valve, bool* should_open)
{
    //printf("->%s",v->name);
    if(!v->sim_open && open_the_valve)
    {
        //printf("(open)");
        (*total_flow_rate) += (v->flow_rate * ((*minutes_left)-1));
        v->sim_open = true;
        (*minutes_left)--;
    }

    (*depth)++;
    if(*depth >= max_depth || *minutes_left <= 0)
    {
        //printf("  Hit depth. Total flow rate: %d\n",*total_flow_rate);
        return 0; // discarded
    }

    // dive into each tunnel
    int winning_tunnel = 0;
    int max_flow_rate = 0;

    for(int i = 0; i < v->tunnel_count; ++i)
    {
        if(*depth == 1) printf("Testing valve %d (%s, flow_rate: %d)\n",i,v->tunnels[i]->name, v->tunnels[i]->flow_rate);

        int test_rate1 = *total_flow_rate;
        int test_depth1 = *depth;
        int test_minutes_left1 = (*minutes_left) - 1;

        evaluate_valve_route(v->tunnels[i],&test_rate1,&test_minutes_left1, &test_depth1, max_depth,false,NULL); // test not opening the valve

        if(test_rate1 > max_flow_rate)
        {
            max_flow_rate = test_rate1;
            winning_tunnel = i;
        }

        int test_rate2 = *total_flow_rate;
        if(!v->tunnels[i]->sim_open && v->tunnels[i]->flow_rate > 0)
        {
            int test_depth2 = *depth;
            int test_minutes_left2 = (*minutes_left) - 1;

            evaluate_valve_route(v->tunnels[i],&test_rate2,&test_minutes_left2, &test_depth2, max_depth,true,NULL); // test opening the valve

            if(test_rate2 > max_flow_rate)
            {
                max_flow_rate = test_rate2;
                winning_tunnel = i;

                if(should_open)
                    *should_open = true;
            }
        }

        if(v->tunnels[i]->sim_open != v->tunnels[i]->open)
            v->tunnels[i]->sim_open = v->tunnels[i]->open; // reset the simulated open state

        if(*depth == 1) printf("Rate (closed): %d, Rate (open): %d\n",test_rate1,test_rate2);
        if(*depth == 1) util_wait_until_key_press();

    }

    *total_flow_rate = max_flow_rate; // set total flow rate to max of children tunnels

    //if(*depth == 1) printf("Winning tunnel: %d\n",winning_tunnel);

    return winning_tunnel;
}

void day16()
{
    util_print_day(16);

    char* input_file = "inputs/16_test.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Valve valves[50] = {0};
    int valve_count = 0;

    char line[100+1] = {0};
    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        Valve v = {0};

        int num_matches = sscanf(line, "Valve %s has flow rate=%d;",&v.name,&v.flow_rate);

        // get tunnels
        char* tunnels = strstr(line,";");
        if(!tunnels)
            continue;

        char tunnel_str[20] = {0};
        int tunnel_str_index = 0;

        for(int i = 0; i < strlen(tunnels); ++i)
        {
            char c = tunnels[i];
            if(c >= 'A' && c <= 'Z')
            {
                // tunnel
                v.tunnel_str[tunnel_str_index++] = c;
            }
            else if(c == ',')
            {
                v.tunnel_count++;
                v.tunnel_str[tunnel_str_index++] = ',';
            }
        }
        v.tunnel_count++;

        memcpy(&valves[valve_count++],&v,sizeof(Valve));
    }

    // fill out valve pointers
    for(int i = 0; i < valve_count; ++i)
    {
        Valve* v = &valves[i];

        for(int j = 0; j < v->tunnel_count; ++j)
        {
            char tunnel_name[3] = {0};
            memcpy(tunnel_name,v->tunnel_str+(3*j),2*sizeof(char));

            for(int k = 0; k < valve_count; ++k)
            {
                if(strcmp(tunnel_name,valves[k].name) == 0)
                {
                    v->tunnels[j] = &valves[k];
                }
            }
        }
    }

    // part 1
    // simulate

    int max_total_released_pressure = 0;
    int debug = 1;
    int seed = 1000;

    const int num_loops = 1;

    for(int i = 0; i < num_loops; ++i)
    {
        srand(seed++);

        Valve* curr_valve = &valves[0];

        int m = 30;
        int total_released_pressure = 0;

        // open all valves again
        for(int j = 0; j < valve_count; ++j)
        {
            valves[j].open = false;
            valves[j].sim_open = false;
        }

        bool open_valve = false;

        for(;;)
        {
            if(m <= 0)
                break;

            bool all_valves_open = true;
            for(int j = 0; j < valve_count; ++j)
            {
                if(!valves[j].open && valves[j].flow_rate > 0)
                {
                    all_valves_open = false;
                    break;
                }
            }

            if(all_valves_open)
            {
                if(debug)printf("[%d min left] All valves open. Waiting...\n",m);
                --m;
                continue;
            }

            if(curr_valve->flow_rate > 0 && !curr_valve->open && open_valve)
            {
                // open valve
                if(debug) printf("[%d min left] Open %s (flow rate: %d)\n",m,curr_valve->name,curr_valve->flow_rate);

                curr_valve->open = true;
                total_released_pressure += (curr_valve->flow_rate*(m-1));
                --m;
            }
            
            // pick a tunnel to move to
            int projected_flow_rate1 = 0;
            int initial_depth = 0;
            int minutes_left = m;

            open_valve = false;
            int chosen_tunnel = evaluate_valve_route(curr_valve, &projected_flow_rate1, &minutes_left, &initial_depth, 10,open_valve,&open_valve); // don't open valve

            //int tunnel = rand() % (curr_valve->tunnel_count);
            curr_valve = curr_valve->tunnels[chosen_tunnel];
            if(debug) printf("[%d min left] Move to %s\n",m,curr_valve->name);
            --m;
        }

        if(debug) printf("total pressure: %d\n", total_released_pressure);

        if(total_released_pressure > max_total_released_pressure)
        {
            max_total_released_pressure = total_released_pressure;
        }
    }

    printf("Total Released Pressure: %d\n",max_total_released_pressure);
}

#define BOARD_MAX_HEIGHT 100000

typedef struct
{
    const char* data;
    int x,y;
} Shape;

void draw_board(char* board, int max_height, const int board_width)
{
    for(int y = max_height + 3 + 4; y >= 0; --y)
    {
        if(y == 0) printf("+"); else printf("|"); // wall/floor

        for(int x = 0; x < board_width; ++x)
        {
            if(y == 0)
                printf("-");
            else 
                printf("%c", board[(y*7)+x]);
        }
        if(y == 0) printf("+"); else printf("|\n"); // wall/floor
    }
    printf(" max_height: %d\n",max_height);
}

bool sim_shape_on_board(char* board, Shape* s, bool draw)
{
    int draw_x = s->x;
    int draw_y = s->y;

    bool collision = false;

    for(int i = 0; i < strlen(s->data); ++i)
    {
        char c = s->data[i];

        if(c == '\n')
        {
            draw_y--;
            draw_x = s->x;
            continue;
        }

        if(c == '#')
        {
            int index = (draw_y*7)+draw_x;
            if(board[index] == '#')
            {
                // collision
                collision = true;
            }

            if(draw)
            {
                board[index] = c;
            }
        }

        draw_x++;
    }

    return collision;

}

bool get_highest_tetris(char* board, int max_height, int* line)
{
    for(int i = max_height; i >= 1; --i)
    {
        int index = i*7;
        if(board[index+0] == '#' && board[index+1] == '#' && board[index+2] == '#' &&
           board[index+3] == '#' && board[index+4] == '#' && board[index+5] == '#' && board[index+6] == '#')
        {
            *line = i;
            return true;
        }
    }
    return false;
}

int day17_simulate(char* air_flow, int air_flow_count, int num_rocks)
{
    const char* shapes[5] = {
        "####", // -
        ".#.\n###\n.#.", //+
        "..#\n..#\n###", // L
        "#\n#\n#\n#", // |
        "##\n##" // square
    };

    const PathPos shape_sizes[5] = {
        {4,1}, // - 
        {3,3}, // +
        {3,3}, // L
        {1,4}, // |
        {2,2}  // square
    };

    // initialize board
    char* base_board = malloc(BOARD_MAX_HEIGHT*7*sizeof(char));
    char* board = malloc(BOARD_MAX_HEIGHT*7*sizeof(char));

    memset(base_board,'.',BOARD_MAX_HEIGHT*7*sizeof(char));
    memset(board,'.',BOARD_MAX_HEIGHT*7*sizeof(char));
    
    int max_height = 0;
    long tetris_max_height = 0;
    const int board_width = 7;
    bool air_turn = true;
    int air_index = 0;

    PathPos prior_move = {0,0};

    const char debug = 0;

    //for(int b = 0; b < 2022; ++b)

    for(int b = 0; b < num_rocks; ++b)
    {
        if(debug && ((b % 1000000) == 0))
        {
            printf("# Rocks: %d\n",b);
        }

        int shape_index = b % 5;
        PathPos shape_size = shape_sizes[shape_index];
        Shape curr_shape = {shapes[shape_index],2,max_height+3+shape_size.y};

        for(;;)
        {
            // clear current piece
            memcpy(board,base_board,(max_height+8)*7*sizeof(char));

            // try piece on board
            bool collision = sim_shape_on_board(board,&curr_shape, false);

            if(collision)
            {
                // resolve collision
                if(debug) printf("Collision!\n");

                curr_shape.x -= prior_move.x;
                curr_shape.y -= prior_move.y;

                // draw piece
                sim_shape_on_board(board,&curr_shape, true);

                if(prior_move.y < 0)
                {
                    // final location
                    break;
                }
            }
            else
            {
                // draw piece
                sim_shape_on_board(board,&curr_shape, true);
            }

            // draw board
            if(debug) draw_board(board,max_height, board_width);

            // wait for frame
            if(debug) util_wait_until_key_press();

            // simulate
            if(air_turn)
            {
                // push piece
                char a = air_flow[air_index%air_flow_count];
                if(debug) printf("%c (%d/%d)\n",a,air_index, air_flow_count);

                prior_move.y = 0;

                if(a == '<' && curr_shape.x > 0)
                {
                    prior_move.x = -1;
                    curr_shape.x--;
                }
                if(a == '>' && curr_shape.x + shape_size.x < board_width)
                {
                    prior_move.x = 1;
                    curr_shape.x++;
                }
                air_index++;
                air_turn = false;
            }
            else
            {
                // shape falls
                curr_shape.y--;
                prior_move.x = 0;
                prior_move.y = -1;
                air_turn = true;
            }
           
            // checks
            if(curr_shape.y - shape_size.y + 1 <= 0)
            {
                // hit floor
                curr_shape.y++;
                break;
            }
        }

        // solidify the last piece as part of the board
        memcpy(base_board,board,(max_height+8)*7*sizeof(char));

        if(curr_shape.y > max_height)
            max_height = curr_shape.y;

        int line;
        bool tetris = get_highest_tetris(board, max_height, &line);

        if(tetris)
        {
            bool db = false;

            if((b+1) % (5*10091) == 0) // checking for divisibility by airflow pattern and shape pattern
            {
                printf("Pattern Repeat on Tetris. (Current Max Height: %d, # rocks: %d)\n",max_height+tetris_max_height,b);
            }

            // discard everything below tetris line
            memcpy(base_board, &base_board[line*7],sizeof(char)*7*max_height-line);
            memset(&base_board[7*(max_height-line+1)],'.',sizeof(char)*7*max_height);

            tetris_max_height = tetris_max_height + line;
            max_height -= line;
        }
    }

    return max_height+tetris_max_height;
}

void day17()
{
    util_print_day(17);

    char* input_file = "inputs/17.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char air_flow[20000] = {0};
    int air_flow_count = 0;

    // parse input file
    for(;;)
    {
        int c = fgetc(fp);
        if(c == EOF)
            break;

        if(c == '<' || c == '>')
            air_flow[air_flow_count++] = c;
    }

    int p1 = day17_simulate(air_flow,air_flow_count, 2022);
    //int p2 = day17_simulate(air_flow,air_flow_count, 1000*1000000); // commenting out due to runtime

    printf("1) Max Height: %d\n", p1);
    printf("2) Max Height: %ld\n", (long)1564705882327);

    // My method for getting part2:
    // (1) Find max height right before first "tetris" that occurs on index 5*10091
    // (2) Find max height between two consecutive "tetris" that occur on index 5*10091
    // (3) Find max height for difference between final block of (2) and index 1 trillion
    // compute floor((1000000000000 - (1))/(index of (2)) * (2)
    // Add (1) and (3)

    // 1,564,704,837,460 (too low)

    // 1,564,705,882,327 (right!)
    // 1,564,705,882,338 (wrong)
    // 1,564,705,882,352 (too high)
    
}

typedef struct
{
    int x,y,z;
} Vec3i;

typedef struct
{
    Vec3i points[10000];
    int point_count;
} Vec3iStack;

void vec3i_enqueue(Vec3iStack* q, int x, int y, int z)
{
    q->points[q->point_count].x = x;
    q->points[q->point_count].y = y;
    q->points[q->point_count].z = z;
    q->point_count++;
}

void vec3i_dequeue(Vec3iStack* q, int* x, int* y, int* z)
{
    *x = q->points[q->point_count-1].x;
    *y = q->points[q->point_count-1].y;
    *z = q->points[q->point_count-1].z;

    q->point_count--;
}

bool vec3i_empty(Vec3iStack* q)
{
    return (q->point_count == 0);
}

int is_point_in_list(int x, int y, int z, Vec3i* points,int point_count)
{
    for(int i = 0; i < point_count; ++i)
    {
        Vec3i* point = &points[i];

        if(x == point->x && y == point->y && z == point->z)
        {
            return 1;
        }
    }
    return 0;
}

bool is_point_trapped(int x, int y, int z, Vec3i* points, int point_count)
{
    if(is_point_in_list(x,y,z,points,point_count)) // already on points list
        return false;

    Vec3iStack q = {0};

    Vec3i visited_list[10000] = {0};
    int visited_list_count = 0;

    Vec3i curr = {x,y,z};
    vec3i_enqueue(&q,curr.x,curr.y,curr.z);

    bool debug = false;//(x == 6 && y == 12 && z == 14);

    while(!vec3i_empty(&q))
    {
        vec3i_dequeue(&q,&curr.x,&curr.y,&curr.z);

        if(debug) printf("Stack Size: %d, Checking %d, %d, %d\n",q.point_count, curr.x,curr.y,curr.z);

        // check if point is outside range of lava droplet
        if(curr.x < 0 || curr.y < 0 || curr.z < 0)
            return false;

        if(curr.x > 19 || curr.y > 19 || curr.z > 19)
            return false;

        if(is_point_in_list(curr.x,curr.y,curr.z,visited_list,visited_list_count)) // already visited
        {
            if(debug) printf("Already visited. continuing...\n");
            continue;
        }

        // copy in already visited
        memcpy(&visited_list[visited_list_count++],&curr,sizeof(Vec3i));

        if(is_point_in_list(curr.x,curr.y,curr.z,points,point_count))
        {
            if(debug) printf("Hit lava material\n");
            continue; // hit lava material or already trapped air
        }

        // point is unexplored air
        vec3i_enqueue(&q,curr.x+1,curr.y+0,curr.z+0);
        vec3i_enqueue(&q,curr.x-1,curr.y+0,curr.z+0);
        vec3i_enqueue(&q,curr.x+0,curr.y+1,curr.z+0);
        vec3i_enqueue(&q,curr.x+0,curr.y-1,curr.z+0);
        vec3i_enqueue(&q,curr.x+0,curr.y+0,curr.z+1);
        vec3i_enqueue(&q,curr.x+0,curr.y+0,curr.z-1);
    }

    return true;
}

void day18(bool test)
{
    util_print_day(18);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/18_test.txt" : "inputs/18.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Valve valves[50] = {0};
    int valve_count = 0;

    char line[100+1] = {0};

    Vec3i points[10000] = {0};
    int point_count = 0;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        Vec3i* v = &points[point_count];
        int num_matches = sscanf(line, "%d,%d,%d\n",&v->x,&v->y,&v->z);
        point_count++;
    }

    // part 1
    // find all exposed sides

    int total_sides_exposed = 0;
    Vec3i dimensions = {0};

    for(int i = 0; i < point_count; ++i)
    {
        Vec3i* p = &points[i];
        
        int u = is_point_in_list(p->x, p->y-1, p->z, points,point_count);
        int d = is_point_in_list(p->x, p->y+1, p->z, points,point_count);
        int l = is_point_in_list(p->x-1, p->y, p->z, points,point_count);
        int r = is_point_in_list(p->x+1, p->y, p->z, points,point_count);
        int f = is_point_in_list(p->x, p->y, p->z-1, points,point_count);
        int b = is_point_in_list(p->x, p->y, p->z+1, points,point_count);

        int exposed_sides = 6 - u - d - l - r - f - b;
        
        //printf("Cube %d (%d,%d,%d) has %d sides exposed\n",i,p->x,p->y,p->z,exposed_sides);

        total_sides_exposed += exposed_sides;

        if(p->x > dimensions.x)
            dimensions.x = p->x;
        if(p->y > dimensions.y)
            dimensions.y = p->y;
        if(p->z > dimensions.z)
            dimensions.z = p->z;

    }

    printf("1) Total surface area: %d\n",total_sides_exposed);

    // part 2
    // Get only exterior faces trapped air cubes

    int air_count = 0;

    //printf("dimensions: %d %d %d\n",dimensions.x,dimensions.y,dimensions.z);

    for(int i = 0; i < dimensions.x; ++i)
    {
        for(int j = 0; j < dimensions.y; ++j)
        {
            for(int k = 0; k < dimensions.z; ++k)
            {
                bool trapped = is_point_trapped(i,j,k, points, point_count);

                //util_wait_until_key_press();

                if(trapped)
                {
                    // trapped air
                    // add to list
                    points[point_count].x = i;
                    points[point_count].y = j;
                    points[point_count].z = k;
                    point_count++;
                    air_count++;
                    //printf("Trapped air! (%d, %d, %d)\n",i,j,k);
                }
            }
        }
    }

    //printf("Trapped air count: %d\n",air_count);

    int total_sides_exposed2 = 0;

    for(int i = 0; i < point_count; ++i)
    {
        Vec3i* p = &points[i];
        
        int u = is_point_in_list(p->x, p->y-1, p->z, points,point_count);
        int d = is_point_in_list(p->x, p->y+1, p->z, points,point_count);
        int l = is_point_in_list(p->x-1, p->y, p->z, points,point_count);
        int r = is_point_in_list(p->x+1, p->y, p->z, points,point_count);
        int f = is_point_in_list(p->x, p->y, p->z-1, points,point_count);
        int b = is_point_in_list(p->x, p->y, p->z+1, points,point_count);

        int exposed_sides = 6 - u - d - l - r - f - b;
        
        //printf("Cube %d (%d,%d,%d) has %d sides exposed\n",i,p->x,p->y,p->z,exposed_sides);

        total_sides_exposed2 += exposed_sides;

    }

    printf("2) Total surface area: %d\n",total_sides_exposed2);

}

typedef struct
{
    int cost_ore;
    int cost_clay;
    int cost_obsidian;
} Robot;

typedef struct
{
    int id;
    Robot robot_ore;
    Robot robot_clay;
    Robot robot_obsidian;
    Robot robot_geode;
} Blueprint;

enum MaterialTarget
{
    TARGET_ORE,
    TARGET_CLAY,
    TARGET_OBSIDIAN,
    TARGET_GEODE,
};

void day19(bool test)
{
    util_print_day(19);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/19_test.txt" : "inputs/19.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    Blueprint blueprints[50] = {0};
    int blueprint_count = 0;

    for(;;)
    {
        Blueprint* bp = &blueprints[blueprint_count];

        int num_matches = fscanf(fp, "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.\n",&bp->id, &bp->robot_ore.cost_ore, &bp->robot_clay.cost_ore, &bp->robot_obsidian.cost_ore, &bp->robot_obsidian.cost_clay, &bp->robot_geode.cost_ore, &bp->robot_geode.cost_obsidian);

        if(num_matches <= 0)
            break;

        blueprint_count++;
    }

    printf("Num Blueprints: %d\n",blueprint_count);

    for(int i = 0; i < blueprint_count; ++i)
    {
        Blueprint* bp = &blueprints[i];

        printf("\n========= Blueprint %d ==========\n",bp->id);
        printf("  Ore Robot Cost:      [%02d %02d %02d]\n", bp->robot_ore.cost_ore, bp->robot_ore.cost_clay, bp->robot_ore.cost_obsidian);
        printf("  Clay Robot Cost:     [%02d %02d %02d]\n", bp->robot_clay.cost_ore, bp->robot_clay.cost_clay, bp->robot_clay.cost_obsidian);
        printf("  Obsidian Robot Cost: [%02d %02d %02d]\n", bp->robot_obsidian.cost_ore, bp->robot_obsidian.cost_clay, bp->robot_obsidian.cost_obsidian);
        printf("  Geode Robot Cost:    [%02d %02d %02d]\n", bp->robot_geode.cost_ore, bp->robot_geode.cost_clay, bp->robot_geode.cost_obsidian);
        printf("=================================\n");

        int total_required_ore = bp->robot_ore.cost_ore + bp->robot_clay.cost_ore + bp->robot_obsidian.cost_ore + bp->robot_geode.cost_ore;
        int total_required_clay = bp->robot_ore.cost_clay + bp->robot_clay.cost_clay + bp->robot_obsidian.cost_clay + bp->robot_geode.cost_clay;
        int total_required_obsidian = bp->robot_ore.cost_obsidian + bp->robot_clay.cost_obsidian + bp->robot_obsidian.cost_obsidian + bp->robot_geode.cost_obsidian;

        int max_required_ore = MAX(MAX(MAX(bp->robot_ore.cost_ore, bp->robot_clay.cost_ore),bp->robot_obsidian.cost_ore),bp->robot_geode.cost_ore);
        int max_required_clay = MAX(MAX(MAX(bp->robot_ore.cost_clay, bp->robot_clay.cost_clay),bp->robot_obsidian.cost_clay),bp->robot_geode.cost_clay);
        int max_required_obsidian = MAX(MAX(MAX(bp->robot_ore.cost_obsidian, bp->robot_clay.cost_obsidian),bp->robot_obsidian.cost_obsidian),bp->robot_geode.cost_obsidian);
        int max_required_total = max_required_ore + max_required_clay + max_required_obsidian;

        float ideal_ratio_ore = max_required_ore / (float)max_required_total;
        float ideal_ratio_clay = max_required_clay / (float)max_required_total;
        float ideal_ratio_obsidian = max_required_obsidian / (float)max_required_total;

        printf("totals needed: %02d, %02d, %02d\n", total_required_ore, total_required_clay, total_required_obsidian);
        printf("maxes  needed: %02d, %02d, %02d\n", max_required_ore, max_required_clay, max_required_obsidian);
        printf("ideal  ratios: %f, %f, %f\n", ideal_ratio_ore, ideal_ratio_clay, ideal_ratio_obsidian);

        // init
        int count_ore = 0;
        int count_clay = 0;
        int count_obsidian = 0;
        int count_geode = 0;

        int count_ore_robot = 1;
        int count_clay_robot = 0;
        int count_obsidian_robot = 0;
        int count_geode_robot = 0;

        // simulate
        int m = 24;

        for(;;)
        {
            if(m <= 0)
                break;

            // mine resources
            count_ore      += count_ore_robot;
            count_clay     += count_clay_robot;
            count_obsidian += count_obsidian_robot;
            count_geode    += count_geode_robot;

            // check robot affordability
            bool can_afford_ore_robot      = (bp->robot_ore.cost_ore <= count_ore);
            bool can_afford_clay_robot     = (bp->robot_clay.cost_ore <= count_ore && count_clay_robot < bp->robot_obsidian.cost_clay);
            bool can_afford_obsidian_robot = (bp->robot_obsidian.cost_ore <= count_ore && bp->robot_obsidian.cost_clay <= count_clay);
            bool can_afford_geode_robot    = (bp->robot_geode.cost_ore <= count_ore && bp->robot_geode.cost_obsidian <= count_obsidian);

            printf("[%02d min] [Mined %d %d %d %d], [Robots %d,%d,%d,%d], [Afford? %d %d %d %d]\n", m, count_ore, count_clay, count_obsidian, count_geode,count_ore_robot,count_clay_robot, count_obsidian_robot, count_geode_robot, can_afford_ore_robot, can_afford_clay_robot, can_afford_obsidian_robot, can_afford_geode_robot);

            bool build_ore = false;
            bool build_clay = false;
            bool build_obsidian = false;
            bool build_geode = false;

            // check robot costs
            if(can_afford_geode_robot)
            {
                printf("Buying Geode Robot!\n");
                // always buy a geode robot if you can afford it
                count_ore  -= bp->robot_geode.cost_ore;
                count_obsidian -= bp->robot_geode.cost_obsidian;
                build_geode = true;
            }
            else
            {
                // pick target material
                int target = 0;

                if(count_clay_robot == 0)
                {
                    target = TARGET_CLAY;
                }
                else
                {
                    int proj_output_ore = m * count_ore_robot + count_ore;
                    int proj_output_clay = m * count_clay_robot + count_clay;
                    int proj_output_obsidian = m * count_obsidian_robot + count_obsidian;
                    int proj_output_total = proj_output_ore + proj_output_clay + proj_output_obsidian;

                    float proj_ratio_ore = proj_output_ore / (float)proj_output_total;
                    float proj_ratio_clay = proj_output_clay / (float)proj_output_total;
                    float proj_ratio_obsidian = proj_output_obsidian / (float)proj_output_total;

                    float diff_ratio_ore = ideal_ratio_ore - proj_ratio_ore;
                    float diff_ratio_clay = ideal_ratio_clay - proj_ratio_clay;
                    float diff_ratio_obsidian = ideal_ratio_obsidian - proj_ratio_obsidian;

                    if(diff_ratio_ore > diff_ratio_clay && diff_ratio_ore > diff_ratio_obsidian)
                    {
                        target = TARGET_ORE;
                    }
                    else if(diff_ratio_clay > diff_ratio_ore && diff_ratio_clay > diff_ratio_obsidian)
                    {
                        target = TARGET_CLAY;
                    }
                    else
                    {
                        target = TARGET_OBSIDIAN;
                    }
                }

                if(target == TARGET_ORE)
                {
                    if(can_afford_ore_robot)
                    {
                        // Ore Robot
                        printf("Buying Ore Robot!\n");
                        count_ore -= bp->robot_ore.cost_ore;
                        build_ore = true;
                    }
                }
                else if(target == TARGET_CLAY)
                {
                    if(can_afford_clay_robot)
                    {
                        // Clay Robot
                        printf("Buying Clay Robot!\n");
                        count_ore -= bp->robot_clay.cost_ore;
                        build_clay = true;
                    }
                }
                else if(target == TARGET_OBSIDIAN)
                {
                    if(can_afford_obsidian_robot)
                    {
                        // Obsidian Robot
                        printf("Buying Obsidian Robot!\n");
                        count_ore  -= bp->robot_obsidian.cost_ore;
                        count_clay -= bp->robot_obsidian.cost_clay;
                        build_obsidian = true;
                    }
                }
            }

            if(build_ore || build_clay || build_obsidian || build_geode)
            {

                // mine resources
                count_ore      += count_ore_robot;
                count_clay     += count_clay_robot;
                count_obsidian += count_obsidian_robot;
                count_geode    += count_geode_robot;

                --m;

                if(build_ore) count_ore_robot++;
                else if(build_clay) count_clay_robot++;
                else if(build_obsidian) count_obsidian_robot++;
                else if(build_geode) count_geode_robot++;

                printf("[%02d min] [Mined %d %d %d %d], [Robots %d,%d,%d,%d], [Afford? %d %d %d %d]\n", m, count_ore, count_clay, count_obsidian, count_geode,count_ore_robot,count_clay_robot, count_obsidian_robot, count_geode_robot, can_afford_ore_robot, can_afford_clay_robot, can_afford_obsidian_robot, can_afford_geode_robot);
            }

            --m;
        }

        printf("Geode count: %d\n",count_geode);

    }

    
}

typedef struct
{
    long static_items[5000];
    long items[5000];
    long mapping[5000];
    int count;
} CircArray;

void circ_array_shift_item(CircArray* array, int index, long amount)
{
    //printf("Moving index %d by %ld\n",index,amount);

    if(amount < 0)
    {
        for(long i = amount; i < 0; ++i)
        {
            int swap_index = (index - 1);
            bool shift = false;
            if(swap_index < 0) {
                swap_index = array->count-2;
                shift = true;
            }
            
            long swap = array->items[index];
            long map_swap = array->mapping[index];

            if(shift)
            {
                for(int j = 1; j < array->count; ++j)
                {
                    array->items[j-1] = array->items[j];
                    array->mapping[j-1] = array->mapping[j];
                }
            }

            if(!shift) {
                array->items[index] = array->items[swap_index];
                array->mapping[index] = array->mapping[swap_index];
            }

            array->items[swap_index] = swap;
            array->mapping[swap_index] = map_swap;

            //printf("  ");
            //circ_array_print(array);

            index = swap_index;
        }
    }
    else
    {
        for(long i = 0; i < amount; ++i)
        {
            int swap_index = (index + 1);

            bool shift = false;
            if(swap_index >= array->count) {
                swap_index = 1;
                shift = true;
            }

            long swap = array->items[index];
            long map_swap = array->mapping[index];

            if(shift)
            {
                for(int j = array->count-1; j > 0; --j)
                {
                    array->items[j] = array->items[j-1];
                    array->mapping[j] = array->mapping[j-1];
                }
            }

            if(!shift) {
                array->items[index] = array->items[swap_index];
                array->mapping[index] = array->mapping[swap_index];
            }

            array->items[swap_index] = swap;
            array->mapping[swap_index] = map_swap;

            //printf("  ");
            //circ_array_print(array);

            index = swap_index;
        }
    }
}

void circ_array_print(CircArray* array)
{
    for(int i = 0; i < array->count; ++i)
    {
        printf("%ld ", array->items[i]);
    }
    printf("[ ");
    for(int i = 0; i < array->count; ++i)
    {
        printf("%ld ", array->mapping[i]);
    }
    printf("]\n");
}

void day20(bool test)
{
    util_print_day(20);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/20_test.txt" : "inputs/20.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    CircArray number_list = {0};

    char line[20+1] = {0};

    for(;;)
    {
        if(fgets(line,20,fp) == NULL)
            break;

        int n = atoi(line);

        number_list.items[number_list.count] = n;
        number_list.static_items[number_list.count] = n;
        number_list.mapping[number_list.count] = number_list.count;
        number_list.count++;
    }

    //circ_array_print(&number_list);
    
    // mix
    for(int i = 0; i < number_list.count; ++i)
    {
        int map_index = util_get_index_of_value_long(i,number_list.mapping,number_list.count);
        circ_array_shift_item(&number_list, map_index, number_list.static_items[i] % (number_list.count-1));
        //circ_array_print(&number_list);
    }

    // find 0
    int index_of_zero = 0;
    for(int i = 0; i < number_list.count; ++i)
    {
        if(number_list.items[i] == 0)
        {
            index_of_zero = i;
            break;
        }
    }

    long x = number_list.items[(index_of_zero+1000) % number_list.count];
    long y = number_list.items[(index_of_zero+2000) % number_list.count];
    long z = number_list.items[(index_of_zero+3000) % number_list.count];

    //printf("x: %ld, y: %ld, z: %ld\n",x,y,z);

    printf("1) Grove Coord Sum: %ld\n",x+y+z);

    // part 2

    const int decryption_key = 811589153; 

    // set up 
    for(int i = 0; i < number_list.count; ++i)
    {
        number_list.static_items[i] = (long)(number_list.static_items[i] * decryption_key);
        number_list.items[i] = number_list.static_items[i];
        number_list.mapping[i] = i;
    }

    //circ_array_print(&number_list);

    // 10 rounds
    for(int r = 0; r < 10; ++r)
    {
        // mix
        for(int i = 0; i < number_list.count; ++i)
        {
            int map_index = util_get_index_of_value_long(i,number_list.mapping,number_list.count);
            circ_array_shift_item(&number_list, map_index, number_list.static_items[i] % (number_list.count-1));
            //circ_array_print(&number_list);
        }

        //circ_array_print(&number_list);
    }
    
    // find 0
    index_of_zero = 0;
    for(int i = 0; i < number_list.count; ++i)
    {
        if(number_list.items[i] == 0)
        {
            index_of_zero = i;
            break;
        }
    }

    x = number_list.items[(index_of_zero+1000) % number_list.count];
    y = number_list.items[(index_of_zero+2000) % number_list.count];
    z = number_list.items[(index_of_zero+3000) % number_list.count];

    //printf("x: %ld, y: %ld, z: %ld\n",x,y,z);

    printf("2) Grove Coord Sum: %ld\n",x+y+z);

}

typedef struct MathMonkey MathMonkey;

struct MathMonkey
{
    char name[4+1];
    bool has_number;
    long number;
    char dep1[4+1];
    char dep2[4+1];
    struct MathMonkey* m1;
    struct MathMonkey* m2;
    struct MathMonkey* parent;
    char operation;
};

void day21_resolve_math(MathMonkey* m)
{
    if(m->has_number)
        return;

    day21_resolve_math(m->m1);
    day21_resolve_math(m->m2);

    switch(m->operation)
    {
        case '+':
            m->number = m->m1->number + m->m2->number;
            break;
        case '-':
            m->number = m->m1->number - m->m2->number;
            break;
        case '*':
            m->number = m->m1->number * m->m2->number;
            break;
        case '/':
            m->number = m->m1->number / m->m2->number;
            break;
    }

    m->has_number = true;
}

void day21_collect_monkeys(FILE* fp, MathMonkey* monkeys, int* monkey_count, MathMonkey** root, MathMonkey** humn)
{
    char line[100] = {0};

    for(;;)
    {
        MathMonkey* monkey = &monkeys[*monkey_count];

        if(fgets(line,100,fp) == NULL)
            break;

        int matches = sscanf(line,"%c%c%c%c: %d\n",&monkey->name[0], &monkey->name[1], &monkey->name[2], &monkey->name[3], &monkey->number);
        if(matches == 5)
        {
            monkey->has_number = true;
        }
        else
        {
            matches = sscanf(line,"%c%c%c%c: %s %c %s\n",&monkey->name[0], &monkey->name[1], &monkey->name[2], &monkey->name[3], &monkey->dep1, &monkey->operation, &monkey->dep2);
        }

        (*monkey_count)++;
    }

    for(int i = 0; i < *monkey_count; ++i)
    {
        MathMonkey* m = &monkeys[i];

        if(strcmp(m->name,"root") == 0)
            *root = m;
        if(strcmp(m->name,"humn") == 0)
            *humn = m;

        if(m->has_number)
            continue;

        // search for monkey to link to
        for(int j = 0; j < *monkey_count; ++j)
        {
            MathMonkey* m2 = &monkeys[j];
            if(strcmp(m->dep1,m2->name) == 0)
            {
                m->m1 = m2;
                m2->parent = m;
            }

            if(strcmp(m->dep2,m2->name) == 0)
            {
                m->m2 = m2;
                m2->parent = m;
            }
        }
    }
}

void day21(bool test)
{
    util_print_day(21);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/21_test.txt" : "inputs/21.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    // gather math monkeys

    MathMonkey monkeys[3000] = {0};
    int monkey_count = 0;
    MathMonkey* root;
    MathMonkey* humn;

    // part 1
    day21_collect_monkeys(fp, monkeys, &monkey_count, &root, &humn);
    day21_resolve_math(root);
    printf("1) Root Number: %ld\n",root->number);

    // reset
    monkey_count = 0;
    fseek(fp,0,SEEK_SET);
    memset(monkeys,0,3000*sizeof(MathMonkey));

    // part 2
    day21_collect_monkeys(fp, monkeys, &monkey_count, &root, &humn);

    // just guessed and iterated here
    long humn_number = 3379022190351;
    humn->number = humn_number;
    root->operation = '-';
    day21_resolve_math(root);

    //printf("Root: %ld\n",root->number); // should be zero
    printf("2) Equality Val: %ld\n",humn_number);

#if 0
    printf("Monkeys (%d):\n",monkey_count);
    for(int i = 0; i < monkey_count; ++i)
    {
        MathMonkey* m = &monkeys[i];

        printf("  Monkey %d:\n",i);
        printf("    Name:   %s\n",m->name);
        printf("    Has Number: %d\n",m->has_number);
        printf("    Number: %ld\n",m->number);
        printf("    Monkey 1: %s\n",m->m1->name);
        printf("    Monkey 2: %s\n",m->m2->name);
        printf("    Operation: %c\n",m->operation);
        printf("\n");
    }
#endif

}

typedef struct
{
    int spaces;
    bool turn_left;
    bool turn_right;
} InstructionStep;

typedef enum
{
    DIR_RIGHT = 0,
    DIR_DOWN = 1,
    DIR_LEFT = 2,
    DIR_UP = 3,
} FacingDir;

const char* map_facing_dir_to_str(FacingDir dir)
{
    switch(dir)
    {
        case DIR_RIGHT: return "Right";
        case DIR_DOWN:  return "Down";
        case DIR_LEFT:  return "Left";
        case DIR_UP:    return "Up";
    }
    return "Unknown";
}

void day22_print_grid(char grid[300][300], int grid_width, int grid_height, int player_x, int player_y)
{
    printf("grid size: %d, %d\n",grid_width, grid_height);

    for(int i = 0; i < grid_height; ++i)
    {
        for(int j = 0; j < grid_width; ++j)
        {
            if(j == player_x && i == player_y)
            {
                printf("P");
                continue;
            }
            printf("%c", grid[j][i]);
        }
        printf("\n");
    }
    printf("\n");

}

void day22(bool test)
{
    util_print_day(22);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/22_test.txt" : "inputs/22.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char grid[300][300];
    memset(grid,' ',300*300*sizeof(char));

    uint8_t grid_faces[300][300];
    memset(grid_faces,0,300*300*sizeof(uint8_t));

    PathPos grid_corners[6];
    memset(grid_corners,-1,6*sizeof(PathPos));

    int grid_width = 0;
    int grid_height = 0;

    int x = 0;
    int y = 0;

    int cube_face_len = test ? 4 : 50;

    int prio_cube_face = 0;
    int curr_cube_face = 1;

    int latest_cube = 1;
    int cube_face_count = 0;

    // get map
    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(c == '\n')
        {
            if(x == 0) // 2 newlines in a row, assume next line is instruction
                break;

            if(x > grid_width)
            {
                grid_width = x;
            }

            y++;
            if(y % cube_face_len == 0)
            {
                if(x % cube_face_len == 0)
                {
                    latest_cube = curr_cube_face;
                }
            }

            x = 0;
            cube_face_count = 0;
            curr_cube_face = latest_cube;
            continue;
        }

        if(c == '.' || c == '#')
        {
            grid[x][y] = c;
            grid_faces[x][y] = curr_cube_face;

            if(curr_cube_face == prio_cube_face+1)
            {
                if(grid_corners[prio_cube_face].x == -1)
                {
                    grid_corners[prio_cube_face].x = x;
                    grid_corners[prio_cube_face].y = y;
                }
                prio_cube_face = curr_cube_face;
            }

            cube_face_count++;
            if(cube_face_count % cube_face_len == 0)
            {
                curr_cube_face++;
            }

        }
        x++;
    }

    grid_height = y;

    InstructionStep steps[6000] = {0};
    int num_steps = 0;

    // read instruction
    char num_str[5] = {0};
    int num_str_index = 0;

    for(;;)
    {
        int c = fgetc(fp);
        
        if(c == EOF)
            break;

        if(c >= '0' && c <= '9')
        {
            num_str[num_str_index++] = c;
            continue;
        }

        InstructionStep* step = &steps[num_steps];

        step->spaces = atoi(num_str);

        if(c == 'L')
        {
            step->turn_left = true;
            step->turn_right = false;
        }
        else if(c == 'R')
        {
            step->turn_left = false;
            step->turn_right = true;
        }
        else if(c == '\n')
        {
            step->turn_left = false;
            step->turn_right = false;
        }

        memset(num_str,0,5);
        num_str_index = 0;
        num_steps++;
    }

#if 0
    for(int i = 0; i < grid_height; ++i)
    {
        for(int j = 0; j < grid_width; ++j)
        {
            uint8_t face = grid_faces[j][i];

            if(face == 0)
                printf(" ");
            else
                printf("%c", '0' + face);
        }
        printf("\n");
    }

    for(int i = 0; i < 6; ++i)
    {
        printf("Grid Corner %d: %d, %d\n",i+1,grid_corners[i].x, grid_corners[i].y);
    }
#endif

    int player_x = 0;
    int player_y = 0;

    // get starting location
    for(int i = 0; i < grid_width; ++i)
    {
        if(grid[i][player_y] == '.')
        {
            player_x = i;
            break;
        }
    }

    // part 1
    // simulate
    FacingDir dir = DIR_RIGHT;

    for(int i = 0; i < num_steps; ++i)
    {
        InstructionStep* step = &steps[i];

        // move spaces
        for(int j = 0; j < step->spaces; ++j)
        {
            int next_x = player_x;
            int next_y = player_y;

            do 
            {
                switch(dir)
                {
                    case DIR_RIGHT: next_x++; break;
                    case DIR_DOWN:  next_y++; break;
                    case DIR_LEFT:  next_x--; break;
                    case DIR_UP:    next_y--; break;
                }

                // basic grid boundaries
                if(next_x < 0)
                {
                    next_x = grid_width - 1;
                }
                else if(next_x >= grid_width)
                {
                    next_x = 0;
                }

                if(next_y < 0)
                {
                    next_y = grid_height -1;
                }
                else if(next_y >= grid_height)
                {
                    next_y = 0;
                }

            } while(grid[next_x][next_y] == ' ');

            char check = grid[next_x][next_y];

            if(check == '#')
                break;

            if(check == '.')
            {
                player_x = next_x;
                player_y = next_y;
            }
        }

        // rotate
        if(step->turn_left)
        {
            switch(dir)
            {
                case DIR_RIGHT: dir = DIR_UP;    break;
                case DIR_DOWN:  dir = DIR_RIGHT; break;
                case DIR_LEFT:  dir = DIR_DOWN;  break;
                case DIR_UP:    dir = DIR_LEFT;  break;
            }
        }
        else if(step->turn_right)
        {
            switch(dir)
            {
                case DIR_RIGHT: dir = DIR_DOWN;  break;
                case DIR_DOWN:  dir = DIR_LEFT;  break;
                case DIR_LEFT:  dir = DIR_UP;    break;
                case DIR_UP:    dir = DIR_RIGHT; break;
            }
        }
    }
    printf("1) Final password: %d\n",1000*(player_y+1) + 4*(player_x+1) + dir);
    
    // reset
    player_x = 0;
    player_y = 0;

    // get starting location
    for(int i = 0; i < grid_width; ++i)
    {
        if(grid[i][player_y] == '.')
        {
            player_x = i;
            break;
        }
    }

    int debug = 0;

    if(debug)
    {
        num_steps = 1000;
        player_x += 49;
        player_y = 52;
    }

    // part 2
    // simulate
    dir = DIR_RIGHT;

    for(int i = 0; i < num_steps; ++i)
    {
        InstructionStep* step = debug ? &steps[0] : &steps[i];

        if(debug)
        {
            day22_print_grid(grid,grid_width,grid_height, player_x, player_y);
            //util_wait_until_key_press();
            char c;
            scanf("%c%*c",&c);
            printf("You pressed %c\n",c);

            step->spaces = 1;

            switch(c)
            {
                case 'w': dir = DIR_UP; break;
                case 'a': dir = DIR_LEFT; break;
                case 's': dir = DIR_DOWN; break;
                case 'd': dir = DIR_RIGHT; break;
                default: step->spaces = 0; break;
            }

            //printf("pressed %d\n",c);
            //printf("Move %d spaces %s\n", step->spaces, map_facing_dir_to_str(dir));
        }

        // move spaces
        for(int j = 0; j < step->spaces; ++j)
        {
            uint8_t curr_face = grid_faces[player_x][player_y];

            int next_x = player_x;
            int next_y = player_y;

            switch(dir)
            {
                case DIR_RIGHT: next_x++; break;
                case DIR_DOWN:  next_y++; break;
                case DIR_LEFT:  next_x--; break;
                case DIR_UP:    next_y--; break;
            }

            bool oob = false;

            oob |= (next_x < 0);
            oob |= (next_x >= grid_width);
            oob |= (next_y < 0);
            oob |= (next_y >= grid_height);
            oob |= (grid[next_x][next_y] == ' ');
            
            FacingDir next_dir = dir;

            if(oob)
            {
                // map to another cube face
                int next_face;

                PathPos* curr_corner = &grid_corners[curr_face-1];

                int rel_x = player_x - curr_corner->x;
                int rel_y = player_y - curr_corner->y;

                if(test)
                {
                    if(curr_face == 1)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 6; next_dir = DIR_LEFT; rel_x = rel_x; rel_y = cube_face_len - rel_y - 1; break;
                            case DIR_DOWN:  next_face = 4; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 3; next_dir = DIR_DOWN; rel_x = rel_y; rel_y = 0; break;
                            case DIR_UP:    next_face = 2; next_dir = DIR_DOWN; rel_x = cube_face_len - rel_x - 1; rel_y = rel_y; break;
                        }
                    }
                    else if(curr_face == 2)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 3; next_dir = DIR_RIGHT; rel_x = 0; rel_y = rel_y; break;
                            case DIR_DOWN:  next_face = 5; next_dir = DIR_UP; rel_x = cube_face_len - rel_x - 1; rel_y = rel_y; break;
                            case DIR_LEFT:  next_face = 6; next_dir = DIR_UP; rel_x = cube_face_len - rel_y -1; rel_y = cube_face_len - 1; break;
                            case DIR_UP:    next_face = 1; next_dir = DIR_DOWN; rel_x = cube_face_len - rel_x - 1; rel_y = rel_y; break;
                        }
                    }
                    else if(curr_face == 3)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 4; next_dir = DIR_RIGHT; rel_x = 0; rel_y = rel_y; break;
                            case DIR_DOWN:  next_face = 5; next_dir = DIR_RIGHT; rel_y = cube_face_len - rel_x - 1; rel_x = 0; break;
                            case DIR_LEFT:  next_face = 2; next_dir = DIR_LEFT; rel_x = cube_face_len - 1; rel_y = rel_y; break;
                            case DIR_UP:    next_face = 1; next_dir = DIR_RIGHT; rel_y = rel_x; rel_x = 0; break;
                        }
                    }
                    else if(curr_face == 4)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 6; next_dir = DIR_DOWN; rel_x = cube_face_len - rel_y - 1; rel_y = 0; break;
                            case DIR_DOWN:  next_face = 5; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 3; next_dir = DIR_LEFT; rel_x = cube_face_len - 1; rel_y = rel_y; break;
                            case DIR_UP:    next_face = 1; next_dir = DIR_UP;   rel_x = rel_x; rel_y = cube_face_len - 1; break;
                        }
                    }
                    else if(curr_face == 5)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 6; next_dir = DIR_RIGHT; rel_x = 0; rel_y = rel_y; break;
                            case DIR_DOWN:  next_face = 2; next_dir = DIR_UP; rel_x = cube_face_len - rel_x - 1; rel_y = cube_face_len-1; break;
                            case DIR_LEFT:  next_face = 3; next_dir = DIR_UP; rel_x = cube_face_len - rel_y - 1; rel_y = cube_face_len-1; break;
                            case DIR_UP:    next_face = 4; next_dir = DIR_UP; rel_x = rel_x; rel_y = cube_face_len - 1; break;
                        }
                    }
                    else if(curr_face == 6)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 1; next_dir = DIR_LEFT; rel_y = cube_face_len - rel_y - 1; rel_x = cube_face_len -1;break;
                            case DIR_DOWN:  next_face = 2; next_dir = DIR_RIGHT; rel_y = cube_face_len - rel_x - 1; rel_x = 0; break;
                            case DIR_LEFT:  next_face = 5; next_dir = DIR_LEFT; rel_x = cube_face_len -1; rel_y = rel_y; break;
                            case DIR_UP:    next_face = 4; next_dir = DIR_LEFT; rel_y = cube_face_len - rel_x - 1; rel_x = cube_face_len - 1; break;
                        }
                    }

                }
                else
                {
                    if(curr_face == 1)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 2; next_dir = DIR_RIGHT; rel_x = 0; rel_y = rel_y; break;
                            case DIR_DOWN:  next_face = 3; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 4; next_dir = DIR_RIGHT; rel_x = 0; rel_y = cube_face_len - rel_y - 1; break;
                            case DIR_UP:    next_face = 6; next_dir = DIR_RIGHT; rel_y = rel_x; rel_x = 0; break;
                        }
                    }
                    else if(curr_face == 2)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 5; next_dir = DIR_LEFT; rel_x = cube_face_len-1; rel_y = cube_face_len - rel_y - 1; break;
                            case DIR_DOWN:  next_face = 3; next_dir = DIR_LEFT; rel_y = rel_x; rel_x = cube_face_len - 1; break;
                            case DIR_LEFT:  next_face = 1; next_dir = DIR_LEFT; rel_x = cube_face_len - 1; rel_y = rel_y; break;
                            case DIR_UP:    next_face = 6; next_dir = DIR_UP; rel_x = rel_x; rel_y = cube_face_len - 1; break;
                        }
                    }
                    else if(curr_face == 3)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 2; next_dir = DIR_UP; rel_x = rel_y; rel_y = cube_face_len -1; break;
                            case DIR_DOWN:  next_face = 5; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 4; next_dir = DIR_DOWN; rel_x = rel_y; rel_y = 0; break;
                            case DIR_UP:    next_face = 1; next_dir = DIR_UP; rel_x = rel_x; rel_y = cube_face_len -1; break;
                        }
                    }
                    else if(curr_face == 4)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 5; next_dir = DIR_RIGHT; rel_x = 0; rel_y = rel_y; break;
                            case DIR_DOWN:  next_face = 6; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 1; next_dir = DIR_RIGHT; rel_x = 0; rel_y = cube_face_len - rel_y -1; break;
                            case DIR_UP:    next_face = 3; next_dir = DIR_RIGHT; rel_y = rel_x; rel_x = 0; break;
                        }
                    }
                    else if(curr_face == 5)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 2; next_dir = DIR_LEFT; rel_x = cube_face_len - 1; rel_y = cube_face_len - rel_y - 1; break;
                            case DIR_DOWN:  next_face = 6; next_dir = DIR_LEFT; rel_y = rel_x; rel_x = cube_face_len-1; break;
                            case DIR_LEFT:  next_face = 4; next_dir = DIR_LEFT; rel_x = cube_face_len - 1; rel_y = rel_y; break;
                            case DIR_UP:    next_face = 3; next_dir = DIR_UP; rel_x = rel_x; rel_y = cube_face_len - 1; break;
                        }
                    }
                    else if(curr_face == 6)
                    {
                        switch(dir)
                        {
                            case DIR_RIGHT: next_face = 5; next_dir = DIR_UP; rel_x = rel_y; rel_y = cube_face_len -1;break;
                            case DIR_DOWN:  next_face = 2; next_dir = DIR_DOWN; rel_x = rel_x; rel_y = 0; break;
                            case DIR_LEFT:  next_face = 1; next_dir = DIR_DOWN; rel_x = rel_y; rel_y = 0; break;
                            case DIR_UP:    next_face = 4; next_dir = DIR_UP; rel_x = rel_x; rel_y = cube_face_len - 1; break;
                        }
                    }
                }

                PathPos* next_corner = &grid_corners[next_face-1];

                next_x = next_corner->x + rel_x;
                next_y = next_corner->y + rel_y;
            }

            char check = grid[next_x][next_y];

            if(check == '#')
                break;

            if(check == '.')
            {
                player_x = next_x;
                player_y = next_y;
                dir = next_dir;
            }
        }

        // rotate
        if(step->turn_left)
        {
            switch(dir)
            {
                case DIR_RIGHT: dir = DIR_UP;    break;
                case DIR_DOWN:  dir = DIR_RIGHT; break;
                case DIR_LEFT:  dir = DIR_DOWN;  break;
                case DIR_UP:    dir = DIR_LEFT;  break;
            }
        }
        else if(step->turn_right)
        {
            switch(dir)
            {
                case DIR_RIGHT: dir = DIR_DOWN;  break;
                case DIR_DOWN:  dir = DIR_LEFT;  break;
                case DIR_LEFT:  dir = DIR_UP;    break;
                case DIR_UP:    dir = DIR_RIGHT; break;
            }
        }

    }
    
    //day22_print_grid(grid,grid_width,grid_height, player_x, player_y);

    //printf("player pos: %d, %d, dir: %d\n", player_x+1, player_y+1, dir);
    printf("2) Final password: %d\n",1000*(player_y+1) + 4*(player_x+1) + dir);

    // 34306 too low
    // 123054 too high

}

typedef struct
{
    int x;
    int y;
    int proposed_x;
    int proposed_y;
    int proposed_dir;
} Elf;

bool is_proposed_square_empty(int x, int y, int index, Elf* elves, int elf_count)
{
    for(int i = 0; i < elf_count; ++i)
    {
        if(i == index)
            continue; // don't consider yourself

        if(elves[i].proposed_dir == -1)
        {
            // elf didn't propose a move, don't move on him
            if(elves[i].x == x && elves[i].y == y)
                return false;
        }
        else
        {
            if(elves[i].proposed_x == x && elves[i].proposed_y == y)
                return false;
        }
    }
    return true;
}

bool is_square_empty(int x, int y, int index, Elf* elves, int elf_count)
{
    for(int i = 0; i < elf_count; ++i)
    {
        if(i == index)
            continue; // don't consider yourself

        if(elves[i].x == x && elves[i].y == y)
            return false;
    }
    return true;
}

void day23_print_grid(Elf* elves, int elf_count,int min_x, int max_x, int min_y, int max_y)
{
    // print grid
    for(int i = min_y-1; i <= max_y+1; ++i)
    {
        for(int j = min_x-1; j <= max_x+1; ++j)
        {
            bool is_elf = false;
            for(int k = 0; k < elf_count; ++k)
            {
                if(j == elves[k].x && i == elves[k].y)
                {
                    is_elf = true;
                    break;
                }
            }
            printf("%c", is_elf ? '#' : '.');
        }
        printf("\n");
    }
}

void day23(bool test)
{
    util_print_day(23);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/23_test.txt" : "inputs/23.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int x = 0;
    int y = 0;

    Elf elves[6000] = {0};
    int elf_count = 0;

    for(;;)
    {
        int c = fgetc(fp);

        if(c == EOF)
            break;

        if(c == '\n')
        {
            y++;
            x = 0;
            continue;
        }

        if(c == '#')
        {
            elves[elf_count].x = x;
            elves[elf_count].y = y;
            elf_count++;
        }

        x++;
    }

    //printf("Num elves: %d\n",elf_count);

    // part 1
    int round = 1;

    int dir_priority[] = {0,1,2,3}; // N,S,W,E

    const int debug = 0;

    int grid_min_x = 0;
    int grid_max_x = 0;
    
    int grid_min_y = 0;
    int grid_max_y = 0;

    int round10_empty_spaces = 0;
    int equalibrium_round = 0;

    for(;;)
    {
        // get grid size
        for(int i = 0; i < elf_count; ++i)
        {
            Elf* elf = &elves[i];

            if(elf->x < grid_min_x) grid_min_x = elf->x;
            if(elf->x > grid_max_x) grid_max_x = elf->x;

            if(elf->y < grid_min_y) grid_min_y = elf->y;
            if(elf->y > grid_max_y) grid_max_y = elf->y;
        }

        if(debug)
        {
            printf("x: [%d, %d], y: [%d, %d]\n", grid_min_x, grid_max_x, grid_min_y, grid_max_y);
            for(int i = 0; i < 4; ++i)
            {
                printf("%d ",dir_priority[i]);
            }
            printf("\n");

            day23_print_grid(elves, elf_count,grid_min_x,grid_max_x, grid_min_y, grid_max_y);
            util_wait_until_key_press();
        }

        if(round == 11)
        {
            // calculate part 1
            for(int j = grid_min_x; j <= grid_max_x; ++j)
            {
                for(int i = grid_min_y; i <= grid_max_y; ++i)
                {
                    bool is_elf = false;
                    for(int k = 0; k < elf_count; ++k)
                    {
                        if(j == elves[k].x && i == elves[k].y)
                        {
                            is_elf = true;
                            break;
                        }
                    }
                    if(is_elf)
                        continue;
                    round10_empty_spaces++;
                }
            }
            printf("1) Empty Spaces: %d\n",round10_empty_spaces);
        }

        if(debug) printf("=== ROUND %d ===\n",round);

        // 1st half

        for(int i = 0; i < elf_count; ++i)
        {
            Elf* elf = &elves[i];

            bool n = is_square_empty(elf->x, elf->y-1, i, elves, elf_count);
            bool s = is_square_empty(elf->x, elf->y+1, i, elves, elf_count);
            bool e = is_square_empty(elf->x+1, elf->y, i, elves, elf_count);
            bool w = is_square_empty(elf->x-1, elf->y, i, elves, elf_count);

            bool ne = is_square_empty(elf->x+1, elf->y-1, i, elves, elf_count);
            bool nw = is_square_empty(elf->x-1, elf->y-1, i, elves, elf_count);
            bool se = is_square_empty(elf->x+1, elf->y+1, i, elves, elf_count);
            bool sw = is_square_empty(elf->x-1, elf->y+1, i, elves, elf_count);

            elf->proposed_dir = -1; // reset

            if(n && s && e && w && ne && nw && se && sw)
            {
                // no other elves adjacent
                // do nothing
                continue;
            }
            else
            {
                for(int d = 0; d < 4; ++d)
                {
                    int dir = dir_priority[d];
                    bool move = false;
                    
                    switch(dir)
                    {
                        case 0: move = (n && ne && nw); break;
                        case 1: move = (s && se && sw); break;
                        case 2: move = (w && nw && sw); break;
                        case 3: move = (e && ne && se); break;
                    }

                    if(move)
                    {
                        elf->proposed_dir = dir;
                        break;
                    }
                }

                if(elf->proposed_dir > -1)
                {
                    elf->proposed_x = elf->x;
                    elf->proposed_y = elf->y;

                    switch(elf->proposed_dir)
                    {
                        case 0: elf->proposed_y--; break; // north
                        case 1: elf->proposed_y++; break; // south
                        case 2: elf->proposed_x--; break; // west
                        case 3: elf->proposed_x++; break; // east
                    }
                }
                if(debug) printf("elf %d [%d, %d] proposed move %d\n",i, elf->x, elf->y, elf->proposed_dir);
            }
        }

        // 2nd half
        // try to move everyone
        bool elf_moved = false;

        for(int i = 0; i < elf_count; ++i)
        {
            Elf* elf = &elves[i];
            
            if(elf->proposed_dir == -1)
                continue;

            bool empty = is_proposed_square_empty(elf->proposed_x, elf->proposed_y, i, elves, elf_count);

            if(empty)
            {
                // move elf
                elf_moved = true;
                elf->x = elf->proposed_x;
                elf->y = elf->proposed_y;
            }
        }

        if(!elf_moved)
        {
            equalibrium_round = round;
            //day23_print_grid(elves, elf_count,grid_min_x,grid_max_x, grid_min_y, grid_max_y);
            break;
        }
        
        // cycle dir priority
        int temp_dir = dir_priority[0];

        for(int i = 1; i < 4; ++i)
        {
            dir_priority[i-1] = dir_priority[i];
        }
        dir_priority[3] = temp_dir;

        round++;
    }

    printf("2) Equalibrium Round: %d\n", equalibrium_round);

}

typedef struct
{
    int x;
    int y;
    char dir;
} Blizzard;

void day_24_print_grid(Blizzard* blizzards, int blizzard_count, PathPos* walls, int wall_count, int grid_width, int grid_height)
{
    for(int i = 0; i < grid_height; ++i)
    {
        for(int j = 0; j < grid_width; ++j)
        {
            bool is_wall = false;

            for(int w = 0; w < wall_count; ++w)
            {
                if(walls[w].x == j && walls[w].y == i)
                    is_wall = true;
            }

            if(is_wall)
            {
                printf("#");
                continue;
            }

            char blizzard_dir = ' ';
            int blizzard_n = 0;
            for(int b = 0; b < blizzard_count; ++b)
            {
                if(blizzards[b].x == j && blizzards[b].y == i)
                {
                    blizzard_dir = blizzards[b].dir;
                    blizzard_n++;
                }
            }

            if(blizzard_n > 0)
            {
                printf("%c",blizzard_n == 1 ? blizzard_dir : '0' + blizzard_n);
                continue;
            }

            printf(".");
        }
        printf("\n");
    }
}

void day24(bool test)
{
    util_print_day(24);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/24_test.txt" : "inputs/24.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    int x = 0;
    int y = 0;

    Blizzard blizzards[1000] = {0};
    int blizzard_count = 0;

    int grid_width = 0;
    int grid_height = 0;

    PathPos walls[1000] = {0};
    int wall_count;
    PathPos start = {0,0};

    // parse out dem blizzards
    for(;;)
    {
        int c = fgetc(fp);
        if(c == EOF)
            break;

        if(c == '\n')
        {
            y++;
            if(x > grid_width)
                grid_width = x;
            x = 0;
            continue;
        }

        if(c == '#')
        {
            walls[wall_count].x = x;
            walls[wall_count].y = y;
            wall_count++;
            x++;
            continue;
        }

        if(c == '.')
        {
            // floor
            if(y == 0)
            {
                start.x = x;
                start.y = y;
            }
            x++;
            continue;
        }

        Blizzard* b = &blizzards[blizzard_count++];

        b->x = x;
        b->y = y;
        b->dir = c;
        x++;
    }

    grid_height = y;

    printf("Grid Size: %d, %d\n",grid_width, grid_height);

    day_24_print_grid(blizzards, blizzard_count, walls, wall_count, grid_width, grid_height);
    util_wait_until_key_press();

    // simulate blizzards
    for(;;)
    {

        for(int i = 0; i < blizzard_count; ++i)
        {
            Blizzard* b = &blizzards[i];
            switch(b->dir)
            {
                case '<': b->x--; break;
                case '>': b->x++; break;
                case '^': b->y--; break;
                case 'v': b->y++; break;
            }

            if(b->x <= 0) b->x = grid_width - 2;
            else if(b->x >= grid_width -1) b->x = 1;
            if(b->y <= 0) b->y = grid_height - 2;
            else if(b->y >= grid_height -1) b->y = 1;
        }

        day_24_print_grid(blizzards, blizzard_count, walls, wall_count, grid_width, grid_height);
        util_wait_until_key_press();
    }
}

long UNSNAFU(char* s, int len)
{
    long number = 0;
    int places = 0;
    for(int i = len-1; i >= 0; i--)
    {
        char c = s[i];
        int val = 0;
        switch(c)
        {
            case '=': val = -2; break;
            case '-': val = -1; break;
            case '0': val = 0; break;
            case '1': val = 1; break;
            case '2': val = 2; break;
        }
        number += val*(pow(5,places));
        places++;
    }

    return number;
}

char* SNAFU(long n)
{
    // get largest needed exponent
    int exp = 0;
    for(;;)
    {
        long test = pow(5,exp);
        for(int p = 0; p < exp; ++p)
        {
            test += 2*pow(5,p);
        }

        if(test >= n)
        {
            break;
        }

        exp++;
    }


    int places = exp+1;

    char* s = calloc(places,sizeof(char));
    int s_i = 0;

    for(int i = exp; i >= 0; --i)
    {
        int n_dir = (n < 0 ? -1 : 1);

        // find value at position i
        for(int j = -2; j <= 2; ++j)
        {
            long test = j*pow(5,i);
            for(int e = 0; e < i; ++e)
            {
                test += (-2*pow(5,e));
            }

            if(test-n >= 0 || j == 2)
            {
                int k;
                if(test-n == 0)
                {
                    k = j;
                }
                else if(test-n >= 0)
                {
                    k = j-1;
                }
                else
                {
                    // j==2
                    k = j;
                }

                if(s_i == 0 && k == 0)
                {
                    // disregard leading zeros
                    break;
                }
                
                n -= (k*pow(5,i));

                switch(k)
                {
                    case -2: s[s_i] = '='; break;
                    case -1: s[s_i] = '-'; break;
                    case  0: s[s_i] = '0'; break;
                    case  1: s[s_i] = '1'; break;
                    case  2: s[s_i] = '2'; break;
                }
                s_i++;
                break;
            }
        }
    }

    return s;
}

void day25(bool test)
{
    util_print_day(25);

    if(test) printf("**TEST FILE**\n\n");

    char* input_file = test ? "inputs/25_test.txt" : "inputs/25.txt";
    FILE* fp = fopen(input_file, "r");

    if(!fp)
    {
        printf("Failed to open input file: %s\n",input_file);
        return;
    }

    char s[100+1] = {0};

    long sum = 0;
    for(;;)
    {
        if(fgets(s,100,fp) == NULL)
            break;

        long n = UNSNAFU(s,strlen(s)-1); // -1 because of newline
        sum += n;
    }

    char* snafu_number = SNAFU(sum);
    printf("1) SNAFU: %s\n",snafu_number);
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
    day8();
    day9();
    day10();
    day11();
    day12(1);
    day13();
    day14(1);
    day15(1);
    //day16(); // in progress
    day17();
    day18(1);
    day19(1); // in progress
    //day20(1);
    //day21(0);
    //day22(0);
    //day23(1);
    //day24(1); // in progress
    //day25(0);

    printf("\n======================================================\n");
    return 0;
}
