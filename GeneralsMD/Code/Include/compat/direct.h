#ifndef ZH_COMPAT_DIRECT_H
#define ZH_COMPAT_DIRECT_H

#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif
int _mkdir(const char* path);
int _chdir(const char* path);
char* _getcwd(char* buffer, int maxlen);
int _rmdir(const char* path);
#ifdef __cplusplus
}
#endif

#else

#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

static inline int _mkdir(const char* path)
{
	return mkdir(path, 0777);
}

static inline int _chdir(const char* path)
{
	return chdir(path);
}

static inline char* _getcwd(char* buffer, int maxlen)
{
	return getcwd(buffer, (size_t)maxlen);
}

static inline int _rmdir(const char* path)
{
	return rmdir(path);
}

#endif

#endif
