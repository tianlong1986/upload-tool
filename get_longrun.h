#ifndef _GET_LONGRUN_H
#define _GET_LONGRUN_H
#include "function_common.h"

struct LONGRUN
{
	int hw_id;
	char* test_item;
	int   result;
	char* date;
	char* os_ver;
	int   cycle;
	char* cmdline;
	int   fail_cycle;
	char* kernel;
	char* lan_mac;
};
typedef struct LONGRUN Longrun;
Longrun * read_longrun_info(char* data_file);
/*
 *parm silent:if silent upload,if its value is TRUE, silent upload,else show GUI.
 *
 */
int upload_longrun(Longrun* longrun, MYSQL *conn_ptr,gboolean silent);




#endif
