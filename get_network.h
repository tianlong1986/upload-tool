#ifndef _GET_NETWORK_H
#define _GET_NETWORK_H
#include "function_common.h"

typedef struct NETWORK Network;
struct NETWORK
{
	int	hw_id;
	char*  	result;
	char* 	date;
	char* 	os_ver;
	char* 	lan_mac;
	char*	connected;
	char*	ftp_download;
	char*	fail_reason;
	char*	ping;
};
Network * read_network_info(char* data_file);
/*
 *parm silent:if silent upload,if its value is TRUE, silent upload,else show GUI.
 *
 */
int upload_network(Network *network, MYSQL *conn_ptr,gboolean silent);




#endif
