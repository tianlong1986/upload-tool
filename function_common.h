#ifndef _FUNCTION_COMMON_H
#define _FUNCTION_COMMON_H
#include "confile.h"
#include <mysql/mysql.h>
#include <gtk/gtk.h>
enum WINDOW_TYPE
{
        ERROR,
        SUCCESS,
        MESSAGE
};

void pop_message_box(char* msg, char* title);
void pop_msg_window(char* msg,enum WINDOW_TYPE type );

#endif
