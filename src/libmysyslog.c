#include <syslog.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "mysyslog.h"

#define JSON "libmysyslog-json.so"
#define TEXT "libmysyslog-text.so"

#define FUNC "mysyslog"

char *DRIVERS[] = {TEXT, JSON};

int mysyslog(const char *msg, int level, int driver, int format, const char* path){

	if(msg == NULL || path == NULL)
		return 1;
	
	void *library;
	void (*log_func)(const char*, int, int, int, const char*);
	
	library = dlopen(DRIVERS[driver], RTLD_LAZY|RTLD_LOCAL);
	if(library == NULL){
		fprintf(stderr, "Opening lib: %s", dlerror());
		return 1;
	}
	dlerror();
		
	log_func = dlsym(library, FUNC);
	if(log_func == NULL){
		fprintf(stderr, "Calling func: %s", dlerror());
		return 1;
	}
	dlerror();

	log_func(msg, level, driver, format, path);

	if(dlclose(library) != 0){
		fprintf(stderr, "Closing lib: %s", dlerror());
		return 1;
	}
	dlerror();

	return 0;
}
