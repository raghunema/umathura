#include <stdlib.h>

#include "dungeon.h"
#include "player.h"
#include "render.h"
#include "io.h"

const char *test_map =
"#######   ################      ########"
"#.....#   #..............#      #......#"
"#.....#   #...............#     #......#"
"#.....#####................ #   #......#"
"#............................#  #......#"
"#.........###.................###......#"
"#........#   #.........................#"
"#........#   #.........................#"
"#........#   #.........................#"
"#.....###    ##....#######.............#"
"#.....#    ##.....#      #.............#"
"#.....#   #......#       #......########"
"#.....#   #......#       #......#       "
"#.....#####.......#      #......#       "
"#.................#      #......#       "
"#.........###......#     #.......#      "
"#........#   #......######........#     "
"#........#   #.....................#####"
"#........#   #.........................#"
"##########   ###########################";

int main(void)
{	

    bool run_dungeon_loop = true;

    start_ncurses();
    create_screens();

    Dungeon d;
    d.num_floors = 1;
    d.curr_floor = 0;
    d.floors[0] = floor_from_text(test_map, 40, 40);
    render_map(&d);

    Coordinate start_coors = {
        .y = 3,
        .x = 3
    };

    Player p = intialize_player(start_coors);
    Camera c = initalize_win_camera(start_coors);
   
    // initialize_character_placement(&p); 

    set_camera_view(&c, &d.floors[d.curr_floor], &p);
    render_camera_view(&c);

	while(run_dungeon_loop) {
        reset_debug_row();
        
        int ch = get_play_input();
        print_debug("Input String: %d", ch);

        if (ch == 'q') {
            run_dungeon_loop = false;
            break;
        };

        Coordinate prev_coors = {
            .x = p.coors.x,
            .y = p.coors.y
        };
        D_Tile prev_tile = get_tile(d.floors[d.curr_floor], prev_coors);
        print_debug("global_prev_coors (x, y): (%zu, %zu)", prev_coors.x,  prev_coors.y);

        Coordinate new_coors = get_new_coors_from_input(prev_coors, ch);
        D_Tile new_tile = get_tile(d.floors[d.curr_floor], new_coors);

        if (!new_tile.is_blocker) {
            update_character_position(&p, new_coors);
            print_debug("global_new_coors (x, y): (%zu, %zu)", p.coors.x, p.coors.y);

            //update camera coordinates to follow player
            update_camera_position(&p, &c);
        }

        set_camera_view(&c, &d.floors[d.curr_floor], &p);
        render_camera_view(&c);

	};

    end_ncurses();

	return 0;
}

