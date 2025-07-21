#ifndef HEADER_H
#define HEADER_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <GL/glut.h>
#include <sys/time.h>  // Required for gettimeofday

// Constants
#define NUM_TEAMS 3         // Number of teams (example value; adjust as necessary)
#define NUM_PLAYERS 3       // Number of players per team (example value; adjust as necessary)
#define MIN_ENERGY 50       // Minimum energy value
#define MAX_ENERGY 100      // Maximum energy value
#define READY_TO_JUMP 1     // State constant for ready players
#define DONE 0              // State constant for players who have completed actions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


// Structures
typedef struct Player {
    int player_id;
    int energy;
    int score;
    int state;
    pid_t pid;
} Player;

typedef struct {
    int status_done;          // 0 = in progress, 1 = completed
    int player_id;            // ID of the player
    int team_id;              // Team ID
    int jump_time;            // Time taken for the jump action
    int oscillation_time;     // Oscillation time
    int pull_time;            // Pull time
    int team_score;           // Current score of the team
    int energy_levels[NUM_PLAYERS];  // Energy levels of all players in the team
    pid_t next_player_pid;    // PID of the next player
} PlayerData;


typedef struct Team {
    Player players[NUM_PLAYERS];
} Team;

typedef struct TeamPipes {
    int player_pipe[NUM_PLAYERS][2]; // Pipes for each player in a team
} TeamPipes;

// External variables
extern int game_duration;
extern volatile int current_round;
extern siginfo_t last_signal_info;//hi
extern Team teams[NUM_TEAMS];
extern TeamPipes team_pipes[NUM_TEAMS];


//function prototypes
void load_config(const char *filename, int *game_duration, int *max_score);
int calculate_team_score(int team_id);
void precise_sleep(int seconds);
void referee_signal_handler(int signum, siginfo_t *info, void *context);
void player_signal_handler(int signum);
void init_game(int max_score, int max_duration);
int calculate_jump_time(int energy);
int generate_oscillation_time();
int calculate_pull_time(int team_id);
void referee_process();
void player_process(int team_id, int player_id);
int random_delay(int min_time, int max_time);
int calculate_energy_usage(int action_time);
int calculate_score(int action_time);



#endif
