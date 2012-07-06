#include "get_network.h"

Network *read_network_info(char* data_file)
{
        INI_CONFIG* config;
        Network *network;
        config = ini_config_create_from_file(data_file, 0);
        g_return_val_if_fail(config !=NULL, NULL);
       	network = g_new0(Network, 1);
        network->ping = ini_config_get_string(config, "network_info", "ping", NULL);
        network->result = ini_config_get_string(config, "network_info", "result", NULL);
        network->date = ini_config_get_string(config, "network_info", "date", NULL);
        network->os_ver = ini_config_get_string(config, "network_info", "os_ver", NULL);
        network->connected = ini_config_get_string(config, "network_info", "connected", NULL);
        network->ftp_download = ini_config_get_string(config, "network_info", "ftp_download", NULL);
        network->fail_reason = ini_config_get_string(config, "network_info", "fail_reason", NULL);
	network->lan_mac = ini_config_get_string(config, "network_info", "lan_mac", NULL);
	return network;
}

int upload_network(Network* network, MYSQL *conn_ptr, gboolean silent)
{
	g_return_val_if_fail(network != NULL, -1);
        g_return_val_if_fail(conn_ptr != NULL, -1);
	char* sql;
	sql = g_strdup_printf("insert into network ( \
		hw_id		,\
		result		,\
		ping		,\
		os_ver		,\
		date		,\
		connected	,\
		fail_reason	,\
		ftp_download	,\
		) values(	\
		'%d',\
		'%s',\
		'%s',\
		'%s',\
		'%s',\
		'%s',\
		'%s',\
		'%s'\
		)",	
		network->hw_id		,
		network->result		,
		network->ping		,
		network->os_ver		,
		network->date		,
		network->connected	,
		network->fail_reason	,
		network->ftp_download	
		);
	g_message(sql);	
	if(mysql_query(conn_ptr, sql))
	{
		g_error("insert network data error");
		return -1;
	}
	else
	{
		char* tmp;
		tmp = g_strdup_printf("The network data has upload succeed");
		if(!silent)
		{
			pop_message_box(tmp, "Successfully");
		}
	}
	return 0;
}
