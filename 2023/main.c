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

    fclose(fp);

}

typedef struct
{
    int red;
    int green;
    int blue;
} GameRound;

typedef struct
{
    int id;
    GameRound rounds[20];
    int num_rounds;
} Game;

// Part 1
// 224 (too low)

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

    // parse Games
    Game games[200] = {0};
    int num_games = 0;

    char buf[33] = {0};
    int bufi = 0;
    int count = 0;

    Game* game = &games[num_games++];

    int c;
    for(;;)
    {
        c = fgetc(fp);

        if(feof(fp))
            break;

        if(c == '\n')
        {
            if(game->id > 0)
            {
                // get color string
                if(STR_EQUAL(buf,"red"))
                    game->rounds[game->num_rounds].red += count;
                else if(STR_EQUAL(buf,"green"))
                    game->rounds[game->num_rounds].green += count;
                else if(STR_EQUAL(buf,"blue"))
                    game->rounds[game->num_rounds].blue += count;

                count = 0;
                game->num_rounds++;
            }

            game = &games[num_games++];

            memset(buf,0,32);
            bufi = 0;
            continue;
        }

        if(c == ' ')
        {
            int n = atoi(buf);
            if(n > 0)
                count = n;

            memset(buf,0, 32);
            bufi = 0;
            continue;
        }

        if(c == ',')
        {
            if(game->id > 0)
            {
                // get color string
                if(STR_EQUAL(buf,"red"))
                    game->rounds[game->num_rounds].red += count;
                else if(STR_EQUAL(buf,"green"))
                    game->rounds[game->num_rounds].green += count;
                else if(STR_EQUAL(buf,"blue"))
                    game->rounds[game->num_rounds].blue += count;

                count = 0;
            }

            memset(buf,0, 32);
            bufi = 0;
            continue;
        }

        if(c == ':')
        {
            game->id = atoi(buf);
            memset(buf,0,32);
            bufi = 0;
            continue;
        }

        if(c == ';')
        {
            if(game->id > 0)
            {
                // get color string
                if(STR_EQUAL(buf,"red"))
                    game->rounds[game->num_rounds].red += count;
                else if(STR_EQUAL(buf,"green"))
                    game->rounds[game->num_rounds].green += count;
                else if(STR_EQUAL(buf,"blue"))
                    game->rounds[game->num_rounds].blue += count;

                count = 0;
            }
            game->num_rounds++;
            memset(buf,0,32);
            bufi = 0;
            continue;
        }
        
        buf[bufi++] = c;
    }

    num_games--;

    int sum_ids = 0;

    for(int i = 0; i < num_games; ++i)
    {
        Game* game = &games[i];

        bool game_valid = true;

        for(int j = 0; j < game->num_rounds; ++j)
        {
            GameRound* round = &game->rounds[j];

            if(round->red > 12 || round->green > 13 || round->blue > 14)
            {
                game_valid = false;
                break;
            }
        }

        if(game_valid)
        {
            sum_ids += game->id;
        }
    }

    printf("1) Sum of Ids: %d\n", sum_ids);

    int power_sum = 0;

    for(int i = 0; i < num_games; ++i)
    {
        Game* game = &games[i];

        int max_red = 0;
        int max_green = 0;
        int max_blue = 0;

        for(int j = 0; j < game->num_rounds; ++j)
        {
            GameRound* round = &game->rounds[j];

            if(round->red > max_red)     max_red   = round->red;
            if(round->green > max_green) max_green = round->green;
            if(round->blue > max_blue)   max_blue  = round->blue;
        }

        int power = max_red * max_green * max_blue;
        power_sum += power;
    }

    printf("2) Sum of powers: %d\n", power_sum);

    fclose(fp);
}

typedef struct
{
    int part_number;
    int x;
    int y;
    int length;
    bool valid;
    bool is_gear;
    int gear_x;
    int gear_y;
} EnginePart;
    

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

    EnginePart parts[10000] = {0};
    int part_count = 0;

    char schematic[140][140] = {0};
    int x = 0;
    int y = 0;

    int c;
    char buf[33] = {0};
    int bufi = 0;

    EnginePart* part = &parts[part_count];

    for(;;)
    {
        c = fgetc(fp);

        if(feof(fp))
            break;

        if(c >= '0' && c <= '9')
        {
            buf[bufi++] = c;
        }
        else
        {
            if(bufi > 0)
            {
                // store part number 
                part->part_number = atoi(buf);
                part->x = x-bufi;
                part->y = y;
                part->length = bufi;

                memset(buf,0,32);
                bufi = 0;

                part_count++;
                part = &parts[part_count];
            }

            if(c == '\n')
            {
                y++;
                x = 0;
                continue;
            }
        }

        schematic[x][y] = c;
        x++;
    }

    fclose(fp);

    //tl  top  tr
    //ll  NNN  rr
    //bl  bot  br
    for(int i = 0; i < part_count; ++i)
    {
        EnginePart* part = &parts[i];

        char ll = schematic[part->x-1][part->y];
        char tl = schematic[part->x-1][part->y-1];
        char bl = schematic[part->x-1][part->y+1];

        char rr = schematic[part->x+part->length][part->y];
        char tr = schematic[part->x+part->length][part->y-1];
        char br = schematic[part->x+part->length][part->y+1];

        char top[10] = {0};
        char bot[10] = {0};

        for(int i = 0; i < part->length; ++i)
        {
            top[i] = schematic[part->x+i][part->y-1];
            bot[i] = schematic[part->x+i][part->y+1];
        }

        char checks[32] = {0};
        int checks_x[32] = {0};
        int checks_y[32] = {0};
        int check_count = 0;

        if(part->x > 0)                  { checks[check_count] = ll; checks_x[check_count] = part->x-1; checks_y[check_count] = part->y;   check_count++;}
        if(part->x > 0 && part->y > 0)   { checks[check_count] = tl; checks_x[check_count] = part->x-1; checks_y[check_count] = part->y-1; check_count++;}
        if(part->x > 0 && part->y < 139) { checks[check_count] = bl; checks_x[check_count] = part->x-1; checks_y[check_count] = part->y+1; check_count++;}

        if(part->x+part->length < 140)                  { checks[check_count] = rr; checks_x[check_count] = part->x+part->length; checks_y[check_count] = part->y;   check_count++;}
        if(part->x+part->length < 140 && part->y > 0)   { checks[check_count] = tr; checks_x[check_count] = part->x+part->length; checks_y[check_count] = part->y-1; check_count++;}
        if(part->x+part->length < 140 && part->y < 139) { checks[check_count] = br; checks_x[check_count] = part->x+part->length; checks_y[check_count] = part->y+1; check_count++;}

        for(int i = 0; i < part->length; ++i)
        {
            if(part->y > 0)   { checks[check_count] = top[i]; checks_x[check_count] = part->x+i; checks_y[check_count] = part->y-1; check_count++;}
            if(part->y < 139) { checks[check_count] = bot[i]; checks_x[check_count] = part->x+i; checks_y[check_count] = part->y+1; check_count++;}
        }

        part->valid = false;

        for(int i = 0; i < check_count; ++i)
        {
            char c = checks[i];
            bool is_valid = (c >= 33 && c <= 64 && (c <= '0' || c >= '9') && c != '.');

            if(is_valid)
            {
                part->valid = true;

                if(c == '*')
                {
                    part->is_gear = true;
                    part->gear_x = checks_x[i];
                    part->gear_y = checks_y[i];
                    break;
                }
            }
        }

        //printf("Part %d: number: %d, x: %d, y: %d, len: %d, valid: %d\n",i, part->part_number, part->x, part->y, part->length, part->valid);
    }

    int sum = 0;

    for(int i = 0; i < part_count; ++i)
    {
        if(parts[i].valid)
        {
            sum += parts[i].part_number;
        }
    }

    // 538136: too high
    // 533537: too high
    printf("1) Sum of part numbers: %d\n", sum);

    long sum2 = 0;

    for(int i = 0; i < part_count; ++i)
    {
        EnginePart* p1 = &parts[i];

        if(p1->is_gear)
        {
            // find its partner
            EnginePart* p2 = NULL;
            for(int j = i+1; j < part_count; ++j) // starting at i+1 should prevent checking with self and checking twice
            {
                EnginePart* check = &parts[j];
                
                if(!check->is_gear)
                    continue;

                if(check->gear_x == p1->gear_x && check->gear_y == p1->gear_y)
                {
                    // found buddy
                    int gear_ratio = p1->part_number * check->part_number;
                    sum2 += gear_ratio;
                    break;
                }
            }
            //printf("Part %d: number: %d, x: %d, y: %d, len: %d, valid: %d\n",i, part->part_number, part->x, part->y, part->length, part->valid);
        }
    }

    printf("2) Sum of gear ratios: %d\n", sum2);

}

typedef struct
{
    int card_num;
    int w[10];
    int n[25];
    int matches;
    int instances;
} Scratchcard;

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

    Scratchcard cards[256] = {0};
    int num_cards = 0;

    for(;;)
    {
        if(feof(fp))
            break;

        Scratchcard* c = &cards[num_cards++];

        fscanf(fp,"Card %d: %d %d %d %d %d %d %d %d %d %d | %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                &c->card_num,
                &c->w[0],&c->w[1],&c->w[2],&c->w[3],&c->w[4],&c->w[5],&c->w[6],&c->w[7],&c->w[8],&c->w[9],
                &c->n[0],&c->n[1],&c->n[2],&c->n[3],&c->n[4],&c->n[5],&c->n[6],&c->n[7],&c->n[8],&c->n[9],
                &c->n[10],&c->n[11],&c->n[12],&c->n[13],&c->n[14],&c->n[15],&c->n[16],&c->n[17],&c->n[18],&c->n[19],
                &c->n[20],&c->n[21],&c->n[22],&c->n[23],&c->n[24]);

        /*
        fscanf(fp,"Card %d: %d %d %d %d %d | %d %d %d %d %d %d %d %d\n",
                &c->card_num,
                &c->w[0],&c->w[1],&c->w[2],&c->w[3],&c->w[4],
                &c->n[0],&c->n[1],&c->n[2],&c->n[3],&c->n[4],&c->n[5],&c->n[6],&c->n[7]
          );
          */
    }

    for(int i = 0; i < num_cards; ++i)
    {
        Scratchcard* c = &cards[i];

        c->instances = 1;
        
        for(int j = 0; j < 10; ++j)
        {
            int w = c->w[j];
            for(int k = 0; k < 25; ++k)
            {
                int n = c->n[k];
                if(w == n)
                    c->matches++;
            }
        }

        //printf("Card %d: %ld\n", c->card_num, points);
    }

    long sum = 0;

    for(int i = 0; i < num_cards; ++i)
        sum += pow(2.0, cards[i].matches-1);

    printf("1) Sum of points: %ld\n",sum);

    // calculate instances
    for(int i = 0; i < num_cards; ++i)
    {
        //printf("evaluating card %d\n",cards[i].card_num);

        Scratchcard* c = &cards[i];
        for(int j = 0; j < c->instances; ++j)
        {
            //printf("  instance %d (matches: %d)\n", j+1, c->matches);
            for(int k = 1; k <= c->matches; ++k)
            {
                if(i+k < num_cards)
                {
                    cards[i+k].instances++;
                    //printf("   Card %d instances: %d", i+k, cards[i+k].instances);
                }
            }
        }

        //util_wait_until_key_press();
    }

    long total_cards = 0;

    for(int i = 0; i < num_cards; ++i)
    {
        total_cards += (cards[i].instances);
    }

    printf("2) Total cards: %ld\n", total_cards);

    fclose(fp);
}

int main(int argc, char* args[])
{
    printf("\n===================== AOC 2023 =======================\n");

    day1();
    day2();
    day3();
    day4();

    printf("\n======================================================\n");
    return 0;
}
