#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <GL/freeglut.h>
#include "../include/colour.h"
#include "../include/drawing_routines.h"
#include "../include/keyboard.h"
#include "../include/linear_algebra.h"
#include "../include/texture.h"
#include "../include/level_generator.h"
#include "../include/enums.h"

static vec2d screen = { 1152, 648 };
static vec2l mouse;
static float elapsed_time, frame_time, old_time;
vec2i level_size, tile_size, visible_tile, map_palyer_pos;
char* level_data;
vec2f camera_pos, offset, player_pos, new_player_pos, player_vel, tile_offset;
bool player_on_ground = false, player_touch_wall = false;
float touch_wall_bef;
direction player_facing = right;
float index_change_time = 0.0f;
int index_col = 0, index_row = 0;
int num = 0, shif = 0;
int home_screen_spr;
int start_paze_sprs[7] = { 4, 5, 6, 7, 8, 9, 10 };
int paused_spr_ind = 11;
game_state gstate = welcome_screen;
int player_score, player_score_len;
game_difficulty difficulty = easy;
home_options home_op_sel = home_start;
pause_options pause_op_sel = pause_resume;
themes selected_theme;
float theme_time_elapsed, theme_time_limit = 10.0f;
int key_coin_spr_ind = 12, key_col = 0, coin_col = 0;
int total_coins = 0, collected_coins = 0, total_keys = 0,
	collected_keys = 0, collected_minecraft = 0, total_future = 0,
	collected_future = 0;
int level_no = 0, game_tutorial_spr;
int selected_back, selected_tile, minecraft_tile, minecrft_back,
	normal_tile, normal_back, future_tile, future_back;
int game_end_screen_spr;
float game_elapsed_time;
direction collition_dir;

char get_tile ( int x, int y )
{
	if ( x >= 0 && x < level_size.x && y >= 0 && y < level_size.y )
		return level_data[y * level_size.x + x];
	else return ' ';
}

void set_tile ( int x, int y, char val )
{
	if ( x >= 0 && x < level_size.x && y >= 0 && y < level_size.y )
		level_data[y * level_size.x + x] = val;
}

bool tile_is ( int x, int y, char val )
{
	if ( x >= 0 && x < level_size.x && y >= 0 && y < level_size.y
			&& level_data[y * level_size.x + x] == val )
		return true;
	return false;
}

void draw_string ( const unsigned char* str, int _r, int _c )
{
	glRasterPos2d ( _r, _c );
	glutBitmapString ( GLUT_BITMAP_HELVETICA_18, str );
}

static void game_paued ()
{
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			home_screen_spr,
			0.0f, 0.0f,
			1.0f, 1.0f );
	vec2d mid_scr = div_vec2d_scl ( screen, 2.0 );
	mid_scr.y = 150.0;
	textured_rect_partial (
			mid_scr.x - 96, mid_scr.y,
			192, 32,
			paused_spr_ind,
			0.0f, 0.0f,
			1.0f, 64.0f/320.0f);
	float mul = 1.0f;
	if ( pause_op_sel == pause_resume ) mul = 2.0f;
	else mul = 1.0f;
	textured_rect_partial (
			mid_scr.x - 96, mid_scr.y + 32,
			192, 32,
			paused_spr_ind,
			0.0f, mul*(64.0f/320.0f),
			1.0f, 64.0f/320.0f );
	if ( pause_op_sel == pause_exit ) mul = 4.0f;
	else mul = 3.0f;
	textured_rect_partial (
			mid_scr.x - 96, mid_scr.y + 64,
			192, 32,
			paused_spr_ind,
			0.0f, mul*(64.0f/320.0f),
			1.0f, 64.0f/320.0f );
	if ( get_key ( (unsigned char)13 ) ) {
		if ( pause_op_sel == pause_resume ) {
			gstate = game_loop;
			pause_op_sel = pause_resume;
		} else if ( pause_op_sel == pause_exit ) {
			free ( level_data );
			level_data = NULL;
			gstate = welcome_screen;
		}
	}
	if ( get_key ( (unsigned char)27 ) ) {
		gstate = game_loop;
		pause_op_sel = pause_resume;
	}
	if ( get_arrow ( arrow_down ) ) {
		pause_op_sel = pause_options_next ( pause_op_sel );
	}
	if ( get_arrow ( arrow_up ) ) {
		pause_op_sel = pause_options_previous ( pause_op_sel );
	}
}

void calculate_score ()
{
	player_score = collected_coins * 101;
	int time = game_elapsed_time/10;
	if ( time == 0 ) time = 1;
	player_score /= time;
	if ( player_score == 0 ) player_score += 1;
	int loc_score = player_score;
	player_score_len = 0;
	while ( loc_score ) {
		++player_score_len;
		loc_score /= 10;
	}
}

void show_total_map ()
{
	vec2i cur_visible_tile = level_size;
	vec2f cur_offset = make_vec2f ( 0, 0 );
	vec2i cur_tile_size = make_vec2i ( screen.x / (double)cur_visible_tile.x,
			screen.y / (double)cur_visible_tile.y );
	vec2f cur_tile_offset = make_vec2f ( 0, 0 );
	for ( int x = 0 ; x < cur_visible_tile.x ; ++x ) {
		for ( int y = 0 ; y < cur_visible_tile.y ; ++y ) {
			char c = get_tile ( x + cur_offset.x, y + cur_offset.y );
			switch ( c ) {
				case '#':
					num = 0; shif = 0;
					for ( int _i = -1 ; _i < 2 ; ++_i ) {
						for ( int _j = -1 ; _j < 2 ; ++_j ) {
							if ( _i == _j && _i == 0 && _j == 0 ) continue;
							++shif;
							if ( tile_is ( _i + x + cur_offset.x, _j + y + cur_offset.y, '#' ) ) {
								num |= (1 << shif);
							}
						}
					}
					num |= 330;
					num ^= 330;
					int _x = num / 16, _y = num % 16;
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							selected_tile,
							_y*(64.0/1024.0), _x*(64.0/1024.0),
							64.0/1024.0, 64.0/1024.0 );
					break;
				case 'C':
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							key_coin_spr_ind,
							coin_col*0.1f, 0.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'K':
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							key_coin_spr_ind,
							key_col*0.1f, 64.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'M':
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							key_coin_spr_ind,
							0.0f, 192.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'F':
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							key_coin_spr_ind,
							0.1f, 192.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'D':
					textured_rect_partial (
							x*cur_tile_size.x-cur_tile_offset.x,
							y*cur_tile_size.y-cur_tile_offset.y,
							cur_tile_size.x, cur_tile_size.y,
							key_coin_spr_ind,
							collected_keys==total_keys?0.1f:0.0f, 128.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				default:
					//fprintf ( stderr, "UnknownTile\n" );
					break;
			}
		}
	}
	textured_rect_partial (
			(map_palyer_pos.x-cur_offset.x)*cur_tile_size.x,
			(map_palyer_pos.y-cur_offset.y)*cur_tile_size.y,
			cur_tile_size.x, cur_tile_size.y,
			0, index_col*(64.0/384.0), index_row*(64.0/512.0), 64.0/384.0, 64.0/512.0 );
	if ( get_key ( (unsigned char)'t' ) ) {
		gstate = game_loop;
	}
}

static void main_game_loop ()
{
	touch_wall_bef += frame_time;
	// change to minecraft theme
	if ( get_key ( (unsigned char)'m' ) ) {
		if ( selected_theme == theme_normal && collected_minecraft > 0 ) {
			collected_minecraft -= 1;
			selected_back = minecrft_back;
			selected_tile = minecraft_tile;
			selected_theme = theme_minecraft;
		}
	}
	if ( get_key ( (unsigned char)'f' ) ) {
		if ( selected_theme == theme_normal && collected_future > 0 ) {
			collected_future -= 1;
			selected_back = future_back;
			selected_tile = future_tile;
			selected_theme = theme_future;
		}
	}
	if ( selected_theme != theme_normal ) {
		theme_time_elapsed += frame_time;
		if ( theme_time_elapsed > theme_time_limit ) {
			theme_time_elapsed = 0.0f;
			selected_theme = theme_normal;
			selected_back = normal_back;
			selected_tile = normal_tile;
		}
	}
	// game timing
	if ( selected_theme == theme_normal )
		game_elapsed_time += frame_time;
	else if ( selected_theme == theme_minecraft )
		game_elapsed_time += 0.3 * frame_time;
	// loop start
	if ( get_key ( (unsigned char)'a' ) ) {
		player_vel.x -= 200.0f * frame_time;
		player_facing = left;
	}
	if ( get_key ( (unsigned char)'d' ) ) {
		player_vel.x += 200.0f * frame_time;
		player_facing = right;
	}
	if ( get_key ( (unsigned char)'j' ) ) {
		if ( player_vel.y == 0 ||
				(collition_dir != player_facing && player_touch_wall && touch_wall_bef < 1.0f) ) {
			player_vel.y -= 400.0 * frame_time;
			touch_wall_bef = 0;
			player_touch_wall = false;
		}
	}
	player_vel.y += 10.0 * frame_time;
	if ( player_on_ground ) {
		player_vel.x += -5.0 * player_vel.x * frame_time;
		if ( fabs ( player_vel.x ) < 0.01f )
			player_vel.x = 0;
	}
	if ( player_vel.x < 0.1 && player_vel.x > -0.1 ) player_vel.x = 0.0f;
	if ( player_vel.x > 5.0f ) player_vel.x = 5.0f;
	if ( player_vel.x < -5.0f ) player_vel.x = -5.0f;
	if ( player_vel.y > 7.0f ) player_vel.y = 7.0f;
	if ( player_vel.y < -7.0f ) player_vel.y = -7.0f;
	new_player_pos.x = player_pos.x + player_vel.x * frame_time;
	new_player_pos.y = player_pos.y + player_vel.y * frame_time;
	// pickup collection
	// coin pickup
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'C' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f, '.' );
		collected_coins += 1;
	}
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'C' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f, '.' );
		collected_coins += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'C' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f, '.' );
		collected_coins += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'C' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f, '.' );
		collected_coins += 1;
	}
	// key pickup
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'K' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f, '.' );
		collected_keys += 1;
	}
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'K' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f, '.' );
		collected_keys += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'K' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f, '.' );
		collected_keys += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'K' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f, '.' );
		collected_keys += 1;
	}
	// minecraft pickup
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f, '.' );
		collected_minecraft += 1;
	}
	// future pickup
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'F' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f, '.' );
		collected_future += 1;
	}
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'F' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f, '.' );
		collected_future += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'F' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f, '.' );
		collected_future += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'F' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f, '.' );
		collected_future += 1;
	}
	// minecraft pickup
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f, '.' );
		collected_minecraft += 1;
	}
	if ( get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'M' ) {
		set_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f, '.' );
		collected_minecraft += 1;
	}
	// door endgame
	if ( get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 0.0f ) == 'D'
			|| get_tile ( new_player_pos.x + 0.0f, new_player_pos.y + 1.0f ) == 'D'
			|| get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 0.0f ) == 'D'
			|| get_tile ( new_player_pos.x + 1.0f, new_player_pos.y + 1.0f ) == 'D' ) {
		if ( collected_keys == total_keys ) {
			calculate_score ();
			gstate = end_game;
		}
	}

	// minecraft block remove
	if ( selected_theme == theme_minecraft && get_key ( (unsigned char)' ' ) ) {
		if ( player_facing == left ) {
			if ( player_pos.x - 1 > 0 && get_tile ( player_pos.x - 1, player_pos.y ) == '#' )
				set_tile ( player_pos.x - 1, player_pos.y, '.' );
		} else if ( player_facing == right ) {
			if ( player_pos.x + 1 < level_size.x - 1 && get_tile ( player_pos.x + 1, player_pos.y ) == '#' )
				set_tile ( player_pos.x + 1, player_pos.y, '.' );
		}
	}
	// check for colletion
	if ( touch_wall_bef > 1.0f ) player_touch_wall = false;
	if ( player_vel.x <= 0) {
		if ( get_tile(new_player_pos.x + 0.0f, player_pos.y + 0.0f) != '.'
				|| get_tile(new_player_pos.x + 0.0f, player_pos.y + 0.9f) != '.' ) {
			new_player_pos.x = (int)new_player_pos.x + 1;
			player_vel.x = 0;
			if ( !player_on_ground ) {
				player_touch_wall = true;
				collition_dir = left;
				touch_wall_bef = 0;
			}
		}
	} else {
		if ( get_tile(new_player_pos.x + 1.0f, player_pos.y + 0.0f) != '.'
				|| get_tile(new_player_pos.x + 1.0f, player_pos.y + 0.9f) != '.' ) {
			new_player_pos.x = (int)new_player_pos.x;
			player_vel.x = 0;
			if ( !player_on_ground ) {
				collition_dir = right;
				player_touch_wall = true;
				touch_wall_bef = 0;
			}
		}
	}

	player_on_ground = false;
	if (player_vel.y <= 0) {
		if ( get_tile(new_player_pos.x + 0.0f, new_player_pos.y) != '.'
				|| get_tile(new_player_pos.x + 0.9f, new_player_pos.y) != '.') {
			new_player_pos.y = (int)new_player_pos.y + 1;
			player_vel.y = 0;
		}
	} else {
		if ( get_tile(new_player_pos.x + 0.0f, new_player_pos.y + 1.0f) != '.'
				|| get_tile(new_player_pos.x + 0.9f, new_player_pos.y + 1.0f) != '.') {
			new_player_pos.y = (int)new_player_pos.y;
			player_vel.y = 0;
			player_on_ground = true;
		}
	}
	player_pos = new_player_pos;
	// cpe
	camera_pos = player_pos;
	visible_tile.x = (screen.x / tile_size.x) + 1;
	visible_tile.y = (screen.y / tile_size.y) + 1;
	offset.x = camera_pos.x - visible_tile.x/2.0f;
	offset.y = camera_pos.y - visible_tile.y/2.0f;
	if ( offset.x < 0 ) offset.x = 0;
	if ( offset.y < 0 ) offset.y = 0;
	if ( offset.x >= level_size.x - visible_tile.x + 1 ) offset.x = level_size.x - visible_tile.x + 1;
	if ( offset.y >= level_size.y - visible_tile.y + 1 ) offset.y = level_size.y - visible_tile.y + 1;
	tile_offset.x = (offset.x - (int)offset.x) * tile_size.x;
	tile_offset.y = (offset.y - (int)offset.y) * tile_size.y;
	// drawing back
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			selected_back,
			((double)(offset.x*visible_tile.x)/(double)(level_size.x*level_size.x)),
			((double)(offset.y*visible_tile.y)/(double)(level_size.y*level_size.y)),
			((double)visible_tile.x/(double)level_size.x),
			((double)visible_tile.y/(double)level_size.y)
			);
	fill_rect ( 0, 0, screen.x, screen.y, c_black_75p );
	// end drawing back
	glColor4fv ( c_white );

	for ( int x = -1 ; x <= visible_tile.x ; ++x ) {
		for ( int y = -1 ; y <= visible_tile.y ; ++y ) {
			char c = get_tile ( x + offset.x, y + offset.y );
			switch ( c ) {
				case '#':
					num = 0; shif = 0;
					for ( int _i = -1 ; _i < 2 ; ++_i ) {
						for ( int _j = -1 ; _j < 2 ; ++_j ) {
							if ( _i == _j && _i == 0 && _j == 0 ) continue;
							++shif;
							if ( tile_is ( _i + x + offset.x, _j + y + offset.y, '#' ) ) {
								num |= (1 << shif);
							}
						}
					}
					num |= 330;
					num ^= 330;
					int _x = num / 16, _y = num % 16;
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							selected_tile,
							_y*(64.0/1024.0), _x*(64.0/1024.0),
							64.0/1024.0, 64.0/1024.0 );
					break;
				case 'C':
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							key_coin_spr_ind,
							coin_col*0.1f, 0.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'K':
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							key_coin_spr_ind,
							key_col*0.1f, 64.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'M':
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							key_coin_spr_ind,
							0.0f, 192.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'F':
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							key_coin_spr_ind,
							0.1f, 192.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				case 'D':
					textured_rect_partial (
							x*tile_size.x-tile_offset.x,
							y*tile_size.y-tile_offset.y,
							tile_size.x, tile_size.y,
							key_coin_spr_ind,
							collected_keys==total_keys?0.1f:0.0f, 128.0f/256.0f,
							0.1f, 64.0f/256.0f);
					break;
				default:
					//fprintf ( stderr, "UnknownTile\n" );
					break;
			}
		}
	}

	index_change_time += frame_time;
	if ( index_change_time > 0.2f ) {
		index_change_time -= 0.2f;
		++index_col;
		index_col %= 6;
		key_col += 1;
		key_col %= 9;
		coin_col += 1;
		coin_col %= 8;
	}
	index_row = player_facing;
	if ( player_vel.y > 0.0001f )
		index_row = player_facing + 6;
	else if ( player_vel.y < -0.0001f )
		index_row = player_facing + 2;
	else if ( player_vel.x > 0.0001f || player_vel.x < -0.0001f )
		index_row = player_facing + 4;
	glColor4fv ( c_white );
	textured_rect_partial (
			(player_pos.x-offset.x)*tile_size.x,
			(player_pos.y-offset.y)*tile_size.y,
			tile_size.x, tile_size.y,
			0, index_col*(64.0/384.0), index_row*(64.0/512.0), 64.0/384.0, 64.0/512.0 );
	static char c[50];
	if ( collected_coins == total_coins ) glColor4fv ( c_green );
	else glColor4fv ( c_white );
	sprintf(c, "Coins : %d / %d", collected_coins, total_coins );
	draw_string ( (const unsigned char*)c, 10, 20 );
	if ( collected_keys == total_keys ) glColor4fv ( c_green );
	else glColor4fv ( c_white );
	sprintf(c, "Keys : %d / %d", collected_keys, total_keys );
	draw_string ( (const unsigned char*)c, 10, 40 );
	glColor4fv ( c_white );
	sprintf(c, "Minecraft : %d", collected_minecraft );
	draw_string ( (const unsigned char*)c, 10, 80 );
	sprintf(c, "Future : %d", collected_future );
	draw_string ( (const unsigned char*)c, 10, 60 );
	sprintf(c, "Time : %d", (int)game_elapsed_time );
		draw_string ( (const unsigned char*)c, screen.x-150, 20 );
	if ( selected_theme != theme_normal ) {
		sprintf(c, "Remaining : %d", (int)(theme_time_limit-theme_time_elapsed) );
		draw_string ( (const unsigned char*)c, screen.x-150, 40 );
	}
	glColor4fv ( c_white );
	// loop end
	if ( get_key ( (unsigned char)27 ) ) {
		gstate = game_pause;
	}
	if ( get_key ( (unsigned char)'t' ) ) {
		gstate = game_show_map;
		map_palyer_pos = make_vec2i ( player_pos.x, player_pos.y );
	}
}

void generate_level_and_more ()
{
	tile_size.x = 64; tile_size.y = 64;
	player_pos.x = 1.0f; player_pos.y = 1.0f;
	generate_level ( "resources/level_1.txt", level_no );
	FILE* fp = fopen ( "resources/level_1.txt", "r" );
	fscanf ( fp, "%d %d ", &level_size.x, &level_size.y );
	level_data = (char*) malloc ( sizeof(char) * level_size.x * level_size.y );
	for ( int i = 0 ; i < level_size.x ; ++i ) {
		for ( int j = 0 ; j < level_size.y ; ++j ) {
			fscanf ( fp, "%c ", &level_data[i * level_size.x + j] );
		}
	}
	fclose ( fp );
	make_difficulty ( level_data, level_size, level_no, difficulty );
	total_coins = (level_no + 1) * 8;
	total_keys = (difficulty + 1) * 4;
	collected_coins = 0;
	collected_keys = 0;
	collected_future = 0;
	selected_back = normal_back;
	selected_tile = normal_tile;
	theme_time_elapsed = 0.0f;
	selected_theme = theme_normal;
	game_elapsed_time = 0.0f;
}

void main_welcome_screen ()
{
	if ( get_arrow ( arrow_up ) ) {
		home_op_sel = home_options_previous ( home_op_sel );
	}
	if ( get_arrow ( arrow_down ) ) {
		home_op_sel = home_options_next ( home_op_sel );
	}
	if ( get_arrow ( arrow_left ) ) {
		if ( home_op_sel == home_diffuculty ) {
			difficulty = game_difficulty_previous ( difficulty );
		}
		if ( home_op_sel == home_level ) {
			level_no -= 1;
			if ( level_no < 0 ) level_no += 9;
		}
	}
	if ( get_arrow ( arrow_right ) ) {
		if ( home_op_sel == home_diffuculty ) {
			difficulty = game_difficulty_next ( difficulty );
		}
		if ( home_op_sel == home_level ) {
			level_no += 1;
			level_no %= 9;
		}
	}
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			home_screen_spr,
			0.0f, 0.0f, 1.0f, 1.0f );
	glColor4fv ( c_white );
	int _x = screen.x/2, _y = 80;
	//int _x = screen.x / 6, _y = screen.y / 6;
	for ( int i = 0 ; i < 5 ; ++i ) {
		textured_rect_partial (
				_x-160, (i+1)*32+_y,
				320, 32,
				start_paze_sprs[i],
				0.0f, home_op_sel==i?0.5f:0.0f, 1.0f, 0.5f
				);
	}
	// draw difficulty
	_x = (screen.x/2);
	switch ( difficulty ) {
		case easy:
			textured_rect_partial (
					_x+40, 2*32+_y,
					90, 32,
					start_paze_sprs[5],
					0.0f, home_op_sel==home_diffuculty?0.5f:0.0f,
					0.28125f, 0.5f );
			break;
		case medium:
			textured_rect_partial (
					_x+20, 2*32+_y,
					130, 32,
					start_paze_sprs[5],
					0.28125f, home_op_sel==home_diffuculty?0.5f:0.0f,
					0.421875f, 0.5f );
			break;
		case hard:
			textured_rect_partial (
					_x+40, 2*32+_y,
					85, 32,
					start_paze_sprs[5],
					0.734375f, home_op_sel==home_diffuculty?0.5f:0.0f,
					0.265625f, 0.5f );
			break;
		default:
			assert ( false );
			break;
	}
	// draw level
	_x = (screen.x/2);
	textured_rect_partial (
			_x+55, 3*32+_y,
			32, 32,
			start_paze_sprs[6],
			0.1f * level_no, home_op_sel==home_level?0.5f:0.0f,
			0.1f, 0.5f );
	if ( get_key ( (unsigned char)13 ) ) {
		if ( home_op_sel == home_start ) {
			generate_level_and_more ();
			gstate = game_loop;
		}
		if ( home_op_sel == home_exit ) {
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);
			glutLeaveMainLoop ();
		}
		if ( home_op_sel == home_tutorial ) {
			gstate = game_tutorial;
		}
	}
}

static void game_tutorial_fund ()
{
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			home_screen_spr,
			0.0f, 0.0f, 1.0f, 1.0f );
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			game_tutorial_spr,
			0.0f, 0.0f, 1.0f, 1.0f );
	if ( get_key ( (unsigned char)13 ) ) {
		gstate = welcome_screen;
	}
}

static void end_game_func ()
{
	textured_rect_partial (
			0, 0,
			screen.x, screen.y,
			home_screen_spr,
			0.0f, 0.0f, 1.0f, 1.0f );
	glColor4fv ( c_white );
	vec2d mid_scr = div_vec2d_scl ( screen, (double)2.0 );
	mid_scr.y = 150.0f;
	textured_rect_partial (
			mid_scr.x - 160, mid_scr.y,
			320, 32,
			game_end_screen_spr,
			0.0f, 0.0f,
			1.0f, 64.0f/192.0f);
	textured_rect_partial (
			mid_scr.x - 160, mid_scr.y + 32,
			320, 32,
			game_end_screen_spr,
			0.0f, 64.0f/192.0f,
			1.0f, 64.0f/192.0f);
	int score = player_score, cur_num;
	vec2d pos = add_vec2d ( mid_scr,
			make_vec2d ( (double)player_score_len*20.0*0.5, 0) );
	pos.x -= 20.0;
	while ( score ) {
		cur_num = score % 10;
		score /= 10;
		textured_rect_partial (
				pos.x, pos.y + 80,
				32, 32,
				game_end_screen_spr,
				cur_num*0.1f, 128.0f/192.0f,
				0.1f, 64.0f/192.0f );
		pos.x -= 20.0;
	}
	textured_rect_partial (
			mid_scr.x - 96, mid_scr.y + 128,
			192, 32,
			paused_spr_ind,
			0.0f, 4.0f*(64.0f/320.0f),
			1.0f, 64.0f/320.0f );
	if ( get_key ( (unsigned char)13 ) ) {
		gstate = welcome_screen;
	}
}

static void display_func ( void )
{
	glClear(GL_COLOR_BUFFER_BIT);
	//height = glutGet ( GLUT_WINDOW_HEIGHT ); width = glutGet ( GLUT_WINDOW_WIDTH );
	glLoadIdentity();
	gluOrtho2D ( (double)0.0, (double)screen.x, (double)screen.y, (double)0.0 );
	elapsed_time = ((float)glutGet ( GLUT_ELAPSED_TIME ) / 1000.0f );
	frame_time = elapsed_time - old_time;
	old_time = elapsed_time;

	switch ( gstate ) {
		case welcome_screen:
			main_welcome_screen ();
			break;
		case game_loop:
			main_game_loop ();
			break;
		case game_pause:
			game_paued ();
			break;
		case end_game:
			end_game_func ();
			break;
		case game_show_map:
			show_total_map ();
			break;
		case game_tutorial:
			game_tutorial_fund ();
			break;
		default:
			assert ( false && "Invalid Game State" );
		break;
	}

	key_cleanup ();
	glutSwapBuffers();
}

static void idle ( void ) { glutPostRedisplay (); }

void initialize_all ()
{
	create_texture ( 19 );
	texture_from_image ( "./assets/character.png", 0 );
	texture_from_image ( "./assets/minecraf_tile.png", 1 );
	texture_from_image ( "./assets/minecrft_back.jpg", 2 );
	minecraft_tile = 1;
	minecrft_back = 2;
	texture_from_image ( "./assets/COVER.jpg", 3 );
	home_screen_spr = 3;
	texture_from_image ( "./assets/start.png", 4 );
	texture_from_image ( "./assets/difficulty.png", 5 );
	texture_from_image ( "./assets/level.png", 6 );
	texture_from_image ( "./assets/tutorial.png", 7 );
	texture_from_image ( "./assets/exit.png", 8 );
	texture_from_image ( "./assets/difclt_opt.png", 9 );
	texture_from_image ( "./assets/level_opt.png", 10 );
	texture_from_image ( "./assets/paused.png", 11 );
	texture_from_image ( "./assets/door_coin_key.png", 12 );
	texture_from_image ( "./assets/night_tiles.png", 13 );
	texture_from_image ( "./assets/nnn_back.jpg", 14 );
	normal_tile = 13;
	normal_back = 14;
	texture_from_image ( "./assets/future_tile.png", 15 );
	texture_from_image ( "./assets/future_back.jpg", 16 );
	future_tile = 15;
	future_back = 16;
	texture_from_image ( "./assets/game_end_screen.png", 17 );
	game_end_screen_spr = 17;
	texture_from_image ( "./assets/g_tutorial.png", 18 );
	game_tutorial_spr = 18;
}

void cleanup_all ()
{
	if ( level_data != NULL ) free ( level_data );
	destroy_texture ();
}

static void mouse_input ( int button, int state, int x, int y )
{
	if ( button == GLUT_LEFT_BUTTON ) {
		if ( state == GLUT_DOWN ) { }
		if ( state == GLUT_UP ) { }
	}
}

static void mouse_motion_input ( int x, int y )
{
	mouse.x = x; mouse.y = y;
}

int main ( int argc, char* argv[] )
{
	glutInit ( &argc, argv );
	glutInitWindowSize ( screen.x, screen.y );
	glutInitWindowPosition ( 400, 25 );
	glutInitDisplayMode ( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
	glutCreateWindow ( "GLUT Template" );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glClearColor(0.0,0.0,0.0,0.0);
	glutDisplayFunc ( display_func );
	glutIdleFunc ( idle );
	glutMouseFunc ( mouse_input );
	glutKeyboardFunc ( keyboard_input );
	glutPassiveMotionFunc ( mouse_motion_input );
	glutSpecialFunc ( arrow_keys );
	initialize_all ();
	atexit ( cleanup_all );
	glutMainLoop();

	return 0;
}
