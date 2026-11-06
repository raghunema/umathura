#include <stdlib.h>

#include "dungeon.h"
#include "player.h"
#include "render.h"

const char *test_map = 
    "##########"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "##########"
    ;


int main(void)
{	

    bool run_dungeon_loop = true;

    start_ncurses();
    create_screens();

    Dungeon d;
    d.num_floors = 1;
    d.curr_floor = 0;
    d.floors[0] = floor_from_text(test_map, 10, 10);
    render_map(&d);

    Coordinates start_coors = {
        .y = 10,
        .x = 10
    };

    Player p = intialize_player(start_coors);
    initialize_character_placement(&p); 

	while(run_dungeon_loop) {
        //do things
	}

    end_ncurses();

	return 0;
}

