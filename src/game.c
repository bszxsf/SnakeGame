#include "game.h"
#include "key_control.h"
#include "console.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define START_X    13
#define START_Y    8
#define START_X2   14
#define START_Y2   8
#define START_X3   15
#define START_Y3   8
#define DFT_DIR    DIR_LEFT

int game_map[MAP_WIDTH][MAP_HEIGHT];

uint32_t cnt_cycle = 0;
int cnt_direction = DIR_LEFT;

uint16_t head_x, head_y;

void coord_map(int* x, int* y)
{
	*x = *x * 2 + 3;
	*y = *y + 2;
	return;
}

void debug(void)
{
	int i, j;
	char name[25];
	sprintf(name, "test/out%u.txt", cnt_cycle);
	FILE* a = fopen(name, "w");
	for(i = 0; i < MAP_HEIGHT; ++i){
		for(j = 0; j < MAP_WIDTH; ++j)fprintf(a, "%3d", game_map[j][i]);
		fprintf(a, "\n");
	}
	fclose(a);
	return;
}

int calc_cycle(void)
{
	int x, y;
	int val = game_map[head_x][head_y];
	switch (cnt_direction) {
	case DIR_UP:
		--head_y; break;
	case DIR_DOWN:
		++head_y; break;
	case DIR_RIGHT:
		++head_x; break;
	case DIR_LEFT:
		--head_x; break;
	}
	// hint: head_x and head_y are unsigned
	if (head_x >= MAP_WIDTH || head_y >= MAP_HEIGHT ||
		game_map[head_x][head_y] > 0) {
		return -1;
	}
	x = head_x; y = head_y;
	coord_map(&x, &y);
	move_to2(x, y);
	set_color_back(0, 255, 255);
	set_color_front(0, 255, 255);
	printf("@@");
	reset_color();
	if (game_map[head_x][head_y] == -1) {
		game_map[head_x][head_y] = val + 1;
		return 1;
	}
	x = head_x;
	y = head_y;
	game_map[x][y] = val;
	while (val) {
		game_map[x][y] = val;
		if (x > 0 && game_map[x-1][y] == val) {
			--x; --val;
			continue;
		}
		if (x + 1 < MAP_WIDTH && game_map[x+1][y] == val) {
			++x; --val;
			continue;
		}
		if (y > 0 && game_map[x][y-1] == val) {
			--y; --val;
			continue;
		}
		if (y + 1 < MAP_HEIGHT && game_map[x][y+1] == val) {
			++y; --val;
			continue;
		}
	}
	game_map[x][y] = 0;
	coord_map(&x, &y);
	move_to2(x, y);
	printf("  ");
	fflush(stdout);
	//This buffer took me hours. Huh, seriously.
	return 0;
}

int menu(void)
{
	reset_color();
	clear_console();
	printf("Snake\n\n");
	set_color_back(255, 255, 255);
	set_color_front(0, 0, 0);
	printf("> Start a new game <\n");
	reset_color();
	printf("        Rank        \n");
	printf("        Exit        \n");
	int cnt = 0;
	int key;
	move_up(3);
	while ((key = get_key()) != KEY_ENTER) {
		if (key == KEY_UP) {
			switch (cnt) {
			case 0: break;
			case 1: 
				cnt = 0;
				move_left(16);
				printf("        Rank        \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(16);
				move_up(2);
				printf("> Start a new game <\n");
				move_up(1);
				reset_color();
				break;
			case 2:
				cnt = 1;
				move_left(16);
				printf("        Exit        \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(16);
				move_up(2);
				printf("      > Rank <      \n");
				move_up(1);
				reset_color();
				break;
			}
		} else if (key == KEY_DOWN) {
			switch (cnt) {
			case 0:
				cnt = 1;
				move_left(16);
				printf("  Start a new game  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(16);
				printf("      > Rank <      \n");
				move_up(1);
				reset_color();
				break;
			case 1: 
				cnt = 2;
				move_left(16);
				printf("        Rank        \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(16);
				printf("      > Exit <      \n");
				move_up(1);
				reset_color();
				break;
			case 2: break;
			}
		}
	}
	reset_color();
	return cnt;
}

int select_difficulty(void)
{
	reset_color();
	clear_console();
	printf("Select difficulty\n\n");
	set_color_back(255, 255, 255);
	set_color_front(0, 0, 0);
	printf(" > Easy < \n");
	reset_color();
	printf("  Normal  \n");
	printf("   Hard   \n");
	printf("  Cancel  \n");
	int cnt = 0;
	int key;
	move_up(4);
	while ((key = get_key()) != KEY_NONE);
	while ((key = get_key()) != KEY_ENTER) {
		if (key == KEY_UP) {
			switch (cnt) {
			case 0: break;
			case 1: 
				cnt = 0;
				move_left(6);
				printf("  Normal  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf(" > Easy < \n");
				move_up(1);
				reset_color();
				break;
			case 2:
				cnt = 1;
				move_left(6);
				printf("   Hard   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf("> Normal <\n");
				move_up(1);
				reset_color();
				break;
			case 3:
				cnt = 2;
				move_left(6);
				printf("  Cancel  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf(" > Hard < \n");
				move_up(1);
				reset_color();
				break;
			}
		} else if (key == KEY_DOWN) {
			switch (cnt) {
			case 0:
				cnt = 1;
				move_left(6);
				printf("   Easy   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf("> Normal <\n");
				move_up(1);
				reset_color();
				break;
			case 1: 
				cnt = 2;
				move_left(6);
				printf("  Normal  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf(" > Hard < \n");
				move_up(1);
				reset_color();
				break;
			case 2:
				cnt = 3;
				move_left(6);
				printf("   Hard   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf("> Cancel <\n");
				move_up(1);
				reset_color();
				break;
			case 3: break;
			}
		}
	}
	reset_color();
	return cnt;
}

int game_main(int difficulty, int* cycle)
{
	int i, j;
	uint32_t score = 0;
	clock_t delay;
	switch (difficulty) {
	case DIFFICULTY_EASY:
		delay = 1000000; break;
	case DIFFICULTY_NORMAL:
		delay = 500000; break;
	case DIFFICULTY_HARD:
		delay = 100000; break;
	default:
		clear_console();
		fprintf(stderr, "An error has occured. Returning to menu in 5s");
		_sleep(1000000);
		fprintf(stderr, "\b\b4s");
		_sleep(1000000);
		fprintf(stderr, "\b\b3s");
		_sleep(1000000);
		fprintf(stderr, "\b\b2s");
		_sleep(1000000);
		fprintf(stderr, "\b\b1s");
		_sleep(1000000);
		return 0;
	}
	cnt_cycle = 0;
	cnt_direction = DIR_LEFT;
	clear_console();
	reset_color();
	set_color_back(255, 255, 255);
	set_color_front(255, 255, 255);
	for (i = 0; i < MAP_WIDTH + 2; ++i) printf("XX");
	printf("\n");
	for (i = 0; i < MAP_HEIGHT; ++i) {
		printf("XX");
		reset_color();
		for (j = 0; j < MAP_WIDTH; ++j) {
			printf("  ");
		}
		set_color_back(255, 255, 255);
		set_color_front(255, 255, 255);
		printf("XX\n");
	}
	for (i = 0; i < MAP_WIDTH + 2; ++i) printf("XX");
	printf("\n");
	memset(game_map, 0, sizeof(game_map));
	int x = START_X;
	int y = START_Y;
	set_color_back(0, 255, 255);
	set_color_front(0, 255, 255);
	game_map[x][y] = 3;
	coord_map(&x, &y);
	move_to2(x, y);
	printf("@@");
	x = START_X2; y = START_Y2;
	game_map[x][y] = 2;
	coord_map(&x, &y);
	move_to2(x, y);
	printf("@@");
	x = START_X3; y = START_Y3;
	game_map[x][y] = 1;
	coord_map(&x, &y);
	move_to2(x, y);
	printf("@@");
	reset_color();
	head_x = START_X;
	head_y = START_Y;
	int key, status;
	int food_x, food_y;
	srand(time(0));
	do {
		food_x = rand() % MAP_WIDTH;
		food_y = rand() % MAP_HEIGHT;
	} while (game_map[food_x][food_y] != 0);
	game_map[food_x][food_y] = -1;
	coord_map(&food_x, &food_y);
	move_to2(food_x, food_y);
	set_color_back(255, 0, 0);
	set_color_front(255, 0, 0);
	printf("..");
	fflush(stdout);
	while ((key = get_key()) != KEY_NONE); //Flush stdin
	while (1) {
		_sleep(delay);
		key = get_key();
		if (key == KEY_ESC) break; //Why can't we just while (key = get_key() != KEY_ESC)?
		if (key == KEY_UP && cnt_direction != DIR_DOWN)
			cnt_direction = DIR_UP;
		if (key == KEY_DOWN && cnt_direction != DIR_UP)
			cnt_direction = DIR_DOWN;
		if (key == KEY_LEFT && cnt_direction != DIR_RIGHT)
			cnt_direction = DIR_LEFT;
		if (key == KEY_RIGHT && cnt_direction != DIR_LEFT)
			cnt_direction = DIR_RIGHT;
		status = calc_cycle();
		if (status < 0) {
			reset_color();
			clear_console();
			break;
		} else if (status > 0) {
			++score;
			do {
				food_x = rand() % MAP_WIDTH;
				food_y = rand() % MAP_HEIGHT;
			} while (game_map[food_x][food_y] != 0);
			game_map[food_x][food_y] = -1;
			coord_map(&food_x, &food_y);
			move_to2(food_x, food_y);
			set_color_back(255, 0, 0);
			set_color_front(255, 0, 0);
			printf("..");
			reset_color();
		}
		record_cursor();
		move_to2(0, MAP_HEIGHT + 3);
		printf("Score: %3d Cycle: %u", score, cnt_cycle);
		restore_cursor();
		++cnt_cycle;
	}
	*cycle = cnt_cycle;
	reset_color();
	return score;
}

//length of dst must be more than 10
//sz will always be overwritten
int game_over(int difficulty, uint32_t score, uint32_t cycle, player* const dst, size_t* sz)
{
	reset_color();
	clear_console();
	printf("Game over with\n");
	_sleep(500000);
	printf("%d pts\n", score);
	_sleep(500000);
	printf("in %d cycles\n\n", cycle);
	_sleep(500000);
	int key = KEY_NONE;
	if (score == 0) {
		set_color_back(255, 255, 255);
		set_color_front(0, 0, 0);
		printf("> That's too bad <\n");
		while (key != KEY_ENTER && key != KEY_ESC)
			key = get_key();
		reset_color();
		return OPTION_NOREC;
	}
	int ret;
	if (dst == NULL) {
		set_color_back(255, 255, 255);
		set_color_front(0, 0, 0);
		printf("OK\n");
		while (key != KEY_ENTER && key != KEY_ESC)
			key = get_key();
		reset_color();
		return OPTION_NOREC;
	}
	*sz = MAX_RANK_SIZE;
	if ((ret = read_rank(difficulty, dst, sz)) < 0) {
		*sz = 0;
		clear_console();
		printf("Oops...\n\n");
		fprintf(stderr, "Failed to read the rank. Returning to menu in 5s");
		_sleep(1000000);
		fprintf(stderr, "\b\b4s");
		_sleep(1000000);
		fprintf(stderr, "\b\b3s");
		_sleep(1000000);
		fprintf(stderr, "\b\b2s");
		_sleep(1000000);
		fprintf(stderr, "\b\b1s");
		_sleep(1000000);
		return OPTION_NOREC;
	}
	if (*sz + 1 >= MAX_RANK_SIZE && (dst[MAX_RANK_SIZE - 1].score > score ||
		(dst[MAX_RANK_SIZE - 1].score == score && dst[MAX_RANK_SIZE - 1].cycle <= cycle))) {
		printf("Your score is too low to be recorded :(\n");
		set_color_back(255, 255, 255);
		set_color_front(0, 0, 0);
		printf("That's too bad\n");
		while (key != KEY_ENTER && key != KEY_ESC)
			key = get_key();
		reset_color();
		return OPTION_NOREC;
	}
	int cnt = 0;
	printf("Your score is high enough to be recorded. Create a new record?\n");
	set_color_back(255, 255, 255);
	set_color_front(0, 0, 0);
	printf("   > Sure <   \n");
	reset_color();
	printf("  No, thanks  \n");
	move_left(9);
	move_up(2);
	while ((key = get_key()) != KEY_NONE);
	while ((key = get_key()) != KEY_ENTER) {
		if (key == KEY_UP) {
			switch (cnt) {
			case 0: break;
			case 1: 
				cnt = 0;
				move_left(9);
				printf("  No, thanks  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(9);
				move_up(2);
				printf("   > Sure <   \n");
				move_up(1);
				reset_color();
				break;
			}
		} else if (key == KEY_DOWN) {
			switch (cnt) {
			case 0:
				cnt = 1;
				move_left(9);
				printf("     Sure     \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(9);
				printf("> No, thanks <\n");
				move_up(1);
				reset_color();
				break;
			case 1: break;
			}
		}
	}
	reset_color();
	return cnt;
}

int write_rank(int difficulty, player* const src, size_t sz)
{
	char buf[10];
	sprintf(buf, "rank_%d", difficulty);
	FILE* rank = fopen(buf, "w");
	if (rank == NULL)
		return FILE_OPEN_FAILED;
	if (src == NULL) {
		//You may add a path with readlink
		fclose(rank);
		return 0;
	}
	size_t i;
	if (sz > MAX_RANK_SIZE) sz = MAX_RANK_SIZE;
	for (i = 0; i < sz; ++i)
		fprintf(rank, "%u %u %s\n", src[i].score, src[i].cycle, src[i].name);
	fclose(rank);
	return sz;
}

int read_rank(int difficulty, player* dst, size_t* sz)
{
	if (dst == NULL) return 0;
	char buf[10];
	sprintf(buf, "rank_%d", difficulty);
	int status;
	FILE* rank = fopen(buf, "r");
	if (rank == NULL) {
		status = write_rank(difficulty, NULL, 0);
		if (status < 0) return status;
		rank = fopen(buf, "r");
		if (rank == NULL) return FILE_OPEN_FAILED;
	}
	char buff[PLAYER_NAME_MAX];
	uint32_t t, t2;
	size_t i = 0;
	if (*sz > MAX_RANK_SIZE) *sz = MAX_RANK_SIZE;
	while (i < *sz && fscanf(rank, " %u %u %[^\n]s", &t, &t2, buff) != EOF) {
		strcpy(dst[i].name, buff);
		//Grammar basis: What if we try player[i].name = buf?
		dst[i].score = t;
		dst[i].cycle = t2;
		++i;
	}
	*sz = i;
	fclose(rank);
	return i;
}

int show_rank(player* src, size_t sz)
{
	int difficulty;
	_Bool f = 0;
	int key = KEY_NONE;
	int cnt = 0;
	reset_color();
	clear_console();
	printf("Select which difficulty to check\n\n");
	set_color_back(255, 255, 255);
	set_color_front(0, 0, 0);
	printf(" > Easy < \n");
	reset_color();
	printf("  Normal  \n");
	printf("   Hard   \n");
	printf("  Cancel  \n");
	move_left(6);
	move_up(4);
	while ((key = get_key()) != KEY_NONE);
	while ((key = get_key()) != KEY_ENTER) {
		if (key == KEY_UP) {
			switch (cnt) {
			case 0: break;
			case 1: 
				cnt = 0;
				move_left(6);
				printf("  Normal  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf(" > Easy < \n");
				move_up(1);
				reset_color();
				break;
			case 2: 
				cnt = 1;
				move_left(6);
				printf("   Hard   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf("> Normal <\n");
				move_up(1);
				reset_color();
				break;
			case 3: 
				cnt = 2;
				move_left(6);
				printf("  Cancel  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				move_up(2);
				printf(" > Hard < \n");
				move_up(1);
				reset_color();
				break;
			}
		} else if (key == KEY_DOWN) {
			switch (cnt) {
			case 0:
				cnt = 1;
				move_left(6);
				printf("   Easy   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf("> Normal <\n");
				move_up(1);
				reset_color();
				break;
			case 1:
				cnt = 2;
				move_left(6);
				printf("  Normal  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf(" > Hard < \n");
				move_up(1);
				reset_color();
				break;
			case 2:
				cnt = 3;
				move_left(6);
				printf("   Hard   \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(6);
				printf("> Cancel <\n");
				move_up(1);
				reset_color();
				break;
			case 3: break;
			}
		}
	}
	if (cnt == 3) return 0;
	difficulty = cnt;
	reset_color();
	clear_console();
	printf("Rank - ");
	switch (difficulty) {
	case 0:
		printf("Easy\n\n");break;
	case 1:
		printf("Normal\n\n");break;
	case 2:
		printf("Hard\n\n");break;
	}
	if (src == NULL) {
		sz = MAX_RANK_SIZE;
		src = malloc(MAX_RANK_SIZE * sizeof(player));
		if (src == NULL) return MEMORY_FAILED;
		f = 1;
	}
	if (read_rank(difficulty, src, &sz) < 0) {
		if (f) free(src);
		return FILE_OPEN_FAILED;
	}
	if (sz == 0) {
		printf("No record found\n\n");
		set_color_back(255, 255, 255);
		set_color_front(0, 0, 0);
		printf("OK\n");
		while ((key = get_key()) != KEY_NONE);
		while (key != KEY_ENTER && key != KEY_ESC)
			key = get_key();
		reset_color();
		if (f) free(src);
		return 0;
	}
	size_t i;
	printf("No Score  Cycles  User\n");
	for (i = 0; i < sz; ++i)
		printf("%2d %5u  %6u  %s\n", (int)(i + 1), src[i].score, src[i].cycle, src[i].name);
	printf("\n");
	if (f) free(src);
	set_color_back(255, 255, 255);
	set_color_front(0, 0, 0);
	printf("> Return to menu <\n");
	reset_color();
	printf("   Clear records  \n");
	move_left(14);
	move_up(2);
	cnt = 0;
	while ((key = get_key()) != KEY_NONE);
	while ((key = get_key()) != KEY_ENTER) {
		if (key == KEY_UP) {
			switch (cnt) {
			case 0: break;
			case 1: 
				cnt = 0;
				move_left(14);
				printf("   Clear records  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(14);
				move_up(2);
				printf("> Return to menu <\n");
				move_up(1);
				reset_color();
				break;
			}
		} else if (key == KEY_DOWN) {
			switch (cnt) {
			case 0:
				cnt = 1;
				move_left(14);
				printf("  Return to menu  \n");
				set_color_back(255, 255, 255);
				set_color_front(0, 0, 0);
				move_left(14);
				printf(" > Clear records <\n");
				move_up(1);
				reset_color();
				break;
			case 1: break;
			}
		}
	}
	if (cnt == 1) {
		cnt = 0;
		reset_color();
		clear_console();
		printf("Are you sure? (This cannot be restored)\n\n");
		set_color_back(255, 255, 255);
		set_color_front(0, 0, 0);
		printf("> Yes <\n");
		reset_color();
		printf("   No  \n");
		move_left(3);
		move_up(2);
		while ((key = get_key()) != KEY_NONE);
		while ((key = get_key()) != KEY_ENTER) {
			if (key == KEY_UP) {
				switch (cnt) {
				case 0: break;
				case 1: 
					cnt = 0;
					move_left(3);
					printf("   No  \n");
					set_color_back(255, 255, 255);
					set_color_front(0, 0, 0);
					move_left(3);
					move_up(2);
					printf("> Yes <\n");
					move_up(1);
					reset_color();
					break;
				}
			} else if (key == KEY_DOWN) {
				switch (cnt) {
				case 0:
					cnt = 1;
					move_left(3);
					printf("  Yes  \n");
					set_color_back(255, 255, 255);
					set_color_front(0, 0, 0);
					move_left(3);
					printf(" > No <\n");
					move_up(1);
					reset_color();
					break;
				case 1: break;
				}
			}
		}
		if (cnt == 0) write_rank(difficulty, NULL, 0);
	}
	reset_color();
	return 0;
}

