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

typedef struct
{
    long n1;
    long n2;
    long n3;
} SeedMapItem;


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

    long seed_nums[20] = {0};

    fscanf(fp,"seeds: %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
            &seed_nums[0], &seed_nums[1], &seed_nums[2], &seed_nums[3], &seed_nums[4],
            &seed_nums[5], &seed_nums[6], &seed_nums[7], &seed_nums[8], &seed_nums[9],
            &seed_nums[10], &seed_nums[11], &seed_nums[12], &seed_nums[13], &seed_nums[14],
            &seed_nums[15], &seed_nums[16], &seed_nums[17], &seed_nums[18], &seed_nums[19]
    );

    char line[101] = {0};

    SeedMapItem map_seed_to_soil[100]      = {0};
    SeedMapItem map_soil_to_fert[100]      = {0};
    SeedMapItem map_fert_to_water[100]     = {0};
    SeedMapItem map_water_to_light[100]    = {0};
    SeedMapItem map_light_to_temp[100]     = {0};
    SeedMapItem map_temp_to_humid[100]     = {0};
    SeedMapItem map_humid_to_location[100] = {0};

    int count_seed_to_soil      = 0;
    int count_soil_to_fert      = 0;
    int count_fert_to_water     = 0;
    int count_water_to_light    = 0;
    int count_light_to_temp     = 0;
    int count_temp_to_humid     = 0;
    int count_humid_to_location = 0;

    SeedMapItem* map = NULL;
    int* count = NULL;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        int line_len = strlen(line);
        line[line_len-1] = 0; // remove newline

        if(STR_EQUAL(line, "seed-to-soil map:"))
        {
            map = map_seed_to_soil;
            count = &count_seed_to_soil;
        }
        else if(STR_EQUAL(line, "soil-to-fertilizer map:"))
        {
            map = map_soil_to_fert;
            count = &count_soil_to_fert;
        }
        else if(STR_EQUAL(line, "fertilizer-to-water map:"))
        {
            map = map_fert_to_water;
            count = &count_fert_to_water;
        }
        else if(STR_EQUAL(line, "water-to-light map:"))
        {
            map = map_water_to_light;
            count = &count_water_to_light;
        }
        else if(STR_EQUAL(line, "light-to-temperature map:"))
        {
            map = map_light_to_temp;
            count = &count_light_to_temp;
        }
        else if(STR_EQUAL(line, "temperature-to-humidity map:"))
        {
            map = map_temp_to_humid;
            count = &count_temp_to_humid;
        }
        else if(STR_EQUAL(line, "humidity-to-location map:"))
        {
            map = map_humid_to_location;
            count = &count_humid_to_location;
        }
        else
        {
            SeedMapItem* item = &map[*count];
            int matches = sscanf(line, "%ld %ld %ld", &item->n1,&item->n2,&item->n3);
            if(matches > 0)
                (*count)++;
        }
    }

    fclose(fp);

    SeedMapItem* maps[] = {
        map_seed_to_soil,
        map_soil_to_fert,
        map_fert_to_water,
        map_water_to_light,
        map_light_to_temp,
        map_temp_to_humid,
        map_humid_to_location
    };

    int counts[7] = {
        count_seed_to_soil,
        count_soil_to_fert,
        count_fert_to_water,
        count_water_to_light,
        count_light_to_temp,
        count_temp_to_humid,
        count_humid_to_location
    };

    long min_location = LONG_MAX;

    for(int l = 0; l < 20; ++l)
    {
        long num = seed_nums[l];

        for(int i = 0; i < 7; ++i)
        {
            for(int j = 0; j < counts[i]; ++j)
            {
                long n1 = maps[i][j].n1;
                long n2 = maps[i][j].n2;
                long n3 = maps[i][j].n3;

                if(num >= n2 && num < n2 + n3)
                {
                    // found range
                    num = n1 + (num - n2);
                    break;
                }
            }
        }

        if(num < min_location)
            min_location = num;
    }

    printf("1) Lowest Location Number: %ld\n", min_location);

    min_location = LONG_MAX;

    for(int l = 0; l < 10; ++l)
    {
        // dumb brute-force solution
        long fmax = seed_nums[2*l+1];
        for(int f = 0; f < fmax; ++f)
        {
            //if(f % 1000000 == 0)
            //    printf("%ld/%ld\n",f,fmax);

            long num = seed_nums[2*l] + f;

            for(int i = 0; i < 7; ++i)
            {
                for(int j = 0; j < counts[i]; ++j)
                {
                    long n1 = maps[i][j].n1;
                    long n2 = maps[i][j].n2;
                    long n3 = maps[i][j].n3;

                    if(num >= n2 && num < n2 + n3)
                    {
                        // found range
                        num = n1 + (num - n2);
                        break;
                    }
                }
            }

            if(num < min_location)
                min_location = num;
        }
        printf("min after set %d: %ld\n",l, min_location);
    }

    printf("2) Min Location in Ranges: %ld\n", min_location);
}


typedef struct
{
    int time;
    int dist;
} Race;

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

    const int num_races = 4;
    Race races[4] = {0};

    fscanf(fp, "Time: %d %d %d %d\n",
            &races[0].time,
            &races[1].time,
            &races[2].time,
            &races[3].time);

    fscanf(fp, "Distance: %d %d %d %d\n",
            &races[0].dist,
            &races[1].dist,
            &races[2].dist,
            &races[3].dist);

    fclose(fp);

    int num_ways_to_win[4] = {0};

    int margin_of_error = 1;

    for(int i = 0; i < num_races; ++i)
    {
        int time_limit = races[i].time;

        for(int t = 0; t < time_limit; ++t)
        {
            int dist = (time_limit - t)*t;

            if(dist > races[i].dist)
            {
                num_ways_to_win[i]++;
            }
        }

        margin_of_error *= num_ways_to_win[i];
    }

    printf("1) Margin of Error: %d\n", margin_of_error);

    long time = 56717999;
    long long dist = 334113513502430;

    long ways_to_win = 0;
    for(long t = 0; t < time; ++t)
    {
        long long d = (time - t)*t;
        if(d > dist)
            ways_to_win++;
        
    }

    printf("2) Margin of Error: %ld\n", ways_to_win);

}

typedef enum
{
    HAND_TYPE_NONE = 0,
    HAND_TYPE_HIGH_CARD,
    HAND_TYPE_ONE_PAIR,
    HAND_TYPE_TWO_PAIR,
    HAND_TYPE_THREE_OF_A_KIND,
    HAND_TYPE_FULL_HOUSE,
    HAND_TYPE_FOUR_OF_A_KIND,
    HAND_TYPE_FIVE_OF_A_KIND,
} HandType;

typedef struct
{
    char cards[5];
    char cards_wild[5]; // with wildcards
    int bid;
    HandType type;
    char best_card;
} CardHand;

void day7_determine_hand_type(CardHand* hand, bool jokers)
{
    int max_matches = 0;
    char max_char = 0;
    char second_max_char = 0;
    int matches[5] = {0};
    
    char* cards = hand->cards;

    if(jokers)
    {
        // create best hand
        for(int i = 0; i < 5; ++i)
        {
            char c = hand->cards[i];
            if(c == 'J' && hand->best_card)
            {
                hand->cards_wild[i] = hand->best_card;
            }
            else
            {
                hand->cards_wild[i] = hand->cards[i];
            }
        }

        cards = hand->cards_wild;
    }

    for(int i = 0; i < 5; ++i)
    {
        int num_matches = 0;
        int c1 = cards[i];

        for(int j = 0; j < 5; ++j)
        {
            int c2 = cards[j];
            if(c1 == c2)
                num_matches++;
        }
        matches[i] = num_matches;

        if(num_matches >= max_matches)
        {
            bool check = (num_matches == max_matches && c1 == 'J');
            if(!check)
                max_char = c1;
            max_matches = num_matches;
        }
    }

    for(int i = 0; i < 5; ++i)
    {
        if(cards[i] != max_char)
        {
            second_max_char = cards[i];
            break;
        }
    }

    if(!jokers)
    {
        hand->best_card = max_char == 'J' ? second_max_char : max_char;
    }

    if(max_matches == 5)
    {
        hand->type = HAND_TYPE_FIVE_OF_A_KIND;
        return;
    }

    if(max_matches == 4)
    {
        hand->type = HAND_TYPE_FOUR_OF_A_KIND;
        return;
    }

    if(max_matches == 3)
    {
        // check for full house
        for(int i = 0; i < 5; ++i)
        {
            if(matches[i] == 2)
            {
                hand->type = HAND_TYPE_FULL_HOUSE;
                return;
            }
        }
        hand->type = HAND_TYPE_THREE_OF_A_KIND;
        return;
    }

    if(max_matches == 2)
    {
        // check pairs
        int num_pairs = 0;
        for(int i = 0; i < 5; ++i)
        {
            if(matches[i] == 2)
                num_pairs++;
        }

        num_pairs /= 2;

        if(num_pairs == 2)
        {
            hand->type = HAND_TYPE_TWO_PAIR;
            return;
        }

        hand->type = HAND_TYPE_ONE_PAIR;
        return;
    }

    hand->type = HAND_TYPE_HIGH_CARD;

}

const char* hand_type_to_str(HandType type)
{
    switch(type)
    {
        case HAND_TYPE_NONE: return "None";
        case HAND_TYPE_HIGH_CARD: return "High Card";
        case HAND_TYPE_ONE_PAIR: return "One Pair";
        case HAND_TYPE_TWO_PAIR: return "Two Pair";
        case HAND_TYPE_THREE_OF_A_KIND: return "Three of a Kind";
        case HAND_TYPE_FULL_HOUSE: return "Full House";
        case HAND_TYPE_FOUR_OF_A_KIND: return "Four of a Kind";
        case HAND_TYPE_FIVE_OF_A_KIND: return "Five of a Kind";
    }
    return "Unknown";
}

char card_labels[] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
char card_labels_jokers[] = {'J','2','3','4','5','6','7','8','9','T','Q','K','A'};

bool is_hand_greater(CardHand* hand, CardHand* key, bool jokers)
{
    if(hand->type > key->type)
        return true;

    if(hand->type < key->type)
        return false;

    // types are equal, evaluate cards

    for(int i = 0; i < 5; ++i)
    {
        if(hand->cards[i] == key->cards[i])
            continue;

        int hand_label_position = 0;
        int key_label_position = 0;

        char* labels = jokers ? card_labels_jokers : card_labels;

        for(int j = 0; j < 13; ++j)
        {
            if(hand->cards[i] == labels[j])
                hand_label_position = j;

            if(key->cards[i] == labels[j])
                key_label_position = j;
        }

        if(hand_label_position > key_label_position)
            return true;

        return false;
    }

    return false;
}

void sort_cards(CardHand* hands, int count, bool jokers)
{
    // insertion sort
    int i, j;
    CardHand key;

    for (i = 1; i < count; ++i) 
    {
        memcpy(&key, &hands[i], sizeof(CardHand));
        j = i - 1;

        while (j >= 0 && is_hand_greater(&hands[j], &key, jokers))
        {
            memcpy(&hands[j+1], &hands[j], sizeof(CardHand));
            j = j - 1;
        }
        memcpy(&hands[j+1], &key, sizeof(CardHand));
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

    CardHand hands[1000] = {0};
    int hand_count = 0;

    for(;;)
    {
        if(feof(fp))
            break;

        CardHand* hand = &hands[hand_count];
        int matches = fscanf(fp, "%s %d\n", hand->cards,&hand->bid);
        if(matches)
            hand_count++;
    }

    fclose(fp);

    for(int i = 0; i < hand_count; ++i)
    {
        CardHand* hand = &hands[i];
        day7_determine_hand_type(hand, false);
    }

    sort_cards(hands, hand_count, false);

    long total_winnings = 0;

    for(int i = 0; i < hand_count; ++i)
    {
        CardHand* hand = &hands[i];
        total_winnings += (i+1)*hand->bid;
        //printf("rank %d: %s %d, %s, best card: %c\n", i+1, hand->cards, hand->bid, hand_type_to_str(hand->type), hand->best_card);
    }

    // 250835773: Too High
    printf("1) Total Winnings: %ld\n",total_winnings);

    for(int i = 0; i < hand_count; ++i)
    {
        CardHand* hand = &hands[i];
        day7_determine_hand_type(hand, true);
    }

    sort_cards(hands, hand_count, true);

    total_winnings = 0;
    for(int i = 0; i < hand_count; ++i)
    {
        CardHand* hand = &hands[i];
#if 0
        printf("rank %d: %c%c%c%c%c (%c%c%c%c%c) %d, %s, best card: %c\n", i+1,
            hand->cards[0], hand->cards[1], hand->cards[2], hand->cards[3], hand->cards[4], 
            hand->cards_wild[0], hand->cards_wild[1], hand->cards_wild[2], hand->cards_wild[3], hand->cards_wild[4], 
            hand->bid, hand_type_to_str(hand->type), hand->best_card);
#endif
        total_winnings += (i+1)*hand->bid;
    }

    // 251932676: Too High
    // 251145213: Too Low
    // 251101560: Wrong

    printf("2) Total Winnings: %ld\n",total_winnings);

}

typedef struct NetworkNode NetworkNode;

struct NetworkNode
{
    char name[4];
    char left[4];
    char right[4];

    NetworkNode* left_ptr;
    NetworkNode* right_ptr;
};

// Returns LCM of arr[0..n-1]
unsigned long long int lcm(int arr[], int n)
{
    // Find the maximum value in arr[]
    int max_num = 0;
    for (int i=0; i<n; i++)
        if (max_num < arr[i])
            max_num = arr[i];
 
    // Initialize result
    unsigned long long int res = 1;
 
    // Find all factors that are present in
    // two or more array elements.
    int x = 2;  // Current factor.
    while (x <= max_num)
    {
        // To store indexes of all array
        // elements that are divisible by x.
        int indexes[10];
        int index_count = 0;
        for (int j=0; j<n; j++)
            if (arr[j]%x == 0)
                indexes[index_count++] = j;
 
        // If there are 2 or more array elements
        // that are divisible by x.
        if (index_count >= 2)
        {
            // Reduce all array elements divisible
            // by x.
            for (int j=0; j<index_count; j++)
                arr[indexes[j]] = arr[indexes[j]]/x;
 
            res = res * x;
        }
        else
            x++;
    }
 
    // Then multiply all reduced array elements
    for (int i=0; i<n; i++)
        res = res*arr[i];
 
    return res;
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

    char instructions[1000] = {0};
    int instruction_count = 0;

    // get instructions
    for(;;)
    {
        int c = fgetc(fp);
        if(c == '\n')
            break;
        instructions[instruction_count++] = c;
    }

    NetworkNode nodes[1000] = {0};
    int node_count = 0;

    // build network nodes

    char line[100] = {0};

    for(;;)
    {
        char name[3] = {0};
        char left[3] = {0};
        char right[3] = {0};

        if(fgets(line, 99, fp) == NULL)
            break;

        int matches = sscanf(line,"%c%c%c = (%c%c%c, %c%c%c)\n",
                &name[0],&name[1], &name[2],
                &left[0],&left[1], &left[2],
                &right[0],&right[1], &right[2]);

        if(matches == 9)
        {
            NetworkNode* node = &nodes[node_count++];

            memcpy(node->name,name,3*sizeof(char));
            memcpy(node->left,left,3*sizeof(char));
            memcpy(node->right,right,3*sizeof(char));
        }
    }

    fclose(fp);

#if 0
    for(int i = 0; i < node_count; ++i)
    {
        printf("Node %d: %s (%s, %s)\n", i+1, 
                nodes[i].name,
                nodes[i].left,
                nodes[i].right);
    }
#endif

    // assign pointers
    NetworkNode* start = 0;
    NetworkNode* end   = 0;

    for(int i = 0; i < node_count; ++i)
    {
        NetworkNode* node = &nodes[i];

        if(node->name[0] == 'A' && node->name[1] == 'A' && node->name[2] == 'A')
        {
            start = node;
        }
        else if(node->name[0] == 'Z' && node->name[1] == 'Z' && node->name[2] == 'Z')
        {
            end = node;
        }

        for(int j = 0; j < node_count; ++j)
        {
            if(i == j) continue;

            NetworkNode* check = &nodes[j];

            if(node->left[0] == check->name[0] && node->left[1] == check->name[1] && node->left[2] == check->name[2])
            {
                node->left_ptr = check;
            }

            if(node->right[0] == check->name[0] && node->right[1] == check->name[1] && node->right[2] == check->name[2])
            {
                node->right_ptr = check;
            }
        }
    }

    // follow instructions
    NetworkNode* node = start;

    int steps = 0;

    for(;;)
    {
        char c = instructions[steps % instruction_count];

        if(c == 'L')
        {
            node = node->left_ptr;
        }
        else if(c == 'R')
        {
            node = node->right_ptr;
        }

        steps++;

        if(node == end)
            break;
    }


    printf("1) Required Steps: %d\n", steps);

    NetworkNode* start_nodes[10] = {0};
    NetworkNode* end_nodes[10] = {0};

    int start_nodes_count = 0;
    int end_nodes_count = 0;

    for(int i = 0; i < node_count; ++i)
    {
        NetworkNode* node = &nodes[i];

        if(node->name[2] == 'A')
            start_nodes[start_nodes_count++] = node;
        else if(node->name[2] == 'Z')
            end_nodes[end_nodes_count++] = node;
    }

    NetworkNode* _nodes[10] = {0};
    for(int i = 0; i < start_nodes_count; ++i)
        _nodes[i] = start_nodes[i];

    steps = 0;

    int dist_to_z[10] = {0};

    for(;;)
    {
        char c = instructions[steps % instruction_count];

        if(c == 'L')
        {
            for(int i = 0; i < start_nodes_count; ++i)
                _nodes[i] = _nodes[i]->left_ptr;
        }
        else if(c == 'R')
        {
            for(int i = 0; i < start_nodes_count; ++i)
                _nodes[i] = _nodes[i]->right_ptr;
        }

        steps++;

        for(int i = 0; i < start_nodes_count; ++i)
        {
            if(_nodes[i]->name[2] == 'Z')
            {
                if(dist_to_z[i] == 0)
                {
                    dist_to_z[i] = steps;
                }

                bool got_numbers = true;
                for(int j = 0; j < start_nodes_count; ++j)
                {
                    if(dist_to_z[j] == 0)
                    {
                        got_numbers = false;
                        break;
                    }
                }

                if(got_numbers)
                    goto day8_done;
            }
        }
    }

day8_done:

    unsigned long long steps2 = lcm(dist_to_z, start_nodes_count);

    // 5874751351312018839: too high
    // 16342438708751
    printf("2) Required Steps: %llu\n", steps2);

}

typedef struct
{
    int n[23];
    int count;
} HistoryLevel;

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

    HistoryLevel histories[100] = {0};

    int level = 0;

    long sum1 = 0;
    long sum2 = 0;

    for(;;)
    {
        if(feof(fp))
            break;

        memset(histories,0,100*sizeof(HistoryLevel));
        level = 0;

        int* n = histories[level].n;

        // get input data
        fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                &n[1],&n[2],&n[3],&n[4],&n[5],&n[6],&n[7],&n[8],&n[9],&n[10],
                &n[11],&n[12],&n[13],&n[14],&n[15],&n[16],&n[17],&n[18],&n[19],&n[20], &n[21]);

        histories[level].count = 21;

#if 0
        for(int i = 1; i < 22; ++i)
            printf("%d ",histories[0].n[i]);

        printf("\n");
#endif

        // evaluate histories
        for(;;)
        {
            level++;
            int count = histories[level-1].count-1;
            for(int i = 1; i < count+1; ++i)
            {
                int n1 = histories[level-1].n[i];
                int n2 = histories[level-1].n[i+1];

                histories[level].n[i] = n2-n1;
                //printf("%d ",n2-n1);
            }

            //printf("\n");
            histories[level].count = count;

            bool all_zeroes = true;
            for(int i = 1; i < histories[level].count+1; ++i)
            {
                if(histories[level].n[i] != 0)
                {
                    all_zeroes = false;
                    break;
                }
            }

            if(all_zeroes)
                break;
        }

        for(int i = level; i >= 1; --i)
        {
            int count1 = histories[i-1].count;
            int count2 = histories[i].count;

            histories[i-1].n[count1+1] = histories[i].n[count2] + histories[i-1].n[count1];
            histories[i-1].n[0] = histories[i-1].n[1] - histories[i].n[0];

            histories[i-1].count++;
        }

        int next = histories[0].n[histories[0].count];
        int prev = histories[0].n[0];

        sum1 += next;
        sum2 += prev;

        //printf("Next in Sequence: %d\n",next);
        //printf("Prev in Sequence: %d\n",prev);

        //util_wait_until_key_press();
    }

    printf("1) Sum of extrapolated values: %ld\n",sum1);
    printf("2) Sum of extrapolated values: %ld\n",sum2);

    fclose(fp);
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

    fclose(fp);
}

int main(int argc, char* args[])
{
    printf("\n===================== AOC 2023 =======================\n");

    day1();
    day2();
    day3();
    day4();
    //day5();
    day6();
    day7();
    day8();
    day9();
    day10();

    printf("\n======================================================\n");
    return 0;
}
