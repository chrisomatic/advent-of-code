#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
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

void day12()
{
    util_print_day(12);

    char* input_file = "inputs/12.txt";
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

    printf("Grid (%d x %d)\n", grid_width, grid_height);
    printf("S [%d,%d] -> E [%d,%d]\n",start.x,start.y,end.x,end.y);

#if 1
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
    int x,y;
} PathPoint;

typedef struct
{
    PathPoint points[32];
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
            PathPoint* p1 = &paths[i].points[j];
            PathPoint* p2 = j == paths[i].point_count - 1 ? p1 : &paths[i].points[j+1];

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
                    PathPoint check = {
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

bool is_point_sand(int x, int y, PathPoint* sand, int sand_count)
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

bool is_point_solid(int x, int y, Path* paths, int path_count, PathPoint* sand, int sand_count, int floor)
{
    if(is_point_rock(x,y,paths,path_count,floor) || is_point_sand(x,y,sand,sand_count))
    {
        return true;
    }

    return false;
}

int day14_simulate_sand(Path* paths, int path_count, int floor)
{
    PathPoint sand[100000] = {0};
    int sand_count = 0;
    
    PathPoint s = {500,0};

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
        memcpy(&sand[sand_count++], &s, sizeof(PathPoint));

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

void day14()
{
    util_print_day(14);

    char* input_file = "inputs/14.txt";
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
            PathPoint p = {0};
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
            PathPoint* p = &all_paths[i].points[j];

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
    //day12(); // comment-out due to substantial runtime
    day13();
    //day14(); // comment-out due to substantial runtime
    day15();

    printf("\n======================================================\n");
    return 0;
}
