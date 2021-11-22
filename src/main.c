#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "key_control.h"
#include "console.h"
#include "game.h"
#include "userinfo.h"
#include "util.h"

#ifndef _WIN32
#  include <unistd.h>
#endif

void sighandler(int signum)
{
	(void)signum;
	reset_color();
	show_cursor();
	end_key_control();
	clear_console();
	exit(0);
	return;
}

void segvhandler(int signum)
{
	(void)signum;
	reset_color();
	show_cursor();
	end_key_control();
	clear_console();
	fprintf(stderr, "Segmentation fault");
	abort();
	return;
}

int cmp(const void* _a, const void* _b)
{
	player* a = (player*)_a;
	player* b = (player*)_b;
	if (a->score == b->score) {
		if (a->cycle == b->cycle)
			return strcmp(a->name, b->name);
		return a->cycle > b->cycle;
	}
	return a->score < b->score;
}

int main()
{
	signal(SIGINT, sighandler);
	signal(SIGSEGV, segvhandler);
	clear_console();
	if (init_key_control() < 0) {
		end_key_control();
		fprintf(stderr, "Fatal error: init_key_control failed. Aborted\n");
		return 1;
	}
	hide_cursor();
	int opt, score, cycle, d;
	size_t sz = 0;
	size_t t;
	player buf[10];
	while ((opt = menu()) != OPTION_EXIT) {
		if (opt == OPTION_START) {
			d = select_difficulty();
			if (d == DIFFICULTY_CANCEL) continue;
			score = game_main(d, &cycle);
			opt = game_over(d, score, cycle, buf, &sz);
			if (opt == OPTION_NOREC) continue;
			if (sz == MAX_RANK_SIZE) --sz;
			t = get_entire_name(buf[sz].name, 256, 65);
			if (t > 0) {
				buf[sz].score = score;
				buf[sz].cycle = cycle;
				++sz;
				qsort(buf, sz, sizeof(player), cmp);
				t = write_rank(d, buf, sz);
			}
			if (t == 0) {
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
			}
		} else if (opt == OPTION_RANK) {
			if (sz > 0) show_rank(buf, sz);
			else show_rank(NULL, MAX_RANK_SIZE);
		}
	}
	reset_color();
	show_cursor();
	end_key_control();
	clear_console();
	return 0;
}

