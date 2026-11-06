#include <stdlib.h>

#include "dungeon.h"
#include "player.h"
#include "render.h"
#include "io.h"

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

    Coordinate start_coors = {
        .y = 10,
        .x = 10
    };

    Player p = intialize_player(start_coors);
    initialize_character_placement(&p); 

	while(run_dungeon_loop) {
        int ch = get_play_input();

        if (ch == 'q') {
            run_dungeon_loop = false;
            break;
        }

        Coordinate prev_coors = {
            .x = p.coors.x,
            .y = p.coors.y
        };
        D_Tile prev_tile = get_tile(d.floors[d.curr_floor], prev_coors);
        is_character_input(&p, ch);
        render_move_character(&p, prev_coors, prev_tile);


	};

    end_ncurses();

	return 0;
}

