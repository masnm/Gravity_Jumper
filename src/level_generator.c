#include "../include/level_generator.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
	int row;
	int col;
} pair ;

int curi = 0;
pair cur[4];
pair start, end;

typedef struct {
	bool vis;
	bool right;
	bool down;
} cell ;

bool inside ( int _r, int _c, size_t sz )
{ return ( _r >= 0 && _c >= 0 && _r < sz && _c < sz ); }

bool generate_level ( char* filename, int level_no )
{
	srand ( level_no );
	const size_t sz = (level_no + 1) * 7;
	cell** ar;
	ar = (cell**) malloc ( sizeof(cell*) * (sz+1) );
	if ( ar == NULL ) return false;
	for ( size_t i = 0 ; i < (sz+1) ; ++i ) {
		ar[i] = (cell*) malloc ( sizeof(cell) * (sz+1) );
		if ( ar[i] == NULL ) return false;
	}

	fprintf ( stderr, "All memory allocated Successfully\n" );

	for ( size_t i = 0 ; i < sz ; ++i )
		for ( size_t j = 0 ; j < sz ; ++j )
			ar[i][j] = (cell) { .vis = false, .right = true, .down = true };

	// generate
	pair* st = (pair*) malloc ( sizeof(pair) * (sz+1) * (sz+1) );
	if ( st == NULL ) {
		fprintf ( stderr, "Failed to get pair array." );
		exit ( 0 );
	}
	pair cp = { 0, 0 };
	ar[0][0].vis = true;
	int stt = 0;
	st[stt++] = cp;
	while ( stt > 0 ) {
		curi = 0;
		cp = st[stt-1];
		for ( int i = -1 ; i < 2 ; i += 2 ) {
			if ( inside ( cp.row + i, cp.col, sz ) &&
					! ar[cp.row + i][cp.col].vis ) {
				cur[curi++] = (pair) { cp.row + i, cp.col };
			}
			if ( inside ( cp.row, cp.col + i, sz ) &&
					! ar[cp.row][cp.col + i].vis ) {
				cur[curi++] = (pair) { cp.row, cp.col + i };
			}
		}
		if ( curi == 0 ) {
			--stt;
		} else {
			int ind = rand() % curi;
			ar[cur[ind].row][cur[ind].col].vis = true;
			st[stt++] = cur[ind];
			start = cp; end = cur[ind];
			if ( start.row == end.row ) {
				if ( start.col > end.col )
					ar[end.row][end.col].right = false;
				else
					ar[start.row][start.col].right = false;
			} else if ( start.col == end.col ) {
				if ( start.row > end.row )
					ar[end.row][end.col].down = false;
				else
					ar[start.row][start.col].down = false;
			} else assert ( false );
		}
	}
	// generate

	FILE* fp = fopen ( filename, "w" );
	if ( fp == NULL ) {
		fprintf ( stderr, "LG FO failed\n" );
		fflush ( stderr );
	}
	fprintf ( fp, "%zu %zu\n", sz*3+1, sz*3+1 );
	for ( size_t i = 0 ; i < sz*3 ; ++i )
		fprintf ( fp, "#" );
	fprintf ( fp, "#\n" );
	for ( size_t i = 0 ; i < sz ; ++i ) {
		fprintf ( fp, "#" );
		for ( size_t j = 0 ; j < sz ; ++j ) {
			fprintf ( fp, "%c%c%c", '.', '.', ar[i][j].right?'#':'.' );
		}
		fprintf ( fp, "\n" );
		fprintf ( fp, "#" );
		for ( size_t j = 0 ; j < sz ; ++j ) {
			fprintf ( fp, "%c%c%c", '.', '.', ar[i][j].right?'#':'.' );
		}
		fprintf ( fp, "\n" );
		fprintf ( fp, "#" );
		for ( size_t j = 0 ; j < sz ; ++j ) {
			if ( ar[i][j].down ) fprintf ( fp, "###" );
			else {
				fprintf ( fp, ".." );
				fprintf ( fp, ar[i][j].right|ar[i+1][j].right|ar[i][j+1].down?"#":"." );
			}
		}
		fprintf ( fp, "\n" );
	}
	fclose ( fp );

	for ( size_t i = 0 ; i < (sz+1) ; ++i ) free ( ar[i] );
	free ( ar );

	return true;
}

vec2i glevel_size;

char in_get_tile ( char* level_data, int x, int y )
{
	if ( x >= 0 && x < glevel_size.x && y >= 0 && y < glevel_size.y )
		return level_data[y * glevel_size.x + x];
	else return ' ';
}

void in_set_tile ( char* level_data, int x, int y, char val )
{
	if ( x >= 0 && x < glevel_size.x && y >= 0 && y < glevel_size.y )
		level_data[y * glevel_size.x + x] = val;
}

int make_difficulty ( char* level_data, vec2i level_size, int level_no, int difficulty )
{
	glevel_size = level_size;
	srand ( level_no );
	int keys = (level_no + 1) * 4 * (difficulty + 1);
	// inserting Door
	in_set_tile ( level_data, level_size.x-2, level_size.y-2, 'D' );
	// inserting keys into map
	for ( int i = 0 ; i < keys ; ++i ) {
		while ( true ) {
			int x = rand() % level_size.x, y = rand() % level_size.y;
			char tile = in_get_tile ( level_data, x, y );
			if ( tile == '.' ) {
				in_set_tile ( level_data, x, y, 'K' );
				break;
			}
		}
	}
	// inserting coins
	int coins = (level_no + 1) * 8 * (difficulty + 1);
	for ( int i = 0 ; i < coins ; ++i ) {
		while ( true ) {
			int x = rand() % level_size.x, y = rand() % level_size.y;
			char tile = in_get_tile ( level_data, x, y );
			if ( tile == '.' ) {
				in_set_tile ( level_data, x, y, 'C' );
				break;
			}
		}
	}
	// inserting minecraft pickup
	int mine_pick = (level_no + 1) * 3;
	for ( int i = 0 ; i < mine_pick ; ++i ) {
		while ( true ) {
			int x = rand() % level_size.x, y = rand() % level_size.y;
			char tile = in_get_tile ( level_data, x, y );
			if ( tile == '.' ) {
				in_set_tile ( level_data, x, y, 'M' );
				break;
			}
		}
	}
	// inserting future pickup
	int future_pick = (level_no + 1) * 3;
	for ( int i = 0 ; i < future_pick ; ++i ) {
		while ( true ) {
			int x = rand() % level_size.x, y = rand() % level_size.y;
			char tile = in_get_tile ( level_data, x, y );
			if ( tile == '.' ) {
				in_set_tile ( level_data, x, y, 'F' );
				break;
			}
		}
	}
	return keys;
}
