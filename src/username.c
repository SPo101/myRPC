#include <stdlib.h>
#include <stdio.h>

#include "username.h"

int get_log_name(char **log_name){
	*log_name = malloc(100);
	*log_name = getenv("LOGNAME");
	if(*log_name)
		return SUCCESS;
	return FAILURE;
}

