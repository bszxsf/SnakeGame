#ifndef GAME_H
#define GAME_H 1

#include <stdint.h>
#include <stddef.h>

#define OPTION_START 0
#define OPTION_RANK  1
#define OPTION_EXIT  2

#define DIFFICULTY_EASY   0
#define DIFFICULTY_NORMAL 1
#define DIFFICULTY_HARD   2
#define DIFFICULTY_CANCEL 3

#define MEMORY_FAILED    -2
#define FILE_OPEN_FAILED -1
#define SUCCESS          0

#define OPTION_RECORD 0
#define OPTION_NOREC  1

#define MAP_WIDTH       28
#define MAP_HEIGHT      18
#define MAX_RANK_SIZE   10
#define PLAYER_NAME_MAX 256

#define DIR_UP    0
#define DIR_DOWN  1
#define DIR_RIGHT 2
#define DIR_LEFT  3

#if defined(__cplusplus)
   extern "C" {
#endif

typedef struct _player{
	uint32_t score;
	uint32_t cycle;
	char name[PLAYER_NAME_MAX];
	//Why is this written in this order?
	//What if we get char[16] first?
	//Hint: byte alignment
}player;

int menu(void);

int select_difficulty(void);

int game_main(int difficulty, int* cycle);

int game_over(int difficulty, uint32_t score, uint32_t cycle, player* const dst, size_t* sz);

int read_rank(int difficulty, player* dst, size_t* sz);//This will achieve no more than 10 records

int write_rank(int difficulty, player* src, size_t sz);//This needs no more than 10 records

int show_rank(player* src, size_t sz);

#if defined(__cplusplus)
   }
#endif

#endif

