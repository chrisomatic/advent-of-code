#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

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

bool day9_is_position_unique(Knot_t pos, Knot_t* visited_places, int num_visited_places)
{
    for(int i = 0; i < num_visited_places; ++i)
    {
        if(visited_places[i].x == pos.x && visited_places[i].y == pos.y)
        {
            return false;
        }
    }
    return true;
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

    Knot_t head = {0};
    Knot_t tail = {0};

    Knot_t visited_places[1024] = {0};
    int num_visited_places = 0;

    for(;;)
    {
        char dir;
        int num;

        int num_matches = fscanf(fp,"%c %d\n",&dir,&num);
        if(num_matches != 2)
            break;

        // move head
        switch(dir)
        {
            case 'L': head.x -= num; break;
            case 'R': head.x += num; break;
            case 'U': head.y -= num; break;
            case 'D': head.y += num; break;

            default:
              printf("Not a valid direction %c\n",dir);
        }

        //printf("Head %d %d\n",head.x,head.y);

        // move tail
        int diff_x = head.x - tail.x;
        int diff_y = head.y - tail.y;

        if(diff_y == 0)
        {
            if(diff_x == -2)
                tail.x -= 1;
            else if(diff_x == 2)
                tail.x += 1;
        }
        else if(diff_x == 0)
        {
            if(diff_y == -2)
                tail.y -= 1;
            else if(diff_y == 2)
                tail.y += 1;
        }
        else
        {
            float dist = sqrt(diff_x*diff_x + diff_y*diff_y);

            if(dist >= 2.0)
            {
                // not touching, move tail diagonally
                if(diff_x < 0 && diff_y < 0)
                {
                    tail.x -= 1;
                    tail.y -= 1;
                }
                else if(diff_x < 0 && diff_y > 0)
                {
                    tail.x -= 1;
                    tail.y += 1;
                }
                else if(diff_x > 0 && diff_y < 0)
                {
                    tail.x += 1;
                    tail.y -= 1;
                }
                else if(diff_x > 0 && diff_y > 0)
                {
                    tail.x += 1;
                    tail.y += 1;
                }
            }
        }



    }
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
    printf("\n======================================================\n");
    return 0;
}
