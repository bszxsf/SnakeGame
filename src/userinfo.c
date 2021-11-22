#include "userinfo.h"
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>

//Not null-terminated
size_t get_username(char* dst, size_t sz)
{
	struct passwd* t;
	t = getpwuid(getuid());
	if (t == NULL) return 0;
	size_t s = strlen(t->pw_name);
	if (s < sz) sz = s;
	memcpy(dst, t->pw_name, sz);
	return sz;
}

//Not null-terminated
size_t get_hostname(char* dst, size_t sz)
{
	char buf[HOST_NAME_MAX + 1];
	if (gethostname(buf, HOST_NAME_MAX + 1) < 0)
		return 0;
	size_t s = strlen(buf);
	if (s < sz) sz = s;
	memcpy(dst, buf, sz);
	return sz;
}

//Null-terminated
size_t get_entire_name(char* dst, size_t sz, size_t sz_username)
{
	if (sz_username >= NAME_MAX) return 0;
	if (sz_username >= sz) return 0;
	size_t s1, s2;
	char buf[NAME_MAX];
	s1 = get_username(buf, sz_username - 1);
	if (s1 == 0) return 0;
	buf[s1] = '@';
	s2 = get_hostname(buf + s1 + 1, sz - sz_username - 1);
	if (s2 == 0) return 0;
	buf[s1 + s2 + 1] = '\0';
	memcpy(dst, buf, s1 + s2 + 2);
	return s1 + s2 + 1;
}

