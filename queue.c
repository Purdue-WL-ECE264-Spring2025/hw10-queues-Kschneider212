#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <string.h>

// Helper function to compare two game states (used for goal check)
int compare_game_states(struct game_state a, struct game_state b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (a.tiles[i][j] != b.tiles[i][j]) {
                return 0; // Not equal
            }
        }
    }
    return 1; // Equal
}

// Helper function to generate all possible valid moves from the current state
void generate_valid_moves(struct game_state *state, struct queue *q) {
    int row = state->empty_row;
    int col = state->empty_col;

    // List of possible moves (up, down, left, right)
    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    // Try all possible moves
    for (int i = 0; i < 4; i++) {
        int new_row = row + directions[i][0];
        int new_col = col + directions[i][1];

        // Check if the move is within bounds
        if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
            // Swap the empty space with the adjacent tile
            struct game_state new_state = *state;
            new_state.tiles[row][col] = new_state.tiles[new_row][new_col];
            new_state.tiles[new_row][new_col] = 0;
            new_state.empty_row = new_row;
            new_state.empty_col = new_col;
            new_state.num_steps = state->num_steps + 1; // Increment step count

            enqueue(q, new_state); // Enqueue the new state
        }
    }
}

// Function to check if the puzzle is solved (goal state)
int is_goal_state(struct game_state state) {
    // Define the goal state (solved puzzle configuration)
    struct game_state goal = {
        .tiles = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0} // 0 represents the empty space
        },
        .empty_row = 3,
        .empty_col = 3,
        .num_steps = 0
    };

    // Compare the current state with the goal state
    return compare_game_states(state, goal);
}

// Function to enqueue a new game state into the queue
void enqueue(struct queue *q, struct game_state state) {
    // Allocate memory for a new node to store the game state
    struct game_state *new_state = malloc(sizeof(struct game_state));
    if (!new_state) {
        return; // Handle malloc failure
    }
    *new_state = state;

    insert_at_tail(&q->data, (size_t)new_state); // Insert state at the tail of the list
}

// Function to dequeue a game state from the queue
struct game_state dequeue(struct queue *q) {
    if (q->data.head == NULL) {
        // Return a default empty state if the queue is empty
        return (struct game_state){0};
    }
    
    struct game_state *state = (struct game_state *)remove_from_head(&q->data);
    struct game_state result = *state;
    free(state); // Free the memory after returning the value
    return result;
}

// Function to calculate the number of moves required to solve the puzzle
int number_of_moves(struct game_state start) {
    struct queue q = { .data = { .head = NULL } };
    enqueue(&q, start);

    // Set to track visited states to avoid loops
    struct linked_list visited_states = { .head = NULL };

    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);

        // Check if the current state is the goal state
        if (is_goal_state(current_state)) {
            return current_state.num_steps; // Return the number of steps to reach the goal
        }

        // Check if this state has been visited before
        int visited = 0;
        for (struct list_node *node = visited_states.head; node != NULL; node = node->next) {
            struct game_state *visited_state = (struct game_state *)node->value;
            if (compare_game_states(*visited_state, current_state)) {
                visited = 1;
                break;
            }
        }

        if (!visited) {
            // Mark the current state as visited by adding it to the visited list
            struct game_state *new_state = malloc(sizeof(struct game_state));
            if (!new_state) {
                continue; // Skip if memory allocation fails
            }
            *new_state = current_state;
            insert_at_tail(&visited_states, (size_t)new_state);

            // Generate all valid moves from the current state
            generate_valid_moves(&current_state, &q);
        }
    }

    return -1; // Return -1 if no solution is found (in case of an unsolvable puzzle)
}
