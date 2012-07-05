#include "get_hwinfo.h"
int check_if_exist(HWinfo* hwinfo, MYSQL *conn_ptr)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	g_return_val_if_fail(hwinfo != NULL, 1);
	g_return_val_if_fail(conn_ptr != NULL, 1);
	if (mysql_query(conn_ptr, "select lan_mac from hwinfo")) 
	{
		g_error("query the lan_mac failed");
		return 1;
	}
	res = mysql_use_result(conn_ptr);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if(g_strrstr(row[0], hwinfo->lan_mac))
		{
			char *msg;
			msg = g_strdup_printf("\nError: the lan MAC:%s,has existed in the database,please check,\n maybe this machine has upload,\n \
			this programer will exit,thank you!\n");
			g_print(msg);
			//pop_msg_window(msg, ERROR);	
			pop_message_box(msg, "Sorry,Error has occured");	
			exit(0);
			break;	
		}
	}
	mysql_free_result(res);
	return 0;
}
int set_hw_id(HWinfo* hwinfo, int hw_id)
{
	g_return_val_if_fail(hwinfo != NULL, -1);
	hwinfo->hw_id = hw_id;
	return 0;
}
int get_max_hwinfo_from_db (MYSQL *conn_ptr)
{
	g_return_val_if_fail(conn_ptr != NULL, -1);
	MYSQL_RES *res;
	MYSQL_ROW row;
	int max_hw_id = 0;
	if(mysql_query(conn_ptr, "select max(hw_id) from hwinfo"))
	{
		g_error("get max hw_id faild");
		return -1;
	}
	res = mysql_use_result(conn_ptr);
	row = mysql_fetch_row(res);
	if( row == NULL)
	{
		g_error("get max hw_id faild,not hw_id");
		return -1;
	}
	max_hw_id = atoi(row[0]);
	mysql_free_result(res);
	return max_hw_id;
}
HWinfo* read_hwinfo(char* data_file)
{
	INI_CONFIG* config;
	HWinfo *hwinfo;
	config = ini_config_create_from_file(data_file, 0);
	g_return_val_if_fail(config !=NULL, NULL);
	hwinfo = g_new0(HWinfo, 1);
        hwinfo->zichan_id = ini_config_get_string(config, "hardware_info", "zichan_id", NULL);
        hwinfo->model = ini_config_get_string(config, "hardware_info", "model", NULL);
        hwinfo->sku = ini_config_get_string(config, "hardware_info", "sku", NULL);
        hwinfo->vga_vendor = ini_config_get_string(config, "hardware_info", "vga_vendor", NULL);
        hwinfo->odm = ini_config_get_string(config, "hardware_info", "odm", NULL);
        hwinfo->sample_stage = ini_config_get_string(config, "hardware_info", "sample_stage", NULL);
        hwinfo->manufacture = ini_config_get_string(config, "hardware_info", "manufacture", NULL);
        hwinfo->cpu_info = ini_config_get_string(config, "hardware_info", "cpu_info", NULL);
        hwinfo->chipset = ini_config_get_string(config, "hardware_info", "chipset", NULL);
        hwinfo->memory = ini_config_get_string(config, "hardware_info", "memory", NULL);
        hwinfo->hdd1 = ini_config_get_string(config, "hardware_info", "hdd1", NULL);
        hwinfo->hdd2 = ini_config_get_string(config, "hardware_info", "hdd2", NULL);
        hwinfo->vga1_id = ini_config_get_string(config, "hardware_info", "vga1_id", NULL);
        hwinfo->vga1_desc = ini_config_get_string(config, "hardware_info", "vga1_desc", NULL);
        hwinfo->vga2_id = ini_config_get_string(config, "hardware_info", "vga2_id", NULL);
        hwinfo->vga2_desc = ini_config_get_string(config, "hardware_info", "vga2_desc", NULL);
        hwinfo->audio_id = ini_config_get_string(config, "hardware_info", "audio_id", NULL);
        hwinfo->audio_desc = ini_config_get_string(config, "hardware_info", "audio_desc", NULL);
        hwinfo->audio_codec_name = ini_config_get_string(config, "hardware_info", "audio_codec_name", NULL);
        hwinfo->audio_codec_vid = ini_config_get_string(config, "hardware_info", "audio_codec_vid", NULL);
        hwinfo->audio_codec_sid = ini_config_get_string(config, "hardware_info", "audio_codec_sid", NULL);
        hwinfo->audio_codec_revid = ini_config_get_string(config, "hardware_info", "audio_codec_revid", NULL);
        hwinfo->wlan_id = ini_config_get_string(config, "hardware_info", "wlan_id", NULL);
        hwinfo->wlan_desc = ini_config_get_string(config, "hardware_info", "wlan_desc", NULL);
        hwinfo->lan_id = ini_config_get_string(config, "hardware_info", "lan_id", NULL);
        hwinfo->lan_desc = ini_config_get_string(config, "hardware_info", "lan_desc", NULL);
        hwinfo->bluetooth_id = ini_config_get_string(config, "hardware_info", "bluetooth_id", NULL);
        hwinfo->bluetooth_desc = ini_config_get_string(config, "hardware_info", "bluetooth_desc", NULL);
        hwinfo->_3G_id = ini_config_get_string(config, "hardware_info", "3G_id", NULL);
        hwinfo->_3G_desc = ini_config_get_string(config, "hardware_info", "3G_desc", NULL);
        hwinfo->touchpad = ini_config_get_string(config, "hardware_info", "touchpad", NULL);
        hwinfo->cardreader_id = ini_config_get_string(config, "hardware_info", "cardreader_id", NULL);
        hwinfo->cardreader_desc = ini_config_get_string(config, "hardware_info", "cardreader_desc", NULL);
        hwinfo->camera_id = ini_config_get_string(config, "hardware_info", "camera_id", NULL);
        hwinfo->camera_desc = ini_config_get_string(config, "hardware_info", "camera_desc", NULL);
        hwinfo->vga_output = ini_config_get_string(config, "hardware_info", "vga_output", NULL);
        hwinfo->hdmi_output = ini_config_get_string(config, "hardware_info", "hdmi_output", NULL);
        hwinfo->hdmi_codec_name = ini_config_get_string(config, "hardware_info", "hdmi_codec_name", NULL);
        hwinfo->hdmi_codec_vid = ini_config_get_string(config, "hardware_info", "hdmi_codec_vid", NULL);
        hwinfo->hdmi_codec_sid = ini_config_get_string(config, "hardware_info", "hdmi_codec_sid", NULL);
        hwinfo->hdmi_codec_revid = ini_config_get_string(config, "hardware_info", "hdmi_codec_revid", NULL);
        hwinfo->update = ini_config_get_string(config, "hardware_info", "update", NULL);
        hwinfo->LCD = ini_config_get_string(config, "hardware_info", "LCD", NULL);
        hwinfo->keeper = ini_config_get_string(config, "hardware_info", "keeper", NULL);
        hwinfo->status = ini_config_get_string(config, "hardware_info", "status", NULL);
        hwinfo->come_from = ini_config_get_string(config, "hardware_info", "come_from", NULL);
        hwinfo->lan_mac = ini_config_get_string(config, "hardware_info", "lan_mac", NULL);
	return hwinfo;
}

int upload_hwinfo(HWinfo* hwinfo,MYSQL *conn_ptr)
{
	g_return_val_if_fail(hwinfo != NULL, -1);
	g_return_val_if_fail(conn_ptr != NULL, -1);
	check_if_exist(hwinfo, conn_ptr);
	hwinfo->hw_id = get_max_hwinfo_from_db(conn_ptr) + 1;
	char* insert_sql;
	insert_sql = g_strdup_printf("insert into hwinfo (\
               hw_id             ,\
               zichan_id             ,\
               model             ,\
               sku               ,\
               vga_vendor        ,\
               odm               ,\
               sample_stage      ,\
               manufacture       ,\
               cpu_info          ,\
               chipset           ,\
               memory            ,\
               hdd1              ,\
               hdd2              ,\
               vga1_id           ,\
               vga1_desc         ,\
               vga2_id           ,\
               vga2_desc         ,\
               audio_id          ,\
               audio_desc        ,\
               audio_codec_name  ,\
               audio_codec_vid   ,\
               audio_codec_sid   ,\
               audio_codec_revid ,\
               wlan_id           ,\
               wlan_desc         ,\
               lan_id            ,\
               lan_desc          ,\
               bluetooth_id      ,\
               bluetooth_desc    ,\
               3G_id             ,\
               3G_desc           ,\
               touchpad          ,\
               cardreader_id     ,\
               cardreader_desc   ,\
               camera_id         ,\
               camera_desc       ,\
               vga_output        ,\
               hdmi_output       ,\
               hdmi_codec_name   ,\
               hdmi_codec_vid    ,\
               hdmi_codec_sid    ,\
               hdmi_codec_revid  ,\
	       update_date	 ,\
               LCD               ,\
               keeper            ,\
               status            ,\
               come_from         ,\
               lan_mac           \
		) values( \
		'%d',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s',\		
		'%s'\		
		)",
	       hwinfo->hw_id		,
	       hwinfo->zichan_id	,
	       hwinfo->model            ,
               hwinfo->sku              ,
               hwinfo->vga_vendor       ,
               hwinfo->odm              ,
               hwinfo->sample_stage     ,
               hwinfo->manufacture      ,
               hwinfo->cpu_info         ,
               hwinfo->chipset          ,
               hwinfo->memory           ,
               hwinfo->hdd1             ,
               hwinfo->hdd2             ,
               hwinfo->vga1_id          ,
               hwinfo->vga1_desc        ,
               hwinfo->vga2_id          ,
               hwinfo->vga2_desc        ,
               hwinfo->audio_id         ,
               hwinfo->audio_desc       ,
               hwinfo->audio_codec_name ,
               hwinfo->audio_codec_vid  ,
               hwinfo->audio_codec_sid  ,
               hwinfo->audio_codec_revid,
               hwinfo->wlan_id          ,
               hwinfo->wlan_desc        ,
               hwinfo->lan_id           ,
               hwinfo->lan_desc         ,
               hwinfo->bluetooth_id     ,
               hwinfo->bluetooth_desc   ,
               hwinfo->_3G_id            ,
               hwinfo->_3G_desc          ,
               hwinfo->touchpad         ,
               hwinfo->cardreader_id    ,
               hwinfo->cardreader_desc  ,
               hwinfo->camera_id        ,
               hwinfo->camera_desc      ,
               hwinfo->vga_output       ,
               hwinfo->hdmi_output      ,
               hwinfo->hdmi_codec_name  ,
               hwinfo->hdmi_codec_vid   ,
               hwinfo->hdmi_codec_sid   ,
               hwinfo->hdmi_codec_revid ,
               hwinfo->update           ,
               hwinfo->LCD              ,
               hwinfo->keeper           ,
               hwinfo->status           ,
               hwinfo->come_from        ,
               hwinfo->lan_mac
		);
	g_message(insert_sql);
	if(mysql_query(conn_ptr, insert_sql))
	{
		g_error("insert data error");
	}
	else
	{
		char *tmp1;
		char *tmp2;
		tmp1 = g_strdup_printf("echo \"hw_id=%d\" > /tmp/hwid.log", hwinfo->hw_id);
		system(tmp1);
		tmp2 = g_strdup_printf("The data has upload successful!\n please remeber the machine id:%d,it saved in /tmp/hwid.log",hwinfo->hw_id);
		//pop_msg_window(tmp2, SUCCESS);
		pop_message_box(tmp2,"Congratulations, upload to server successfully" );
		g_print("\n****************import*************\n the machine id is:%d, you should rember it, it also saved in /tmp/hwid.log\n***************************************\n**************ByeBye*******************************\n",hwinfo->hw_id);
		g_free(tmp1);
		g_free(tmp2);
		exit(0);
	}
	//g_message(insert_sql);
	//g_free(insert_sql);
}
