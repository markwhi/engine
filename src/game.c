#include <linmath.h>

#include "game.h"
#include "audio.h"
#include "debug.h"
#include "render.h"
#include "input.h"

#include <time.h>

static a_buf buffer;

static r_shader shader;
static r_sheet sheet;
static r_anim anim;

static int dir_x[16];
static int dir_y[16];

int g_init(){
	sheet  = r_get_sheet("res/tex/test_sheet.png", 16, 16);
	shader = r_get_shader("res/shd/main.v", "res/shd/main.f");
	r_map_shader(shader, "default");
	unsigned int frames[5] = { 4, 5, 6, 7, 4};
	unsigned int frames2[5] = { 0, 1, 2, 3, 1};

	time_t t;
	srand((unsigned) time(&t));

	anim = r_get_anim(sheet, frames, 5, 24);
	r_anim anim2 = r_get_anim(sheet, frames2, 5, 24);

	r_cache_anim(anim, "test");
	r_cache_anim(anim2, "test2");

	r_anim* _anim = r_get_anim_n("test");

	buffer = a_create_buffer("res/snd/test.ogg");

	for(int i=0;i<16;++i){
		dir_x[i] = (rand() % 3) - 1;
		dir_y[1] = (rand() % 3) - 1;
	}

	float offset_x = 100.f;
	float offset_y = 100.f;
	for(int i=0;i<16;++i){
		int x = i % 4;
		int y = i / 4;
		r_drawable* d = r_get_drawable(_anim, shader, (vec2){16.f, 16.f}, (vec2){(32.f * x) + offset_x, (32.f * y) + offset_y});
		if(!dir_x[i]){
			d->flip_x = 1;	
		}

		if(dir_y[i]){
			d->flip_y = 1;
		}
		r_anim_p(&d->anim);
	}

	return 1;	
}

void g_exit(){

}

void g_input(long delta){
	if(i_key_clicked('P')){
		a_play_sfx(&buffer, 1.f, (vec2){0.f, 0.f}); 
	}

	if(i_key_clicked('R')){
		int r = (rand() % 15) + 1;//non-zero uid
		r_drawable* d = r_get_drawablei(r);
		r_anim* anim = r_get_anim_n("test2");
		r_drawable_set_anim(d, anim);
	}

	float change_x = 0.f; 
	float change_y = 0.f;
	if(i_key_down('D')){
		change_x += delta;
	}else if(i_key_down('A')){
		change_x -= delta;
	}

	if(i_key_down('W')){
		change_y += delta;
	}else if(i_key_down('S')){
		change_y -= delta;
	}

	if(change_x != 0 || change_y != 0){
		r_move_cam(change_x, change_y);
	}

	if(i_key_clicked(GLFW_KEY_ESCAPE)){
		r_request_close();	
	}
}

void g_update(long delta){
	a_update(delta);
	r_update(delta);
	r_update_batch(shader, &sheet);
}

void g_render(long delta){
	for(int i=1;i<17;++i){
		r_drawable* d = r_get_drawablei(i);
		d->position[0] += dir_x[i] * delta * 0.05f;
		d->position[1] += dir_y[i] * delta * 0.05f;
		d->change = 1;		
	}

	r_draw_call(shader, &sheet);
}
