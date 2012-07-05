#ifndef _GET_HWINFO_H
#define _GET_HWINFO_H
#include "function_common.h"
struct HWINFO 
{
	int  hw_id		;
        char* zichan_id         ; 
        char* model             ; 
        char* sku               ; 
        char* vga_vendor        ; 
        char* odm               ; 
        char* sample_stage      ; 
        char* manufacture       ; 
        char* cpu_info          ; 
        char* chipset           ; 
        char* memory            ; 
        char* hdd1              ; 
        char* hdd2              ; 
        char* vga1_id           ; 
        char* vga1_desc         ; 
        char* vga2_id           ; 
        char* vga2_desc         ; 
        char* audio_id          ; 
        char* audio_desc        ; 
        char* audio_codec_name  ; 
        char* audio_codec_vid   ; 
        char* audio_codec_sid   ; 
        char* audio_codec_revid ; 
        char* wlan_id           ; 
        char* wlan_desc         ; 
        char* lan_id            ; 
        char* lan_desc          ; 
        char* bluetooth_id      ; 
        char* bluetooth_desc    ; 
        char* _3G_id            ; 
        char* _3G_desc          ; 
        char* touchpad          ; 
        char* cardreader_id     ; 
        char* cardreader_desc   ; 
        char* camera_id         ; 
        char* camera_desc       ; 
        char* vga_output        ; 
        char* hdmi_output       ; 
        char* hdmi_codec_name   ; 
        char* hdmi_codec_vid    ; 
        char* hdmi_codec_sid    ; 
        char* hdmi_codec_revid  ; 
        char* update            ; 
        char* LCD               ; 
        char* keeper            ; 
        char* status            ; 
        char* come_from         ; 
        char* lan_mac           ;	
};
typedef struct HWINFO HWinfo;

HWinfo* read_hwinfo(char* data_file);
int upload_hwinfo(HWinfo* hwinfo,MYSQL *conn_ptr);
int set_hw_id(HWinfo* hwinfo, int hw_id);
int get_max_hwinfo_from_db(MYSQL *conn_ptr);

#endif
