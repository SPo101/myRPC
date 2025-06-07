#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

char *LEVELS[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
int mysyslog(const char* msg, int level, int driver, int format, const char* path){
	int fd = open(path, O_WRONLY|O_CREAT|O_APPEND, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);

	char *proc = "My_syslog";

	dprintf(fd, "{\"timestamp\":%ld, \"log_level\": %s, \"process\": %s, \"message\": %s}\n", time(NULL), LEVELS[level-1], proc, msg);

	close(fd);
	return 0;
}
