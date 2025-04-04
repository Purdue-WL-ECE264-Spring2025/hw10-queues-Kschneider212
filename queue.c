#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <string.h>

int compare_game_states(struct game_state a, struct game_state b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a.tiles[i][j] != b.tiles[i][j]) {
                return 0; 
            }
        }
    }
    return 1;
}

void generate_valid_moves(struct game_state *state, struct queue *q) {
    int row = state->empty_row;
    int col = state->empty_col;

    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int i = 0; i < 4; i++) {
        int new_row = row + directions[i][0];
        int new_col = col + directions[i][1];

        if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
            struct game_state new_state = *state;
            new_state.tiles[row][col] = new_state.tiles[new_row][new_col];
            new_state.tiles[new_row][new_col] = 0;
            new_state.empty_row = new_row;
            new_state.empty_col = new_col;
            new_state.num_steps = state->num_steps + 1;

            enqueue(q, new_state); 
        }
    }
}

int is_goal_state(struct game_state state) {
    struct game_state goal = {
        .tiles = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0} 
        },
        .empty_row = 3,
        .empty_col = 3,
        .num_steps = 0
    };
    return compare_game_states(state, goal);
}

void enqueue(struct queue *q, struct game_state state) {
    struct game_state *new_state = malloc(sizeof(struct game_state));
    if (!new_state) {
        return;
    }
    *new_state = state;

    insert_at_tail(&q->data, (size_t)new_state); 
}

struct game_state dequeue(struct queue *q) {
    if (q->data.head == NULL) {
        return (struct game_state){0};
    }
    
    struct game_state *state = (struct game_state *)remove_from_head(&q->data);
    struct game_state result = *state;
    free(state); 
    return result;
}

int number_of_moves(struct game_state start) {
    struct queue q = { .data = { .head = NULL } };
    enqueue(&q, start);

    struct linked_list visited_states = { .head = NULL };

    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);
        if (is_goal_state(current_state)) {
            // Free all visited states
            struct list_node *cur = visited_states.head;
            while (cur) {
                struct list_node *next = cur->next;
                free((struct game_state *)cur->value);
                free(cur);
                cur = next;
            }
            return current_state.num_steps; 
        }

        int visited = 0;
        for (struct list_node *node = visited_states.head; node != NULL; node = node->next) {
            struct game_state *visited_state = (struct game_state *)node->value;
            if (compare_game_states(*visited_state, current_state)) {
                visited = 1;
                break;
            }
        }

        if (!visited) {
            struct game_state *new_state = malloc(sizeof(struct game_state));
            if (!new_state) {
                continue; 
            }
            *new_state = current_state;
            insert_at_tail(&visited_states, (size_t)new_state);
            generate_valid_moves(&current_state, &q);
        }
    }

    // Free all visited states before returning
    struct list_node *cur = visited_states.head;
    while (cur) {
        struct list_node *next = cur->next;
        free((struct game_state *)cur->value);
        free(cur);
        cur = next;
    }

    return -1; 
}

