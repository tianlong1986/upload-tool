#include "function_common.h"
/* 此函数有问题，暂废弃 */
void pop_message_box(char* msg, char* title)
{
	GtkDialog *dialog ;
	/*dialog = gtk_dialog_new_with_buttons("message box",
                                        NULL,
                                        GTK_DIALOG_MODAL, 
                                        "退出"
                                        );*/
	dialog = GTK_WIDGET(gtk_dialog_new());
        GtkWidget *button;
        GtkWidget *label;
        button = gtk_button_new_with_label ("OK");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG (dialog)->action_area), button, TRUE, TRUE, 0);
    	gtk_widget_show (button);
 	label = gtk_label_new (msg);
    	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox),label, TRUE, TRUE, 0);
    	gtk_widget_show (label);
	gtk_container_border_width(GTK_CONTAINER(dialog),30);
	gtk_window_set_title(GTK_WINDOW(&GTK_DIALOG(dialog)->window), title);
	gtk_widget_show(GTK_WIDGET(dialog));
        g_signal_connect_swapped (G_OBJECT (button), "clicked",G_CALLBACK (gtk_widget_destroy),dialog);
	gtk_dialog_run(dialog);
}
void pop_msg_window(char* msg,enum WINDOW_TYPE type )
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *btn_exit;
	GtkWidget *label;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS );
	gtk_container_border_width(GTK_CONTAINER(window),80);
	label = gtk_label_new (msg);
	btn_exit = gtk_button_new_with_label ("Exit");
	vbox = gtk_vbox_new(TRUE, 0);

	gtk_box_pack_start(GTK_BOX (vbox), label, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX (vbox), btn_exit, FALSE, FALSE, 5);


	gtk_container_add (GTK_CONTAINER (window), vbox);

	switch(type)
	{
		case ERROR:
			gtk_window_set_title(GTK_WINDOW(window), "Sorry, error occurred");	
			g_signal_connect_swapped (G_OBJECT (btn_exit), "clicked",
				G_CALLBACK (gtk_main_quit),
				window);
			break;
		case SUCCESS:
			gtk_window_set_title(GTK_WINDOW(window), "Congratulations,successful");	
			g_signal_connect_swapped (G_OBJECT (btn_exit), "clicked",
				G_CALLBACK (gtk_main_quit),
				window);
			break;
		default:
			g_message("window=%ld",(long)window);
			gtk_window_set_title(GTK_WINDOW(window), "Message box");
			g_signal_connect_swapped (G_OBJECT (btn_exit), "clicked",
				G_CALLBACK (gtk_widget_destroy),
				G_OBJECT(window));
	}

	gtk_widget_show(label);	
	gtk_widget_show(btn_exit);	
	gtk_widget_show(vbox);	
	gtk_widget_show(window);	
}

