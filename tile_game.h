#ifndef TILE_GAME_H_
#define TILE_GAME_H_

#include <stdint.h>

struct game_state {
  uint8_t tiles[4][4];
  uint8_t empty_row, empty_col;
  uint16_t num_steps;
};

uint64_t serialize(struct game_state state);
struct game_state deserialize(uint64_t state);

void move_up(struct game_state *state);
void move_down(struct game_state *state);
void move_left(struct game_state *state);
void move_right(struct game_state *state);

#endif // TILE_GAME_H_