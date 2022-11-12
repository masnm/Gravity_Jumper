#ifndef __ENUMS_H__
#define __ENUMS_H__

typedef enum {
	right = 0,
	left = 1,
} direction ;

typedef enum {
	welcome_screen = 0,
	game_loop = 1,
	game_pause = 2,
	end_game = 3,
	game_show_map = 4,
	game_tutorial = 5,
} game_state ;

typedef enum {
	easy = 0,
	medium = 1,
	hard = 2,
} game_difficulty ;

typedef enum {
	home_start = 0,
	home_diffuculty = 1,
	home_level = 2,
	home_tutorial = 3,
	home_exit = 4,
} home_options ;

typedef enum {
	pause_resume = 0,
	pause_exit = 1,
} pause_options ;

typedef enum {
	theme_normal = 0,
	theme_minecraft = 1,
	theme_future = 2,
} themes ;

home_options home_options_next ( home_options ho );
home_options home_options_previous ( home_options ho );

game_difficulty game_difficulty_next ( game_difficulty gd );
game_difficulty game_difficulty_previous ( game_difficulty gd );

pause_options pause_options_next ( pause_options po );
pause_options pause_options_previous ( pause_options po );

#endif // __ENUMS_H__
