#include "get_longrun.h"

Longrun *read_longrun_info(char* data_file)
{
        INI_CONFIG* config;
        Longrun *longrun;
        config = ini_config_create_from_file(data_file, 0);
        g_return_val_if_fail(config !=NULL, NULL);
       	longrun = g_new0(Longrun, 1);
        //longrun->hw_id = ini_config_get_int(config, "Longrun", "hw_id", 0);
        longrun->test_item = ini_config_get_string(config, "Longrun_info", "test_item", NULL);
        longrun->result = ini_config_get_string(config, "Longrun_info", "result", "F");
        longrun->date = ini_config_get_string(config, "Longrun_info", "date", NULL);
        longrun->os_ver = ini_config_get_string(config, "Longrun_info", "os_ver", NULL);
        longrun->cycle = ini_config_get_int(config, "Longrun_info", "cycle", 0);
        longrun->fail_cycle = ini_config_get_int(config, "Longrun_info", "fail_cycle", 0);
        longrun->cmdline = ini_config_get_string(config, "Longrun_info", "cmdline", NULL);
        longrun->kernel = ini_config_get_string(config, "Longrun_info", "kernel", NULL);
        longrun->comment = ini_config_get_string(config, "Longrun_info", "comment", NULL);
	longrun->lan_mac = ini_config_get_string(config, "Longrun_info", "lan_mac", NULL);
	return longrun;
}

int upload_longrun(Longrun* longrun, MYSQL *conn_ptr, gboolean silent)
{
	g_return_val_if_fail(longrun != NULL, -1);
        g_return_val_if_fail(conn_ptr != NULL, -1);
	char* sql;
	sql = g_strdup_printf("insert into longrun ( \
		hw_id		,\
		result		,\
		test_item	,\
		os_ver		,\
		date		,\
		cycle		,\
		cmdline		,\
		fail_cycle	,\
		comment		,\
		kernel		\
		) values(	\
		'%d',\
		'%d',\
		'%s',\
		'%s',\
		'%s',\
		'%d',\
		'%s',\
		'%d',\
		'%s',\
		'%s'\
		)",	
		longrun->hw_id		,
		longrun->result		,
		longrun->test_item	,
		longrun->os_ver		,
		longrun->date		,
		longrun->cycle		,
		longrun->cmdline	,
		longrun->fail_cycle	,
		longrun->comment	,
		longrun->kernel	
		);
	g_message(sql);	
	if(mysql_query(conn_ptr, sql))
	{
		g_error("insert longrun data error");
		return -1;
	}
	else
	{
		char* tmp;
		tmp = g_strdup_printf("The longrun data has upload succeed");
		if(!silent)
		{
			pop_message_box(tmp, "Successfully");
		}
	}
	return 0;
}
