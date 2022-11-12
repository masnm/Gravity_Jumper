#include "../include/enums.h"

home_options home_options_next ( home_options ho )
{
	ho += 1;
	return ho%5;
}

home_options home_options_previous ( home_options ho )
{
	if ( ho == 0 ) ho = 4;
	else ho -= 1;
	return ho;
}

game_difficulty game_difficulty_next ( game_difficulty gd )
{
	gd += 1;
	return gd%3;
}

game_difficulty game_difficulty_previous ( game_difficulty gd )
{
	if ( gd == 0 ) gd = 2;
	else gd -= 1;
	return gd;
}

pause_options pause_options_next ( pause_options po )
{
	return po^1;
}

pause_options pause_options_previous ( pause_options po )
{
	return po^1;
}
