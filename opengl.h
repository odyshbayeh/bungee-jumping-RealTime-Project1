//
// Created by odysh on 11/9/2024.
//

#ifndef OPENGL_H
#define OPENGL_H

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
#include <sys/time.h>

// Assuming NUM_TEAMS and NUM_PLAYERS are already defined somewhere (e.g., in header.h)
#define NUM_TEAMS 3
#define NUM_PLAYERS 3

// Declare arrays for jump time, oscillation time, pull time, team score, etc.
extern float JumpTime[NUM_TEAMS];          // Stores jump time for each team
extern float OscillationTime[NUM_TEAMS];   // Stores oscillation time for each team
extern float PullTime[NUM_TEAMS];          // Stores pull time for each team
extern int TeamID[NUM_TEAMS];              // Stores team IDs for each active player
extern int PlayerID[NUM_TEAMS];            // Stores player IDs for each active player
extern int TeamScore[NUM_TEAMS];           // Stores the current score of each team
extern int EnergyLevels[NUM_TEAMS][NUM_PLAYERS]; // Stores energy levels for each player in each team

//opengl functions

void init();
void display();
void timerCallback(int value);
void POLYGON(float pos_x, float pos_y, int color);
void drawLine(float from_posX, float from_posY, float to_posX, float to_posY);
void drawLine1(float from_posX1, float from_posY1, float to_posX1, float to_posY1);
void TeamScores(int team_A_score , int team_B_score , int team_C_score);
void playerHead(float pos_x, float pos_y, int teamNo, char *player_energy);
void RefereeHead(float pos_x, float pos_y, int teamNo);
void playerEnergy(float pos_x, float pos_y, const char* energy);
void playerBody(float pos_x, float pos_y, int teamNo);
void playerArms(float pos_x, float pos_y, float teamNo);
void playerLegs(float pos_x, float pos_y, float teamNo);
void player(float pos_x, float pos_y, int teamNo, char *player_energy);
void draw();
void playerNo(float pos_x, float pos_y, char *playerNo);
void drawPlayers();
void Referee(float pos_x, float pos_y, int teamNo);
void drawreferee();
void drawText(float x, float y, const char* text);
void motion_team_A_player1_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA );
void motion_team_B_player1_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB );
void motion_team_C_player1_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC );
void motion_team_A_player2_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA );
void motion_team_B_player2_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB );
void motion_team_C_player2_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC );
void motion_team_A_player3_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA );
void motion_team_B_player3_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB );
void motion_team_C_player3_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC );


#endif //OPENGL_H
