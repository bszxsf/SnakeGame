#include "key_control.h"
#include <termios.h>
#include <stdio.h>

#ifdef _WIN32
#  include <conio.h>
#else
#  include <termios.h>
#  include <unistd.h>
#  include <fcntl.h>
#endif

static struct termios backup;

int set_icanon(void)
{
	int ret;
	struct termios opt;
	if ((ret = tcgetattr(STDIN_FILENO, &opt)) < 0) {
		perror("set_icanon - tcgetattr");
		return ret;
	}
	opt.c_lflag |= ICANON;
	if ((ret = tcsetattr(STDIN_FILENO, TCSANOW, &opt)) < 0) {
		perror("set_icanon - tcsetattr");
		return ret;
	}
	return 0;
}

int unset_icanon(void)
{
	int ret;
	struct termios opt;
	if ((ret = tcgetattr(STDIN_FILENO, &opt)) < 0) {
		perror("set_icanon - tcgetattr");
		return ret;
	}
	opt.c_lflag &= (~ICANON);
	if ((ret = tcsetattr(STDIN_FILENO, TCSANOW, &opt)) < 0) {
		perror("set_icanon - tcsetattr");
		return ret;
	}
	return 0;
}

int init_key_control(void)
{
	int ret;
	if ((ret = fcntl(STDIN_FILENO, F_GETFL, 0)) < 0) {
		perror("init_key_control - fcntl - F_GETFL");
		return ret;
	}
	if ((ret = fcntl(STDIN_FILENO, F_SETFL, ret | O_NONBLOCK)) < 0) {
		perror("init_key_control - fcntl - F_SETFL");
		return ret;
	}
	if ((ret = tcgetattr(STDIN_FILENO, &backup)) < 0) {
		perror("init_key_control - tcgetattr");
		return ret;
	}
	struct termios opt = backup;
	opt.c_lflag &= (~ICANON);
	opt.c_cc[VTIME] = 0;
	opt.c_cc[VMIN] = 1;
	opt.c_lflag |= ISIG;
	opt.c_lflag &= (~ECHO);
	opt.c_iflag &= (~IGNBRK);
	opt.c_iflag |= BRKINT;
	if ((ret = tcsetattr(STDIN_FILENO, TCSANOW, &opt)) < 0) {
		perror("init_key_control - tcsetattr");
		return ret;
	}
	return 0;
}

int end_key_control(void)
{
	int ret;
	if ((ret = tcsetattr(STDIN_FILENO, TCSANOW, &backup)) < 0) {
		perror("end_key_control - tcsetattr");
		return ret;
	}
	return 0;
}

int get_key(void)
{
	char buf[8];
	int sz, i;
	int ret = 0;
	sz = read(STDIN_FILENO, buf, 8);
	if (sz <= 0) return KEY_NONE;
	for (i = 0; i < sz; ++i) {
		ret <<= 8;
		ret |= (unsigned int)buf[i];
	}
	return ret;
}

