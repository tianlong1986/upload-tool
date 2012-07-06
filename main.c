#include <stdlib.h>  
#include <stdio.h>  
#include "function_common.h"
#include "get_hwinfo.h"
#include "get_longrun.h"
#include "get_network.h"
//gcc -o test test.c `mysql_config --cflags --libs`
//gcc -o test test.c `mysql_config --cflags` -L ./*
/* 调试时编译使用*/
//#define DEBUG
#ifdef DEBUG
#define CONFIG_FILE "./config/config.ini" 
#define BG_PICTURE "./config/bk.jpg"
#define RUN1_SH "./run1.sh"
#else
#define CONFIG_FILE "/usr/local/upload/config/config.ini" 
#define BG_PICTURE "/usr/local/upload/config/bk.jpg"
#define RUN1_SH "/usr/local/upload/run1.sh"
#endif

#define DATA_FILE "/tmp/autotest.ini"
typedef struct
{
	GtkWidget *entry_hw_id;
	GtkWidget *entry_zc_id;
	GtkWidget *chk_hw_info;
	GtkWidget *chk_upload_all;
	GtkWidget *chk_longrun;
	GtkWidget *chk_nm;
	GtkWidget *btn_upload;
}UPload;
typedef enum
{
	UP_HWINFO,
	UP_ALL_TEST,
	UP_NETWORK,
	UP_LONGRUN,
	UP_POWER,
	UP_ERROR	
}ENUM_UP_TYPE;
static MYSQL* init_con_db()
{
	INI_CONFIG* ini_config;
	MYSQL *conn_ptr; 
	char *host;
	char *user;
	char *passwd;
	char *db_name;
	int  port;		
	ini_config = ini_config_create_from_file(CONFIG_FILE, 0);
	if ( ini_config == NULL)
	{
		g_error("Cannot get the init config,Confirmed the existence of ./config/config.ini?");
		exit(1);
	}
	conn_ptr = mysql_init(NULL);
	if (!conn_ptr)
	{	
		g_error("mysql_init failed");	
		exit(2);
	}
	host = ini_config_get_string(ini_config, "mysql_db", "host", "127.0.0.1");
	user = ini_config_get_string(ini_config, "mysql_db", "user", "root");
	passwd = ini_config_get_string(ini_config, "mysql_db", "passwd", NULL);
	db_name = ini_config_get_string(ini_config, "mysql_db", "db_name", "test");
	port = ini_config_get_int(ini_config, "mysql_db", "port", 0);
	conn_ptr = mysql_real_connect(conn_ptr, host, user, passwd,  db_name, port, NULL, 0);  
	if(!conn_ptr)
	{
		g_error("mysql_real_connect failed");	
		exit(3);
	}	
	ini_config_destroy(ini_config);
	return conn_ptr;
}

ENUM_UP_TYPE  check_upload_items(UPload *self)
{
	/* 以下几个判断顺序须注意 */
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->chk_hw_info)))
	{
		int len;
		len = gtk_entry_get_text_length(GTK_ENTRY(self->entry_zc_id));
		if(len < 3)	
		{
			pop_message_box("Zichan ID: asset number bits too small, correct it?","please check");	
			return UP_ERROR;
		}
		else
		{
			return UP_HWINFO;
		}
	}
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->chk_upload_all)))
	{
		return UP_ALL_TEST;
	}	
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->chk_longrun)))
	{
		return UP_LONGRUN;
	}
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->chk_nm)))
	{
		return UP_NETWORK;
	}
	return  UP_ERROR;	
}
int get_hwid_by_lanmac(MYSQL *conn_ptr,char* lanmac)
{
	char* sql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int hw_id;
	sql = g_strdup_printf("select hw_id from hwinfo where lan_mac='%s'",lanmac);		
	if(mysql_query(conn_ptr, sql))
	{
		g_warning("get the hw_id error");
		return 0;
	}
	res = mysql_use_result(conn_ptr);
	row = mysql_fetch_row(res);
	if (row == NULL)
	{
		g_warning("get the hw_id error, lan_mac=%s,not exist in db.",lanmac);
		return 0;
	}
        hw_id = atoi(row[0]);
        mysql_free_result(res);
	return hw_id;
}

void upload_data(UPload* self, ENUM_UP_TYPE uptype)
{
	MYSQL *conn_ptr = init_con_db();		
	
	switch (uptype)
	{
		case UP_HWINFO:
			{
			HWinfo* hwinfo = read_hwinfo(DATA_FILE);
			hwinfo->zichan_id = gtk_entry_get_text(GTK_ENTRY(self->entry_zc_id));
			upload_hwinfo(hwinfo, conn_ptr);
			break;
			}
		case UP_ALL_TEST:
			break;
		case UP_LONGRUN:
			{
			Longrun* longrun = read_longrun_info(DATA_FILE);
			longrun->hw_id = get_hwid_by_lanmac(conn_ptr, longrun->lan_mac);
			upload_longrun(longrun, conn_ptr, FALSE);
			break;
			}
		case UP_NETWORK:
			{
			Network * network = read_network_info(DATA_FILE);
			network->hw_id = get_hwid_by_lanmac(conn_ptr, network->lan_mac);
			upload_network(network, conn_ptr, FALSE);
			}
			break;
		default:
			;
	}
	mysql_close(conn_ptr);
}
void cb_upload(GtkWidget *widget,gpointer *data) /*定义回调函数hello，单击按钮时系统自动调用*/
{
	UPload * self;
	ENUM_UP_TYPE uptype;
	self = (UPload*)data;
	uptype = check_upload_items(self);
	if( uptype == UP_ERROR)
	{
		return;
	}
	else
	{
		upload_data(self, uptype);
	}
}

void destroy(GtkWidget *wiget,gpointer *data)/*定义回调函数destroy，关闭窗口时系统自动调用*/
{
 	gtk_main_quit();
}

void toggle_activity_hwinfo( GtkWidget *widget,UPload* self)
{
	gboolean selected = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget));
	if (selected)
	{
		gtk_widget_set_sensitive(self->entry_zc_id, TRUE);	
	}
	else
	{
		gtk_widget_set_sensitive(self->entry_zc_id, FALSE);	
	}
}

void toggle_activity_all( GtkWidget *widget,UPload* self)
{
	gboolean selected = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget));
	if (selected)
	{
		gtk_widget_set_sensitive(self->entry_zc_id, FALSE);	
	}
	else
	{
		gtk_widget_set_sensitive(self->entry_zc_id, TRUE);	
	}
}

GtkWidget * create_fixed(UPload *self)
{
	GtkWidget *fixed;
	GtkWidget *widget;
	fixed = gtk_fixed_new();
	widget = gtk_label_new("Machine ID:");
	gtk_fixed_put(GTK_FIXED(fixed), widget, 10,10);

	self->entry_hw_id = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed), self->entry_hw_id, 110,10);
	widget = gtk_label_new("Zichan ID:");
	gtk_fixed_put(GTK_FIXED(fixed), widget, 310,10);
	self->entry_zc_id = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed), self->entry_zc_id, 410,10);

	self->chk_hw_info = gtk_check_button_new_with_label("Upload ardware information");
	self->chk_upload_all = gtk_check_button_new_with_label("Upload all test result");
  	g_signal_connect(G_OBJECT( self->chk_hw_info), "clicked",
                G_CALLBACK (toggle_activity_hwinfo),self);
  	g_signal_connect(G_OBJECT( self->chk_upload_all), "clicked",
                G_CALLBACK (toggle_activity_all),self);

	gtk_fixed_put(GTK_FIXED(fixed), self->chk_hw_info, 10,60);
	gtk_fixed_put(GTK_FIXED(fixed), self->chk_upload_all, 310,60);

	self->chk_longrun = gtk_check_button_new_with_label("Upload long run result"); 
	self->chk_nm = gtk_check_button_new_with_label("Upload network result");
	gtk_fixed_put(GTK_FIXED(fixed), self->chk_longrun, 10,120);
	gtk_fixed_put(GTK_FIXED(fixed), self->chk_nm, 310,120);

 	self->btn_upload = gtk_button_new_with_label("upload");/*创建按钮*/
	gtk_fixed_put(GTK_FIXED(fixed), self->btn_upload, 250,180);

	return fixed;
}
void
update_widget_bg(GtkWidget *widget, gchar *img_file)
{      
GtkStyle *style;    
GdkPixbuf *pixbuf;      
GdkPixmap *pixmap;    
gint width, height;   

pixbuf = gdk_pixbuf_new_from_file(img_file, NULL);   
width = gdk_pixbuf_get_width(pixbuf);  
height = gdk_pixbuf_get_height(pixbuf);   
pixmap = gdk_pixmap_new(NULL, width, height, 24);  
gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, NULL, 0);
style = gtk_style_copy(GTK_WIDGET (widget)->style);   


if (style->bg_pixmap[GTK_STATE_NORMAL])       
g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);   

style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);   
style->bg_pixmap[GTK_STATE_ACTIVE] = g_object_ref(pixmap);
style->bg_pixmap[GTK_STATE_PRELIGHT] = g_object_ref(pixmap);
style->bg_pixmap[GTK_STATE_SELECTED] = g_object_ref(pixmap);
style->bg_pixmap[GTK_STATE_INSENSITIVE] = g_object_ref(pixmap);
gtk_widget_set_style(GTK_WIDGET (widget), style);
g_object_unref(style);

}

int silent_upload(char* type)
{
	if( g_strcmp0(type, "longrun") == 0 )
	{
		MYSQL *conn_ptr = init_con_db();		
		Longrun* longrun = read_longrun_info(DATA_FILE);
		longrun->hw_id = get_hwid_by_lanmac(conn_ptr, longrun->lan_mac);
		upload_longrun(longrun, conn_ptr,TRUE);
		mysql_close(conn_ptr);
		return 0;
	}	
	if( g_strcmp0(type, "network") == 0 )
	{
		MYSQL *conn_ptr = init_con_db();		
		Network * network = read_network_info(DATA_FILE);
		network->hw_id = get_hwid_by_lanmac(conn_ptr, network->lan_mac);
		upload_network(network, conn_ptr,TRUE);
		mysql_close(conn_ptr);
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) 
{  
	GtkWidget * window;
	GtkWidget * id_lable;
	GtkWidget * fixed;
	char* silent_type;
	UPload *self;
	gtk_init(&argc,&argv);
	system(RUN1_SH);
	self = g_new0(UPload, 1);
	//
	if(argc == 2)
	{
		silent_type = g_strdup(argv[1]);
		g_message("argv[1]=%s,args[2]=%s,silent_type=%s",argv[1], argv[2], silent_type);
		return silent_upload(silent_type);	
	}
	//
 	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

	/*设置当关闭窗口时，要执行的回调函数*/
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(destroy),NULL);
 	gtk_container_border_width(GTK_CONTAINER(window),10);/*设置窗口属性*/
	
	fixed = create_fixed(self);
	/*设置当单击按钮时，要执行的回调函数*/
 	g_signal_connect(GTK_OBJECT(self->btn_upload),"clicked",GTK_SIGNAL_FUNC(cb_upload),self);
 	gtk_container_add(GTK_CONTAINER(window),fixed);/*将按钮加入到窗口中*/

	gtk_window_set_title(GTK_WINDOW(window), "Linpus autotest upload tool");
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS );
//test for
	update_widget_bg(window, BG_PICTURE);
//end testt
 	gtk_widget_show(fixed);
 	gtk_widget_show_all(window);  
	gtk_main();
}  
