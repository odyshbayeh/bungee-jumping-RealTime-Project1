#include "header.h"
#include "opengl.h"

// Game and signal variables used
volatile sig_atomic_t game_over = 0;
volatile sig_atomic_t player_done_signal = 0;
volatile sig_atomic_t start_task_signal_received = 0;
int max_score_reached = 0;
int game_duration;
int max_score;
siginfo_t last_signal_info;
int winning_team = -1;
time_t start_time;
time_t current_time;
int jump_time;
int oscillation_time;
int pull_time;
int duration_time;
int elapsedTime = 0;

//////////////////////////////////not used///////////////////////

// int currentTeamA = 0;
// int currentTeamB = 1;
// int currentTeamC = 2;
// int teamAscore = 0;
// int teamBscore = 0;
// int teamCscore = 0;
// char duration_str[16];
// char time2suration[16];
// int pipefd[2];
// volatile int current_round = 0;
// int active_player_y_offset = 150;
// double elapsed_time;

////////////////////////////////////////////////////////////////
Team teams[NUM_TEAMS];
TeamPipes team_pipes[NUM_TEAMS];
PlayerData player_data;
////////////////////////////////////////////////////////////////
// function to load configuration
void load_config(const char *filename, int *game_duration, int *max_score) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read the values from the file using fscanf
    if (fscanf(file, "game_duration = %d\n", game_duration) != 1) {
        printf("Error reading game_duration from file.\n");
    }
    if (fscanf(file, "max_score = %d\n", max_score) != 1) {
        printf("Error reading max_score from file.\n");
    }

    fclose(file);
}
////////////////////////////////////////////////////////////
int calculate_team_score(int team_id) {
    int team_score = 0;
    for (int player = 0; player < NUM_PLAYERS; player++) {
        team_score += teams[team_id].players[player].score;
    }
    if (team_score >= max_score) {
        max_score_reached = 1;
    }
    return team_score;
}
///////////////////////////////////////////////////////////////
void precise_sleep(int seconds) {
    struct timespec req = {seconds, 0}, rem;
    nanosleep(&req, &rem);
}
/////////////////////////////////////////////////////////////////////////////
// signal handler for referee process
void referee_signal_handler(int signum, siginfo_t *info, void *context) {
    last_signal_info = *info;
    player_done_signal = 1;
}
/////////////////////////////////////////////////////////////////////////////
void player_signal_handler(int signum) {
    start_task_signal_received = 1;
    //printf("Player signal handler activated. Task signal received set to 1.\n");
}
////////////////////////////////////////////////////////////////////////////
// initialize game and players
void init_game(int max_score, int max_duration) {
    srand(time(NULL));
    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int player = 0; player < NUM_PLAYERS; player++) {
            teams[team].players[player] = (Player){
                .player_id = player,
                .energy = MIN_ENERGY + rand() % (MAX_ENERGY - MIN_ENERGY + 1),
                .score = 0,
                .state = (player == 0) ? READY_TO_JUMP : DONE
            };
            if (pipe(team_pipes[team].player_pipe[player]) == -1) {
                perror("Pipe creation failed");
                exit(1);
            }
        }
    }
    printf("Game initialized with %d teams.\n", NUM_TEAMS);
}
/////////////////////////////////////////////////////////////////////////////////
// helper function to calculate jump time
int calculate_jump_time(int energy) {
    return energy < 50 ? 5 : (10 - energy / 20);
}
////////////////////////////////////////////////////////////////////////////////////
// helper function to generate random oscillation time
int generate_oscillation_time() {
    return random_delay(1, 3);
}
/////////////////////////////////////////////////////////////////////////////////////////
// helper function to calculate pull time based on IDLE player's energy in team
int calculate_pull_time(int team_id) {
    int pull_time = 2;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (teams[team_id].players[i].state == DONE) {
            pull_time += teams[team_id].players[i].energy / 20;
        }
    }
    return pull_time;
}
/////////////////////////////////////////////////////////////////////////////////////
void player_process(int team_id, int player_id) {
    printf("Player %d in Team %d: Setting up and waiting for signal\n", player_id + 1, team_id + 1);
////the initialization of the signal
    struct sigaction sa;
    sa.sa_handler = player_signal_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    close(team_pipes[team_id].player_pipe[player_id][0]);
////the mask of the signal
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    // close(pipefd[0]);  // not used any where

    Player *player = &teams[team_id].players[player_id];
    while (!game_over || max_score_reached) {
        while (!start_task_signal_received) {
            pause();
        }
        if (max_score_reached == 1) {
            break;
        }
        start_task_signal_received = 0;
        //printf("Player %d in Team %d received SIGUSR1, starting actions\n", player_id + 1, team_id + 1);

        int jump_time = calculate_jump_time(player->energy);
        int oscillation_time = generate_oscillation_time();
        int pull_time = calculate_pull_time(team_id);
        // sprintf(time2suration,"%d",oscillation_time+pull_time);
        // sprintf(duration_str, "%d", jump_time);


        player_data.status_done = 0;
        player_data.player_id = player->player_id;
        player_data.team_id = team_id;
        player_data.jump_time = jump_time;
        player_data.oscillation_time = oscillation_time;
        player_data.pull_time = pull_time;
        player_data.team_score = calculate_team_score(team_id);

        for (int i = 0; i < NUM_PLAYERS; i++) {
            player_data.energy_levels[i] = teams[team_id].players[i].energy;
        }
        player_data.next_player_pid = teams[team_id].players[(player_id + 1) % NUM_PLAYERS].pid;

        write(team_pipes[team_id].player_pipe[player_id][1], &player_data, sizeof(player_data));

        //printf("\nplayer data structure is sending %d\n",player_data.player_id);
        player->energy -= calculate_energy_usage(jump_time);
        player->score += calculate_score(jump_time);

        // if (team_id == 0) teamAscore = calculate_team_score(0);
        // else if (team_id == 1) teamBscore = calculate_team_score(1);
        // else if (team_id == 2) teamCscore = calculate_team_score(2);
        //precise_sleep(jump_time);
        printf("Player %d in Team %d completed jump with time %d with Energy: %d, Score: %d\n",player_id + 1, team_id + 1, jump_time,player->energy, player->score);

        //precise_sleep(oscillation_time);
        printf("Player %d in Team %d oscillated. Time: %d seconds\n", player_id + 1, team_id + 1, oscillation_time);

        // Pull action
        //precise_sleep(pull_time);
        printf("Player %d in Team %d completed pulling. Time: %d seconds\n", player_id + 1, team_id + 1, pull_time);

        player_data.status_done = 1;
        player_data.next_player_pid = teams[team_id].players[player_id + 1].pid;
        write(team_pipes[team_id].player_pipe[player_id][1], &player_data, sizeof(player_data));

        player->state = DONE;

        int next_player_id = (player_id + 1) % NUM_PLAYERS;
        teams[team_id].players[next_player_id].state = READY_TO_JUMP;
        teams[team_id].players[(player_id + 2) % 3].state = DONE;
        teams[team_id].players[(player_id - 1) % 3].state = DONE;
        //printf("Player %d in Team %d set Player %d in Team %d to READY_TO_JUMP\n",player_id + 1, team_id + 1, next_player_id + 1, team_id + 1);
        while (!start_task_signal_received) {
            pause();
        }
        start_task_signal_received = 0;
        for (int team = 0; team < NUM_TEAMS; team++) {
            int team_score = calculate_team_score(team);
            if (team_score >= max_score) {
                winning_team = team;
                game_over = 1;
                printf("Team %d has reached the maximum score of %d!\n", team + 1, team_score);
                for (int team = 0; team < NUM_TEAMS; team++) {
                    for (int player = 0; player < NUM_PLAYERS; player++) {
                        printf("Player %d in Team %d exiting as the game has ended.\n", player_id + 1, team_id + 1);
                        kill(teams[team].players[player].pid, SIGTERM);
                    }
                }
                break;
            }
        }
    }
    exit(0);
}
//////////////////////////////////////////////////////////////////
int random_delay(int min_time, int max_time) {
    return rand() % (max_time - min_time + 1) + min_time;
}
/////////////////////////////////////////////////////////////
int calculate_energy_usage(int action_time) {
    return action_time / 2;
}
////////////////////////////////////////////////////////////
int calculate_score(int action_time) {
    return 10 - action_time / 2;
}
/////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <config_file>\n", argv[0]);
        exit(1);
    }

    load_config(argv[1], &game_duration, &max_score);
    printf("game duration :%d max score :%d\n", game_duration, max_score);
    start_time = time(NULL);

    init_game(max_score, game_duration);

    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int player = 0; player < NUM_PLAYERS; player++) {
            if (pipe(team_pipes[team].player_pipe[player]) == -1) {
                perror("Pipe creation failed");
                exit(1);
            }
            pid_t pid = fork();
            if (pid == 0) {
                close(team_pipes[team].player_pipe[player][0]);
                player_process(team, player);
                exit(0);
            } else if (pid > 0) {
                teams[team].players[player].pid = pid;
                close(team_pipes[team].player_pipe[player][1]);
            } else {
                perror("Fork failed");
                exit(1);
            }
        }
    }

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = referee_signal_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR2, &sa, NULL);

    int current_players[NUM_TEAMS] = {0};
    printf("Referee process started.\n");

    PlayerData currentplayerdata[NUM_TEAMS];

    while (!game_over && !max_score_reached) {
        current_time = time(NULL);
        double elapsed_time = difftime(current_time, start_time);

        if (elapsed_time >= game_duration) {
            game_over = 1;
            printf("Game duration reached.\n");
            for (int t = 0; t < NUM_TEAMS; t++) {
                for (int p = 0; p < NUM_PLAYERS; p++) {
                    printf("Player %d in Team %d exiting as the game has ended.\n", p + 1, t + 1);
                    if (kill(teams[t].players[p].pid, SIGTERM) == -1) {
                      perror("Failed to send SIGTERM to player");
                    }
                }
            }
            exit(0);
        }

        printf("--- New Round --- (Elapsed Time: %.0f seconds)\n", elapsed_time);

        for (int team = 0; team < NUM_TEAMS; team++) {
            int player_id = current_players[team];
            Player *player = &teams[team].players[player_id];

            if (player->state == READY_TO_JUMP) {
                //printf("Referee signaling Player %d in Team %d with PID %d\n", player_id + 1, team + 1, player->pid);
                if (kill(player->pid, SIGUSR1) == -1) {
                    perror("Failed to send SIGUSR1 to player");
                }
            } else {
                //printf("Player %d in Team %d is not READY_TO_JUMP (state: %d)\n", player_id + 1, team + 1, player->state);
            }
        }

        int all_players_done = 0;
        int max_action_time = 0;

        while (!all_players_done) {
            all_players_done = 1;
            max_action_time = 0;

            for (int team = 0; team < NUM_TEAMS; team++) {
                int player_id = current_players[team];

                ssize_t bytes_read = read(team_pipes[team].player_pipe[player_id][0], &currentplayerdata[team], sizeof(PlayerData));
                if (bytes_read > 0) {
                    if (currentplayerdata[team].status_done == 1) {
                        player_done_signal = 1;
                        teams[team].players[player_id].state = DONE;
                    } else {
                        all_players_done = 0;
                    }
                    int player_action_time = currentplayerdata[team].jump_time + currentplayerdata[team].oscillation_time + currentplayerdata[team].pull_time;
                    if (player_action_time > max_action_time) {
                        max_action_time = player_action_time;
                    }
                } else {
                    all_players_done = 0;
                }
            }
            if (!all_players_done) {
                 pid_t pid = fork();
                if (pid == 0) {
                    char jump_time_str[NUM_TEAMS][16];
                    char oscillation_time_str[NUM_TEAMS][16];
                    char pull_time_str[NUM_TEAMS][16];
                    char team_id_str[NUM_TEAMS][16];
                    char player_id_str[NUM_TEAMS][16];
                    char team_score_str[NUM_TEAMS][16];
                    char energy_levels_str[NUM_TEAMS][NUM_PLAYERS][16];

                    for (int i = 0; i < NUM_TEAMS; i++) {
                        sprintf(jump_time_str[i], "%d", currentplayerdata[i].jump_time);
                        sprintf(oscillation_time_str[i], "%d", currentplayerdata[i].oscillation_time);
                        sprintf(pull_time_str[i], "%d", currentplayerdata[i].pull_time);
                        sprintf(team_id_str[i], "%d", currentplayerdata[i].team_id);
                        sprintf(player_id_str[i], "%d", currentplayerdata[i].player_id);
                        sprintf(team_score_str[i], "%d", currentplayerdata[i].team_score);

                        for (int j = 0; j < NUM_PLAYERS; j++) {
                            sprintf(energy_levels_str[i][j], "%d", currentplayerdata[i].energy_levels[j]);
                        }
                    }

                    execlp(
                        "./opengl",
                        "./opengl",
                        jump_time_str[0], oscillation_time_str[0], pull_time_str[0], team_id_str[0], player_id_str[0], team_score_str[0],
                        energy_levels_str[0][0], energy_levels_str[0][1], energy_levels_str[0][2],

                        jump_time_str[1], oscillation_time_str[1], pull_time_str[1], team_id_str[1], player_id_str[1], team_score_str[1],
                        energy_levels_str[1][0], energy_levels_str[1][1], energy_levels_str[1][2],

                        jump_time_str[2], oscillation_time_str[2], pull_time_str[2], team_id_str[2], player_id_str[2], team_score_str[2],
                        energy_levels_str[2][0], energy_levels_str[2][1], energy_levels_str[2][2],
                        NULL
                    );

                    perror("execlp failed");
                    exit(1);
                }
                sleep(max_action_time-2);
            }
            max_action_time=-2;
        }
        for (int team = 0; team < NUM_TEAMS; team++) {
            int completed_player_id = current_players[team];


            int next_player_id = (completed_player_id + 1) % NUM_PLAYERS;
            teams[team].players[next_player_id].state = READY_TO_JUMP;
            teams[team].players[completed_player_id].state = DONE;

            current_players[team] = next_player_id;

            if (kill(teams[team].players[next_player_id].pid, SIGUSR1) == -1) {
                perror("Failed to send SIGUSR1 to next player");
            } else {
                //printf("Referee signaled Player %d in Team %d to jump (PID: %d).\n", next_player_id + 1, team + 1, teams[team].players[next_player_id].pid);
            }
        }
        for (int team = 0; team < NUM_TEAMS; team++) {
            int team_score = calculate_team_score(team);
            if (team_score >= max_score) {
                winning_team = team;
                game_over = 1;
                printf("Team %d has reached the maximum score of %d!\n", team + 1, team_score);


                for (int t = 0; t < NUM_TEAMS; t++) {
                    for (int p = 0; p < NUM_PLAYERS; p++) {
                        printf("Player %d in Team %d exiting as the game has ended.\n", p + 1, t + 1);
                        if (kill(teams[t].players[p].pid, SIGTERM) == -1) {
                            perror("Failed to send SIGTERM to player");
                        }
                    }
                }
                exit(0);
            }
        }

        if (game_over) {

            for (int t = 0; t < NUM_TEAMS; t++) {
                for (int p = 0; p < NUM_PLAYERS; p++) {
                    printf("Player %d in Team %d exiting as the game has ended.\n", p + 1, t + 1);
                    if (kill(teams[t].players[p].pid, SIGTERM) == -1) {
                        perror("Failed to send SIGTERM to player");
                    }
                }
            }
            exit(0);
        }
    }

    while (wait(NULL) > 0);
    printf("Game ended.\n");

    return 0;
}
