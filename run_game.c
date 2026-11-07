#include <stdlib.h>

#include "dungeon.h"
#include "player.h"
#include "render.h"
#include "io.h"

const char *test_map =
   "#######   ##########"
    "#.....#   #........#"
    "#.....#   #........#"
    "#.....#####........#"
    "#..................#"
    "#.........###......#"
    "#........#   #.....#"
    "#........#   #.....#"
    "#........#   #.....#"
    "##########   #######";

int main(void)
{	

    bool run_dungeon_loop = true;

    start_ncurses();
    create_screens();

    Dungeon d;
    d.num_floors = 1;
    d.curr_floor = 0;
    d.floors[0] = floor_from_text(test_map, 20, 20);
    render_map(&d);

    Coordinate start_coors = {
        .y = 3,
        .x = 3
    };

    Player p = intialize_player(start_coors);
    Camera c = initalize_win_camera(start_coors);
   
    initialize_character_placement(&p); 

	while(run_dungeon_loop) {
        reset_debug_row();
        
        int ch = get_play_input();
        print_debug("Input String: %d", ch);

        if (ch == 'q') {
            run_dungeon_loop = false;
            break;
        }

        Coordinate prev_coors = {
            .x = p.coors.x,
            .y = p.coors.y
        };
        D_Tile prev_tile = get_tile(d.floors[d.curr_floor], prev_coors);
        print_debug("prev_cors (x, y): (%zu, %zu)", prev_coors.x,  prev_coors.y);
        print_debug("tile sprite: %c", prev_tile.sprite);

        Coordinate new_coors = get_new_coors_from_input(prev_coors, ch);
        D_Tile new_tile = get_tile(d.floors[d.curr_floor], new_coors);

        if (!new_tile.is_blocker) {
            update_character_position(&p, new_coors);
            print_debug("new character postion (x, y): (%zu, %zu)", p.coors.x, p.coors.y);

            render_move_character(&p, prev_coors, prev_tile);  
        }
        
	};

    end_ncurses();

	return 0;
}

