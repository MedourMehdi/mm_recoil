#include "control_bar.h"
#include "new_window.h"
#include "utils/utils.h"
#include "file.h"

#include "img_png/img_png.h"

typedef struct {
    char        export_path[256];
    char        export_dir[192];
    char        file_without_ext[64];
    char        export_extension[6];
    char        export_comments[64];
    int16_t     export_width;
    int16_t     export_height;
    int16_t     export_components;
    u_int8_t    *export_data;
    void        *(*export_function)(void*);
} struct_export;

#define GREY_COLOR 0x7AC0C0C0

/* 
*	mm_png_lib
*/

/* Functions executed when you click an icon from the control bar */

/* MFDB for image control bar icons */
struct_st_ico_png st_ico_1_mfdb, st_ico_2_mfdb, st_ico_3_mfdb, st_ico_4_mfdb, st_ico_5_mfdb;
struct_st_ico_png st_ico_6_mfdb, st_ico_7_mfdb, st_ico_8_mfdb, st_ico_9_mfdb, st_ico_10_mfdb;
struct_st_ico_png st_ico_11_mfdb, st_ico_12_mfdb, st_ico_13_mfdb, st_ico_14_mfdb;
struct_st_ico_png st_ico_wdoc_1_mfdb, st_ico_wdoc_2_mfdb, st_ico_wdoc_3_mfdb, st_ico_wdoc_4_mfdb;
struct_st_ico_png st_ico_wdoc_7_mfdb, st_ico_wdoc_8_mfdb, st_ico_wdoc_9_mfdb, st_ico_wdoc_10_mfdb;
struct_st_ico_png st_ico_wdoc_11_mfdb, st_ico_wdoc_12_mfdb;
struct_st_ico_png st_ico_wvid_13_mfdb, st_ico_wvid_14_mfdb;

struct_st_ico_png st_ico_wstart_1_mfdb, st_ico_wstart_2_mfdb, st_ico_wstart_3_mfdb;

/*	Here you declare an array of struct with :
*		- index: an unique index associated to the icon you want to display in the control bar. Negative index signals the end of the array and must end it
*		- main icon path: must match the icon path, i.e. where your icon is located
*		- mask like icon path: when you click the main icon the mask path is showed - Let it null if no mask icon is needed
*		- main icon mfdb, must be declared before the struct array
*		- mask icon mfdb, must be declared before the struct array
*		- funtion executed when the icon was clicked - void function with a void pointer parameter
*		- x coordonate relative to the control bar
*		- y coordonate relative to the control bar
*		- mask icon status, this is handled by the lib so this value should always be FALSE
*/
#ifdef ENABLE_IMAGE
struct_st_ico_png_list control_bar_winimage_list[] = {
	{	1,		 "\\ico24\\open.png",		NULL,		&st_ico_1_mfdb,		NULL,		st_Img_Open, 	12,		4 ,		FALSE	},
	{	2,		 "\\ico24\\collapse.png",		"\\ico24\\expand.png",		&st_ico_3_mfdb,		&st_ico_4_mfdb,		st_Img_Autoscale, 	48,		4 ,		FALSE	},
	{	3,		 "\\ico24\\reload.png",		NULL,		&st_ico_5_mfdb,		NULL,		st_Img_Reload, 	80,		4 ,		FALSE	},
	{	4,		 "\\ico24\\cut.png",		NULL,		&st_ico_7_mfdb,		NULL,		st_Img_Crop, 	112,		4 ,		FALSE	},
	{	5,		 "\\ico24\\rotate.png",		NULL,		&st_ico_8_mfdb,		NULL,		st_Img_Rotate, 	144,		4 ,		FALSE	},
	{	6,		 "\\ico24\\zoom_in.png",		NULL,		&st_ico_9_mfdb,		NULL,		st_Img_ZoomIn, 	176,		4 ,		FALSE	},
	{	7,		 "\\ico24\\zoom_out.png",		NULL,		&st_ico_10_mfdb,		NULL,		st_Img_ZoomOut, 	208,		4 ,		FALSE	},
	{	8,		 "\\ico24\\png.png",		NULL,		&st_ico_11_mfdb,		NULL,		st_Img_Export_to_PNG, 	240,		4 ,		FALSE	},	
	{	-1,		NULL,					NULL, 				NULL, 			 		NULL,				NULL,			0,		0 ,		0	},
};
#endif
struct_st_ico_png_list control_bar_winstart_list[] = {
	{	1,		 "\\ico24\\open.png",		NULL,		&st_ico_wstart_1_mfdb,		NULL,		st_Img_Open, 	12,		4 ,		FALSE	},
	{	2,		 "\\ico24\\cut.png",		NULL,		&st_ico_wstart_2_mfdb,		NULL,		st_Img_Crop, 	48,		4 ,		FALSE	},
	{	3,		 "\\ico24\\quit.png",		NULL,		&st_ico_wstart_3_mfdb,		NULL,		st_Img_Close_All, 	80,		4 ,		FALSE	},
	{	-1,		NULL,					NULL, 				NULL, 			 		NULL,				NULL,			0,		0 ,		0	},
};
#ifdef ENABLE_DOCUMENT
struct_st_ico_png_list control_bar_windocument_list[] = {
	{	1,		 "\\ico24\\open.png",		NULL,		&st_ico_wdoc_1_mfdb,		NULL,		st_Img_Open, 	12,		4 ,		FALSE	},
	{	2,		 "\\ico24\\collapse.png",		"\\ico24\\expand.png",		&st_ico_wdoc_3_mfdb,		&st_ico_wdoc_4_mfdb,		st_Img_Autoscale, 	48,		4 ,		FALSE	},
	{	3,		 "\\ico24\\up.png", NULL,		&st_ico_wdoc_11_mfdb,		NULL,		st_Img_up, 	80,		4 ,		FALSE	},
	{	4,		 "\\ico24\\down.png",		NULL,		&st_ico_wdoc_12_mfdb,		NULL,		st_Img_down, 	112,		4 ,		FALSE	},
	{	5,		 "\\ico24\\cut.png",		NULL,		&st_ico_wdoc_7_mfdb,		NULL,		st_Img_Crop, 	144,		4 ,		FALSE	},
	{	6,		 "\\ico24\\rotate.png",		NULL,		&st_ico_wdoc_8_mfdb,		NULL,		st_Img_Rotate, 	176,		4 ,		FALSE	},
	{	7,		 "\\ico24\\zoom_in.png",		NULL,		&st_ico_wdoc_9_mfdb,		NULL,		st_Img_ZoomIn, 	208,		4 ,		FALSE	},
	{	8,		 "\\ico24\\zoom_out.png",		NULL,		&st_ico_wdoc_10_mfdb,		NULL,		st_Img_ZoomOut, 	240,		4 ,		FALSE	},
	{	-1,		NULL,					NULL, 				NULL, 			 		NULL,				NULL,			0,		0 ,		0	},
};
#endif

#ifdef ENABLE_VIDEO
struct_st_ico_png_list control_bar_winvideo_list[] = {
	{	1,		 "\\ico24\\play.png",		"\\ico24\\pause.png",		&st_ico_wvid_13_mfdb,		&st_ico_wvid_14_mfdb,		st_Img_Play, 	12,		4 ,		FALSE	},
	{	-1,		NULL,					NULL, 				NULL, 			 		NULL,				NULL,			0,		0 ,		0	},
};
#endif

/*	Your control bar declaration */

// struct_st_control_bar my_control_bar; /* I use a malloc inside my init function so no need for me to declare on here */
void st_Init_Default_Values_Control_Bar(struct_window *this_win);

void st_Init_Default_Values_Control_Bar(struct_window *this_win){
	/* A right padding if you want an icon is showed at the opposite of the others i.e. for example main icons to the left but one of them to the right */
	this_win->wi_control_bar->last_ico_padding_right = 72;
	/* When control_bar_h is equal to zero the control bar was hidden - this value represent the height of the control bar */
	this_win->wi_control_bar->control_bar_h = 0;
	/* If you want some transparency filter set transparency to TRUE */
	this_win->wi_control_bar->transparency = FALSE;
	// this_win->wi_control_bar->transparency = TRUE;
	if(cpu_type < 40 || computer_type < 5){
		this_win->wi_control_bar->transparency = FALSE;
		/* Disabling transparency computing on ST */
	}
	this_win->wi_control_bar->background_mfdb = NULL;
	this_win->wi_control_bar->need_to_reload_control_bar = TRUE;
	/* Transparency color - ARGB: value 'A' represent the transparecny level */
	this_win->wi_control_bar->transparency_color = GREY_COLOR;
	/* VDI handle */
	this_win->wi_control_bar->vdi_handle = &st_vdi_handle;
	/* Screen MFDB - You may obtained it with a declaration like MFDB screen_mfdb = {0}; */
	this_win->wi_control_bar->virtual_screen_mfdb = &screen_mfdb;
	/* We want hide the control bar with the right click */
	this_win->wi_control_bar->force_unhide = FALSE;
}

#ifdef ENABLE_IMAGE

void st_Init_WinImage_Control_Bar(void* p_param){
	/* depend of your application - I need this in order to get a win_handle linked to this control bar */
	struct_window *this_win = (struct_window*)p_param;
	if( this_win->wi_to_display_mfdb->fd_addr != NULL ){
		this_win->wi_control_bar = (struct_st_control_bar*)mem_alloc(sizeof(struct_st_control_bar));
		this_win->wi_control_bar->control_bar_list = (struct_st_ico_png_list*)mem_alloc(sizeof(control_bar_winimage_list));
		memcpy(this_win->wi_control_bar->control_bar_list, &control_bar_winimage_list, sizeof(control_bar_winimage_list));
		st_Init_Default_Values_Control_Bar(this_win);
	}
}

bool st_Ico_PNG_Init_Image(void){
	if(!st_Ico_PNG_Init(control_bar_winimage_list)){ printf("Error init image control bar\n"); return false; }
	return true;
}

void st_Ico_PNG_Release_Image(void){
	st_Ico_PNG_Release(control_bar_winimage_list);
}

#endif

#ifdef ENABLE_DOCUMENT

void st_Init_WinDoc_Control_Bar(void* p_param){
	/* depend of your application - I need this in order to get a win_handle linked to this control bar */
	struct_window *this_win = (struct_window*)p_param;
	if( this_win->wi_to_display_mfdb->fd_addr != NULL ){
		this_win->wi_control_bar = (struct_st_control_bar*)mem_alloc(sizeof(struct_st_control_bar));
		/* The array of struct you declared below - It contain indexes, path, etc... */
		this_win->wi_control_bar->control_bar_list = (struct_st_ico_png_list*)mem_alloc(sizeof(control_bar_windocument_list));
		memcpy(this_win->wi_control_bar->control_bar_list, &control_bar_windocument_list, sizeof(control_bar_windocument_list));
		st_Init_Default_Values_Control_Bar(this_win);
	}
}

bool st_Ico_PNG_Init_Document(void){
	if(!st_Ico_PNG_Init(control_bar_windocument_list)){ printf("Error init document control bar\n"); return false; }
	return true;
}

void st_Ico_PNG_Release_Document(void){
	st_Ico_PNG_Release(control_bar_windocument_list);
}

void* st_Img_down(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	if(this_win->wi_data->thumbnail_slave == TRUE){
		if(this_win->wi_thumb != NULL){
			int16_t i = this_win->wi_data->img.img_id;
			struct_window* this_win_master = detect_window(this_win->wi_thumb->master_win_handle);
			if(this_win_master != NULL ){

				if(i < this_win_master->wi_thumb->thumbs_nb - 1){
					struct_st_thumbs_list *thumb_ptr = this_win_master->wi_thumb->thumbs_list_array;
					while(thumb_ptr->thumb_id != i){
						thumb_ptr = thumb_ptr->next;
					}					
					this_win->wi_data->img.img_id = NIL;
					/* Disable selected thumnail */
					this_win_master->wi_thumb->thumbs_selected_nb = thumb_ptr->thumb_index;
					this_win_master->wi_thumb->thumbs_area_refresh = TRUE;
					st_Start_Window_Process(this_win_master);
					this_win_master->refresh_win(this_win_master->wi_handle);
					st_End_Window_Process(this_win_master);
					/* Enable new thumbnail */
					this_win->wi_data->img.img_id = thumb_ptr->next->thumb_id;
					this_win->wi_data->img.img_index = thumb_ptr->next->thumb_index;
					this_win_master->wi_thumb->thumbs_selected_nb = thumb_ptr->next->thumb_index;
					this_win_master->wi_thumb->thumbs_area_refresh = TRUE;				
					this_win->wi_data->stop_original_data_load = FALSE;
					this_win->wi_data->fx_on = FALSE;
					this_win->wi_data->remap_displayed_mfdb = TRUE;
					this_win->wi_data->img.scaled_pourcentage = 0;
					this_win->wi_data->img.rotate_degree = 0;
					this_win->current_pos_y = 0;
					this_win->refresh_win(this_win->wi_handle);
					send_message(this_win->wi_handle, WM_REDRAW);
					st_Start_Window_Process(this_win_master);
					this_win_master->refresh_win(this_win_master->wi_handle);
					st_End_Window_Process(this_win_master);
				}
			}
			else{
				if( this_win->wi_data->img.img_id < this_win->wi_data->img.img_total){
					// printf("this_win->wi_data->img.img_id %d this_win->wi_data->img.img_total %d\n", this_win->wi_data->img.img_id,this_win->wi_data->img.img_total );
					this_win->wi_data->img.img_id += 1;
					this_win->wi_data->img.img_index += 1;
					st_Img_Reload(p_param);
				}
			}
		}
	}
	return NULL;
}

void* st_Img_up(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	if(this_win->wi_data->thumbnail_slave == TRUE){
		if(this_win->wi_thumb != NULL){
			int16_t i = this_win->wi_data->img.img_id;
			struct_window* this_win_master = detect_window(this_win->wi_thumb->master_win_handle);
			if(this_win_master != NULL && i > 0){
				struct_st_thumbs_list *thumb_ptr = this_win_master->wi_thumb->thumbs_list_array;
				while(thumb_ptr->thumb_id != i){
					thumb_ptr = thumb_ptr->next;
				}
				this_win->wi_data->img.img_id = NIL;
				/* Disbable selected thumnail */
				this_win_master->wi_thumb->thumbs_selected_nb = thumb_ptr->thumb_index;
				this_win_master->wi_thumb->thumbs_area_refresh = TRUE;
				st_Start_Window_Process(this_win_master);
				this_win_master->refresh_win(this_win_master->wi_handle);
				st_End_Window_Process(this_win_master);
				/* Enable new thumbnail */
                this_win->wi_data->img.img_id = thumb_ptr->prev->thumb_id;
            	this_win->wi_data->img.img_index = thumb_ptr->prev->thumb_index;
            	this_win_master->wi_thumb->thumbs_selected_nb = thumb_ptr->prev->thumb_index;
				this_win_master->wi_thumb->thumbs_area_refresh = TRUE;				
                this_win->wi_data->stop_original_data_load = FALSE;
                this_win->wi_data->fx_on = FALSE;
                this_win->wi_data->remap_displayed_mfdb = TRUE;
                this_win->wi_data->img.scaled_pourcentage = 0;
                this_win->wi_data->img.rotate_degree = 0;
				this_win->current_pos_y = 0;
				this_win->refresh_win(this_win->wi_handle);		
				send_message(this_win->wi_handle, WM_REDRAW);
				st_Start_Window_Process(this_win_master);
				this_win_master->refresh_win(this_win_master->wi_handle);
				st_End_Window_Process(this_win_master);				
			} else {
				if( this_win->wi_data->img.img_id > 0){
					this_win->wi_data->img.img_id -= 1;
					this_win->wi_data->img.img_index -= 1;
					st_Img_Reload(p_param);
				}
			}
		}
	}	
	return NULL;
}

#endif //ENABLE_DOCUMENT

#ifdef ENABLE_VIDEO

void st_Init_WinVideo_Control_Bar(void* p_param){
	/* depend of your application - I need this in order to get a win_handle linked to this control bar */
	struct_window *this_win = (struct_window*)p_param;
	if( this_win->wi_to_display_mfdb->fd_addr != NULL ){
		this_win->wi_control_bar = (struct_st_control_bar*)mem_alloc(sizeof(struct_st_control_bar));
		/* The array of struct you declared below - It contain indexes, path, etc... */
		this_win->wi_control_bar->control_bar_list = (struct_st_ico_png_list*)mem_alloc(sizeof(control_bar_winvideo_list));
		memcpy(this_win->wi_control_bar->control_bar_list, &control_bar_winvideo_list, sizeof(control_bar_winvideo_list));
		st_Init_Default_Values_Control_Bar(this_win);
	}
}

bool st_Ico_PNG_Init_Video(void){
	if(!st_Ico_PNG_Init(control_bar_winvideo_list)){ printf("Error init video control bar\n"); return false; }
	return true;
}

void st_Ico_PNG_Release_Video(void){
	st_Ico_PNG_Release(control_bar_winvideo_list);
}

/* Functions for video */

void* st_Img_Play(void* p_param){
	struct_window *this_win = (struct_window*)p_param;
	this_win->wi_data->play_on = this_win->wi_data->play_on == TRUE ? FALSE : TRUE;
	event_timer_used = this_win->wi_data->play_on == TRUE ? event_timer_video : event_timer_default;
	global_progress_bar->progress_bar_enabled = this_win->wi_data->play_on == TRUE ? FALSE : TRUE;
	// printf("Timer %ld\n", event_timer_used);
	return NULL;
}

#endif //ENABLE_VIDEO

bool st_Ico_PNG_Init_Main(void){
	if(!st_Ico_PNG_Init(control_bar_winstart_list)){ printf("Error init main control bar\n"); return false; }
	return true;
}

void st_Ico_PNG_Release_Main(void){
	st_Ico_PNG_Release(control_bar_winstart_list);
}

#ifdef ENABLE_MAIN_CONTROL_BAR

void st_Init_WinStart_Control_Bar(void* p_param){
	struct_window *this_win = (struct_window*)p_param;
	this_win->wi_control_bar = (struct_st_control_bar*)mem_alloc(sizeof(struct_st_control_bar));
	this_win->wi_control_bar->control_bar_list = (struct_st_ico_png_list*)mem_alloc(sizeof(control_bar_winstart_list));
	memcpy(this_win->wi_control_bar->control_bar_list, &control_bar_winstart_list, sizeof(control_bar_winstart_list));
	this_win->wi_control_bar->last_ico_padding_right = 24;
	this_win->wi_control_bar->control_bar_h = CONTROLBAR_H;
	this_win->wi_control_bar->transparency = FALSE;
	this_win->wi_control_bar->background_mfdb = NULL;
	this_win->wi_control_bar->need_to_reload_control_bar = TRUE;
	this_win->wi_control_bar->transparency_color = GREY_COLOR;
	this_win->wi_control_bar->vdi_handle = &st_vdi_handle;
	this_win->wi_control_bar->virtual_screen_mfdb = &screen_mfdb;
	this_win->wi_control_bar->force_unhide = TRUE;
}

void st_Reload_Control_Bar(struct_window *this_win, struct_st_control_bar* control_bar){
	if( this_win->wi_to_display_mfdb->fd_addr != NULL && this_win->wi_control_bar != NULL ){
		if(this_win->wi_control_bar->control_bar_h > 0){
			if( this_win->wi_data->control_bar_media || screen_workstation_bits_per_pixel <= 8 || this_win->work_area.g_w > this_win->total_length_w || this_win->work_area.g_h > this_win->total_length_h || cpu_type < 30){
				if(this_win->work_area.g_w > this_win->total_length_w || this_win->work_area.g_h > this_win->total_length_h){
					control_bar->need_to_reload_control_bar = true;
				}else{
					control_bar->need_to_reload_control_bar = control_bar->st_control_bar_mfdb.fd_w == wrez ? control_bar->need_to_reload_control_bar : true;
				}		// control_bar->need_to_reload_control_bar = control_bar->st_control_bar_mfdb.fd_w == wrez ? control_bar->need_to_reload_control_bar : true;		
				st_Control_Bar_Refresh_Classic(control_bar, wrez, screen_workstation_bits_per_pixel);
			} else {
				st_Control_Bar_Refresh_MFDB(control_bar, this_win->wi_to_display_mfdb, this_win->current_pos_x, this_win->current_pos_y, this_win->work_area.g_w, this_win->work_area.g_h);
			}
			if(msg_buffer[0] != WM_VSLID && msg_buffer[0] != WM_HSLID){
				st_Control_Bar_Buffer_to_Screen(control_bar, &control_bar->rect_control_bar);
			}
		}
	}
}

#endif //ENABLE_MAIN_CONTROL_BAR

/* Custom functions associated to our icons */

void* st_Img_Open(void* param){
	char final_path[128] = {'\0'};
	char filename[128];
	
	if(file_selector(final_path, (char*)"Open new image", filename) != FALSE){
		new_win_img(final_path);
	}

	return NULL;
}

void* st_Img_Crop(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	this_win->wi_data->crop_requested = true;
	wind_update(BEG_MCTRL);
	graf_mouse(THIN_CROSS,0L);
	return NULL;
}

void* st_Img_Close_All(void* param){
	struct_window*	this_win = (struct_window*)param;	
    sprintf(alert_message,"Exit now?\n");
	if(st_form_alert_choice(FORM_STOP, alert_message, (char*)"No", (char*)"Yes") == 1){
		return NULL;
	}else{
		st_Win_Close_All();
		return NULL;
	}
}

void* st_Img_Rotate(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	this_win->wi_data->fx_requested = TRUE;
	this_win->wi_data->img.rotate_degree = this_win->wi_data->img.rotate_degree <= 180 ? this_win->wi_data->img.rotate_degree + 90 : 0;
	this_win->refresh_win(this_win->wi_handle);
	send_message(this_win->wi_handle, WM_REDRAW);	
	return NULL;
}

void* st_Img_ZoomIn(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	this_win->wi_data->autoscale = FALSE;
	this_win->wi_data->fx_requested = TRUE;
	this_win->wi_data->img.scaled_pourcentage = this_win->wi_data->img.scaled_pourcentage < 100 ? this_win->wi_data->img.scaled_pourcentage + 10 : 100;	
	if(this_win->wi_data->resized){
		this_win->wi_data->img.scaled_width = (this_win->wi_data->img.export_width * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
		this_win->wi_data->img.scaled_height = (this_win->wi_data->img.export_height * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
	}
	else{
			this_win->wi_data->img.scaled_pourcentage = +10;
			this_win->wi_data->img.scaled_width = (this_win->total_length_w * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
			this_win->wi_data->img.scaled_height = (this_win->total_length_h * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
	}
	
	this_win->refresh_win(this_win->wi_handle);
	send_message(this_win->wi_handle, WM_REDRAW);
	return NULL;
}

void* st_Img_ZoomOut(void* p_param){
	struct_window*	this_win = (struct_window*)p_param;
	/* Blocked to a resonable resolution */
	if((this_win->total_length_w > MIN_WINDOWS_WSIZE && this_win->total_length_h > MIN_WINDOWS_HSIZE)){
	this_win->wi_data->fx_requested = TRUE;	
	this_win->wi_data->autoscale = FALSE;	
	this_win->wi_data->img.scaled_pourcentage = this_win->wi_data->img.scaled_pourcentage > -90 ? this_win->wi_data->img.scaled_pourcentage - 10 : -90;
		if(this_win->wi_data->resized){
			this_win->wi_data->img.scaled_width = (this_win->wi_data->img.export_width * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
			this_win->wi_data->img.scaled_height = (this_win->wi_data->img.export_height * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
		}
		else{
			this_win->wi_data->img.scaled_pourcentage = -10;
			this_win->wi_data->img.scaled_width = (this_win->total_length_w * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
			this_win->wi_data->img.scaled_height = (this_win->total_length_h * (this_win->wi_data->img.scaled_pourcentage + 100)) / 100;
		}
		
		this_win->refresh_win(this_win->wi_handle);
		
		send_message(this_win->wi_handle, WM_REDRAW);
	}
	return NULL;
}

void* st_Img_Resize(void* p_param){
	struct_window*	this_win_master = (struct_window*)p_param;
	return NULL;
}

void* st_Img_Export_to_PNG(void* p_param){

	struct_window*	this_win_master = (struct_window*)p_param;
	struct_export this_export = {'\0'};
	boolean original_extension_removed = FALSE;

	int16_t i, j, k;

    char default_extension[5] = ".png";
    char file[64] = {'\0'}; 
	char final_path[256] = {'\0'};
    char ext[5] = {'\0'};

	st_clear_char_array(this_export.export_path);
	st_clear_char_array(this_export.export_comments);
	st_clear_char_array(this_export.export_dir);
	st_clear_char_array(this_export.file_without_ext);
	st_clear_char_array(file);
	st_clear_char_array(final_path);
	strcpy(file, basename(this_win_master->wi_name));
	/* Get the filename without extention */
	for(i = strlen(file); i > 0; i--){
		if(file[i] == '.'){
			strcpy(&file[i], default_extension);
			original_extension_removed = TRUE;
			break;
		}
	}
	if(original_extension_removed == FALSE){
		strcat(file, default_extension);
	}
	

	file_selector(final_path, (char*)"Select a destination path", file);

	j = strlen(final_path);
	k = strlen(this_win_master->wi_data->path);

	/* if we got a directory for destination we take the original filename */
	if(st_DirectoryExists(final_path)){
		strncpy(this_export.export_dir, final_path, j);
		for(i = strlen(this_win_master->wi_data->path); i > 0; i--){
			if(this_win_master->wi_data->path[i] == '\\' || this_win_master->wi_data->path[i] == '/'){
				strncpy(file, &this_win_master->wi_data->path[i + 1], k - i);
				break;
			}
		}
	} else {
		/* Get dir & basename */
		for(i = strlen(file); i > 0; i--){
			if(file[i] == '\\' || file[i] == '/'){
				strncpy(file, (const char*)&final_path[ j - i ], i);
				break;
			}
		}
		strncpy(&this_export.export_dir[0], &final_path[0], j - strlen(file));
	}
	/* Get the filename without extention */
	for(i = strlen(file); i > 0; i--){
		if(file[i] == '.' || file[i] == '/' || file[i] == '\\'){
			strncpy(this_export.file_without_ext, file, i);
			if(file[i] == '.'){
				st_clear_char_array(this_export.export_extension);
				strncpy(this_export.export_extension, &file[i], strlen(file));
			}
			break;
		}
	}

	sprintf(this_export.export_path, "%s%s", this_export.export_dir, this_export.file_without_ext );

	if(this_export.export_extension[0] == '\0'){
		strcat(this_export.export_path, default_extension);
	} else {
		strcat(this_export.export_path, this_export.export_extension);
	}
	this_export.export_width = this_win_master->wi_to_work_in_mfdb->fd_w;
	this_export.export_height = this_win_master->wi_to_work_in_mfdb->fd_h;
	this_export.export_data = (u_int8_t*)this_win_master->wi_to_work_in_mfdb->fd_addr;
	this_export.export_components = this_win_master->wi_to_work_in_mfdb->fd_nplanes >> 3;
	if(strcasecmp(this_export.export_extension, ".png") != 0){
		sprintf(alert_message, "Only png extension supported");
		st_form_alert(FORM_EXCLAM, alert_message);
		return NULL;
	}
	printf("this_export.export_width %d this_export.export_height %d\n", this_export.export_width, this_export.export_height);
    int16_t bits_per_component = 8;
    int16_t png_color_type = PNG_COLOR_TYPE_RGB_ALPHA; 
	int16_t png_color_filter = PNG_TRANSFORM_SWAP_ALPHA;
	int16_t nb_components = this_export.export_components;
	int16_t channel = nb_components;
    if(st_FileExistsAccess(this_export.export_path) == 1){
        sprintf(alert_message,"File exist\nDo you want to erase it?");
        if(st_form_alert_choice(FORM_STOP, alert_message, (char*)"No", (char*)"Yes") == 1){
            return NULL;
        }
    }    
    st_Save_PNG(this_export.export_path, this_export.export_width, this_export.export_height, bits_per_component, png_color_type, this_export.export_data, ( channel * MFDB_STRIDE(this_export.export_width) ), png_color_filter);

    form_alert(1, "[1][Export PNG done][Okay]");

	return NULL;
}

void* st_Img_Reload(void* param){
	struct_window *this_win = (struct_window*)param;
	this_win->wi_data->stop_original_data_load = FALSE;
	this_win->wi_data->fx_on = FALSE;
	this_win->wi_data->remap_displayed_mfdb = TRUE;
    this_win->wi_data->img.scaled_pourcentage = 0;
    this_win->wi_data->img.rotate_degree = 0;	
	this_win->refresh_win(this_win->wi_handle);
	send_message(this_win->wi_handle, WM_REDRAW);
	return NULL;
}

void* st_Img_Autoscale(void* param){
	struct_window *this_win = (struct_window*)param;
	this_win->wi_data->autoscale = this_win->wi_data->autoscale == TRUE ? FALSE : TRUE;
	this_win->wi_data->remap_displayed_mfdb = TRUE;
	this_win->wi_data->img.scaled_pourcentage = 0;
	this_win->refresh_win(this_win->wi_handle);
	send_message(this_win->wi_handle, WM_REDRAW);
	return NULL;
}

/* End for declarations and definitions for functions associated to our icons */
