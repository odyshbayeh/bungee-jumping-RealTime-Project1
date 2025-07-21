#include "opengl.h"
#include "header.h"

#define NUM_TEAMS 3
#define NUM_PLAYERS 3

// Variables to hold argument data
float JumpTime[NUM_TEAMS];
float OscillationTime[NUM_TEAMS];
float PullTime[NUM_TEAMS];
int TeamID[NUM_TEAMS];
int PlayerID[NUM_TEAMS];
int TeamScore[NUM_TEAMS];
int EnergyLevels[NUM_TEAMS][NUM_PLAYERS];
int elapsedTime;


///////////////////////////////init////////////////////////////////////////////////////////////////////////////////////////////////

void init() {
    glClearColor(.2, 0.3, 0.5, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1200);
    glMatrixMode(GL_MODELVIEW);
}




///////////////////////////////display////////////////////////////////////////////////////////////////////////////////////////////////

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw background elements
    draw();
    drawPlayers();
    drawreferee();

    // Display timer
    int minutes;
    int seconds;
    minutes = elapsedTime / 60;
    seconds = elapsedTime % 60;
    char timerText[20];
    snprintf(timerText, sizeof(timerText), "Time: %02d:%02d", minutes, seconds);
    drawText(800, 450, timerText);

    // Display team scores dynamically
    TeamScores(TeamScore[0], TeamScore[1], TeamScore[2]);

    // Trigger player motions for each team in a hardcoded way
    for (int i = 0; i < NUM_TEAMS; i++) {
        switch (TeamID[i]) {
            case 0: // Team A
                if (PlayerID[i] == 0) {
                    motion_team_A_player1_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 1) {
                    motion_team_A_player2_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 2) {
                    motion_team_A_player3_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                }
                break;

            case 1: // Team B
                if (PlayerID[i] == 0) {
                    motion_team_B_player1_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 1) {
                    motion_team_B_player2_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 2) {
                    motion_team_B_player3_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                }
                break;

            case 2: // Team C
                if (PlayerID[i] == 0) {
                    motion_team_C_player1_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 1) {
                    motion_team_C_player2_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                } else if (PlayerID[i] == 2) {
                    motion_team_C_player3_jumper(JumpTime[i], OscillationTime[i], PullTime[i], EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
                }
                break;

            default:
                fprintf(stderr, "Unknown Team ID: %d\n", TeamID[i]);
                break;
        }
    }

    // Swap buffers to display the rendered frame
    glutSwapBuffers();
}



/////////////////////////////////////////////main///////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    // // Print number of arguments for debugging
    // printf("Number of arguments received: %d\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("Argument %d: %s\n", i, argv[i]);
    // }

    // Updated argument validation
    int expected_argc = (NUM_TEAMS * (7 + NUM_PLAYERS)) + 1;
    // if (argc != expected_argc) {
    //     fprintf(stderr, "Warning: Expected %d arguments, but got %d.\n", expected_argc, argc);
    //     fprintf(stderr, "Usage: %s <jump_time_0> <oscillation_time_0> <pull_time_0> <team_id_0> <player_id_0> <team_score_0> <energy_level_0_0> <energy_level_0_1> <energy_level_0_2> ... <jump_time_2> <oscillation_time_2> <pull_time_2> <team_id_2> <player_id_2> <team_score_2> <energy_level_2_0> <energy_level_2_1> <energy_level_2_2>\n", argv[0]);
    //     // Return 0 here to see if parsing arguments still works despite mismatches
    // }

    int arg_index = 1;  // Start from the first argument after executable name

    // Parse the arguments for each active player
    for (int i = 0; i < NUM_TEAMS; i++) {
        // Check if the argument index is within bounds
        if (arg_index >= argc) {
            fprintf(stderr, "Error: Not enough arguments provided for parsing. Expected %d arguments, but ran out at index %d.\n", expected_argc, arg_index);
            return 1;
        }

        // Parse arguments for this team
        JumpTime[i] = atof(argv[arg_index++]);
        OscillationTime[i] = atof(argv[arg_index++]);
        PullTime[i] = atof(argv[arg_index++]);
        TeamID[i] = atoi(argv[arg_index++]);
        PlayerID[i] = atoi(argv[arg_index++]);
        TeamScore[i] = atoi(argv[arg_index++]);

        // Parse energy levels of all players in the team
        for (int j = 0; j < NUM_PLAYERS; j++) {
            if (arg_index >= argc) {
                fprintf(stderr, "Error: Not enough arguments provided for energy levels. Expected %d arguments, but ran out at index %d.\n", expected_argc, arg_index);
                return 1;
            }
            EnergyLevels[i][j] = atoi(argv[arg_index++]);
        }
    }

    // Log parsed values for debugging purposes
    for (int i = 0; i < NUM_TEAMS; i++) {
        printf("Team %d: jump_time=%.2f, oscillation_time=%.2f, pull_time=%.2f, team_id=%d, player_id=%d, team_score=%d, energy_levels=%d, %d, %d\n",
               i, JumpTime[i], OscillationTime[i], PullTime[i], TeamID[i], PlayerID[i], TeamScore[i],
               EnergyLevels[i][0], EnergyLevels[i][1], EnergyLevels[i][2]);
    }

    // Initialize OpenGL and game window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1200, 1200);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bungee Jumping Game");

    init();
    glutTimerFunc(1,timerCallback,0);
    // Set display callback to handle rendering
    glutDisplayFunc(display);

    // Start the OpenGL main loop
    glutMainLoop();

    return 0;
}



//////////////////////////////////////////////////////Time///////////////////////////////////////////////////////////////////////////

void timerCallback(int value) {
    elapsedTime++;
    glutPostRedisplay();
    glutTimerFunc(1000, timerCallback, 0);
}



//////////////////////////////////////////////////Polygon///////////////////////////////////////////////////////////////////////////////



void POLYGON(float pos_x, float pos_y, int color) {
    glLoadIdentity();
    glBegin(GL_POLYGON);
    if (color == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (color == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x, pos_y);
    glVertex2f(pos_x + 600, pos_y);
    glVertex2f(pos_x + 600, pos_y + 200);
    glVertex2f(pos_x, pos_y + 200);
    glEnd();
}





//////////////////////////////////////////////////Lines///////////////////////////////////////////////////////////////////////////////


void drawLine(float from_posX, float from_posY, float to_posX, float to_posY) {
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(from_posX, from_posY);
    glVertex2f(to_posX, to_posY);
    glEnd();
}


//////////////////////////////////////////////////line1///////////////////////////////////////////////////////////////////////////////


void drawLine1(float from_posX1, float from_posY1, float to_posX1, float to_posY1) {
    glLoadIdentity();
    glColor3f(0.0, 1.0, 0.0); //  color to green
    glLineWidth(5.0);          //  line width
    glBegin(GL_LINES);
    glVertex2f(from_posX1, from_posY1);
    glVertex2f(to_posX1, to_posY1);
    glEnd();
    glLineWidth(1.0);
}


//////////////////////////////////////////////////Team Score/////////////////////////////////////////////////////////////////////////////



void TeamScores(int team_A_score , int team_B_score , int team_C_score){

     char A = team_A_score;
    char AStr[30];
    sprintf(AStr, "Team A score: %d", team_A_score);

     char B = team_B_score;
    char BStr[30];
    sprintf(BStr, "Team B score: %d", team_B_score);

     char C = team_C_score;
    char CStr[30];
    sprintf(CStr, "Team C score: %d", team_C_score);

    drawText(800, 550, CStr);

    drawText(800, 650, BStr);

    drawText(800, 750, AStr);

}




/////////////////////////////////////player head///////////////////////////////////////////////////////////////////////////////////////////



void playerHead(float pos_x, float pos_y, int teamNo, char *player_energy) {
    glPushMatrix();
    glLoadIdentity();
    float theta;
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + 20 * cos(theta), pos_y + 20 * sin(theta));
    }
    glEnd();
    glPopMatrix();

    playerEnergy(pos_x, pos_y, player_energy);


}

/////////////////////////////////////Referee head///////////////////////////////////////////////////////////////////////////////////////////


void RefereeHead(float pos_x, float pos_y, int teamNo) {
    glPushMatrix();
    glLoadIdentity();
    float theta;
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + 20 * cos(theta), pos_y + 20 * sin(theta));
    }
    glEnd();
    glPopMatrix();

}





/////////////////////////////////////player health ///////////////////////////////////////////////////////////////////////////////////////




void playerEnergy(float pos_x, float pos_y, const char* energy) {
    glColor3f(0, 0, 0);
    glRasterPos2f(pos_x - 10, pos_y - 4);
    int len = (int)strlen(energy);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, energy[i]);
    }
}




////////////////////////////////////////////////////////player body////////////////////////////////////////////////////////////////////



void playerBody(float pos_x, float pos_y, int teamNo) {
    glLoadIdentity();
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x - 20, pos_y - 20);
    glVertex2f(pos_x + 20, pos_y - 20);
    glVertex2f(pos_x + 20, pos_y - 80);
    glVertex2f(pos_x - 20, pos_y - 80);
    glEnd();
}


//////////////////////////////////////////////////////player  arms////////////////////////////////////////////////////////////////////



void playerArms(float pos_x, float pos_y, float teamNo) {
    glLoadIdentity();
    glBegin(GL_LINE_LOOP);
    if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x - 20, pos_y - 30);
    glVertex2f(pos_x - 50, pos_y - 50);
    glEnd();

    glBegin(GL_LINE_LOOP);
    if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x + 20, pos_y - 30);
    glVertex2f(pos_x + 50, pos_y - 50);
    glEnd();
}



///////////////////////////////////////////////////////player legs////////////////////////////////////////////////////////////////////


void playerLegs(float pos_x, float pos_y, float teamNo) {
    glLoadIdentity();
    glBegin(GL_LINE_LOOP);
    if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x + 10, pos_y - 80);
    glVertex2f(pos_x + 10, pos_y - 100);
    glEnd();

    glBegin(GL_LINE_LOOP);
    if (teamNo == 0) {
        glColor3f(1, 1, 1); // white color (referee)
    } else if (teamNo == 1) {
        glColor3f(1, 0, 0); // red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // yellow color
    } else {
        glColor3f(0, 0, 1); // blue color
    }
    glVertex2f(pos_x - 10, pos_y - 80);
    glVertex2f(pos_x - 10, pos_y - 100);
    glEnd();
}



////////////////////////////////////////////////////////// player////////////////////////////////////////////////////////////////////



void player(float pos_x, float pos_y, int teamNo, char *player_energy) {
    playerHead(pos_x, pos_y, teamNo, player_energy);
    playerBody(pos_x, pos_y, teamNo);
    playerArms(pos_x, pos_y, teamNo);
    playerLegs(pos_x, pos_y, teamNo);
}


/////////////////////////////////////player Number///////////////////////////////////////////////////////////////////////////////////////////



void playerNo(float pos_x, float pos_y, char *playerNo) {
    glColor3f(0, 0, 0);
    glRasterPos2d(pos_x - 5, pos_y - 50);
    int len = (int)strlen(playerNo);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, playerNo[i]);
    }
}


/////////////////////////////////////Draw ///////////////////////////////////////////////////////////////////////////////////////////
void draw(){

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(750, 0);
    glVertex2f(1200, 0);
    glVertex2f(1200, 1200);
    glVertex2f(750, 1200);
    glEnd();


    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 900);
    glVertex2f(0, 1200);
    glVertex2f(750, 1200);
    glVertex2f(750, 900);
    glEnd();


    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(750, 0);
    glVertex2f(750, 900);
    glVertex2f(0, 900);
    glEnd();


    drawText(100, 1150, "Team A");
    drawText(350, 1150, "Team B");
    drawText(600, 1150, "Team C");
    drawText(850, 1150, "Referee");
    drawLine(0, 900, 1200, 900);
    drawLine(250, 1200, 250, 900);
    drawLine(500, 1200, 500, 900);
    drawLine(750, 1200, 750, 0);

}

/////////////////////////////////////Draw players///////////////////////////////////////////////////////////////////////////////////////////

void drawPlayers() {

    glLoadIdentity();
}


/////////////////////////////////////Referee///////////////////////////////////////////////////////////////////////////////////////////


void Referee(float pos_x, float pos_y, int teamNo) {
    RefereeHead(pos_x, pos_y, teamNo);
    playerBody(pos_x, pos_y, teamNo);
    playerArms(pos_x, pos_y, teamNo);
    playerLegs(pos_x, pos_y, teamNo);
}


/////////////////////////////////////Draw Referee///////////////////////////////////////////////////////////////////////////////////////////



void drawreferee() {
    glLoadIdentity();
    Referee(870, 1050, 0);
    playerNo(862, 1050, "Ref");
}



/////////////////////////////////////Draw Text///////////////////////////////////////////////////////////////////////////////////////////




void drawText(float x, float y, const char* text) {
    glColor3f(1.0, 1.0, 1.0); // white color
    glRasterPos2f(x, y);
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}





//////////////////////////////motion player 1 is jumper from team A///////////////////////////////////////////////////////////////////



void motion_team_A_player1_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA ) {


    static float currentX_TA = 125.0, currentY_TA = 1000.0;
    static float player1TargetY_TA = 600.0;
    static float player1OriginalY_TA = 1000.0;
    static float player2OriginalX_TA = 60.0, player2OriginalY_TA = 1100.0;
    static float player2TargetX_TA = 125.0, player2TargetY_TA = 1000.0;
    static float player2Target2Y_TA = 1150.0;
    static float player3OriginalX_TA = 190.0, player3OriginalY_TA = 1100.0;
    static float player3TargetX_TA = 125.0, player3TargetY_TA = 1000.0;
    static float player3Target2Y_TA = 1150.0;

    // flags
    static bool player1ReachedTarget_TA = false;
    static bool player1BackAtOriginal_TA = false;
    static bool player2ReachedTarget1_TA = false, player2ReachedTarget2_TA = false;
    static bool player2BackAtOriginal_TA = false;
    static bool player3ReachedTarget1_TA = false, player3ReachedTarget2_TA = false;
    static bool player3BackAtOriginal_TA = false;
    static clock_t player1StartTime = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;



    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;

    char energy1 = healthplayer1_TA;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TA);

     char energy2 = healthplayer2_TA;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TA);

     char energy3 = healthplayer3_TA;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TA);



//////////////////////////////////////////////////////////////////////////////////
     //player 1 (jumper ) motion

    //  moves from original position to target
    if (!player1ReachedTarget_TA) {
        if (player1StartTime == 0) {
            player1StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            currentY_TA = player1OriginalY_TA + (player1TargetY_TA - player1OriginalY_TA) * (elapsedTime / duration);
        } else {
            currentY_TA = player1TargetY_TA;
            player1ReachedTarget_TA = true;
            player1StartTime = 0;
        }
    }

    //  returns to original position after
    else if (player2ReachedTarget1_TA && player3ReachedTarget1_TA && !player1BackAtOriginal_TA) {
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            currentY_TA = player1TargetY_TA + (player1OriginalY_TA - player1TargetY_TA) * (elapsedTime / duration1);
        } else {
            currentY_TA = player1OriginalY_TA;
            player1BackAtOriginal_TA = true;
            player1ReturnStartTime = 0;
        }
    }


//////////////////////////////////////////////////////////////////////////////////
     //player 2 motion

    if (player1ReachedTarget_TA && !player2ReachedTarget1_TA) {
        //  moves to first target
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TA = 1100.0 + (player2TargetY_TA - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TA = 60.0 + (player2TargetX_TA - 60.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TA = player2TargetY_TA;
            player2OriginalX_TA = player2TargetX_TA;
            player2ReachedTarget1_TA = true;
            player2StartTime1 = 0;
        }
    } else if (player2ReachedTarget1_TA && !player2ReachedTarget2_TA) {
        //  moves to second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TA = player2TargetY_TA + (player2Target2Y_TA - player2TargetY_TA) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TA = player2Target2Y_TA;
            player2ReachedTarget2_TA = true;
            player2StartTime2 = 0;
        }
    } else if (player2ReachedTarget2_TA && !player2BackAtOriginal_TA && player1BackAtOriginal_TA) {
        // returns to original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TA = player2Target2Y_TA + (1100.0 - player2Target2Y_TA) * (elapsedTime / duration4);
            player2OriginalX_TA = 125.0 + (60.0 - 125.0) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TA = 1100.0;
            player2OriginalX_TA = 60.0;
            player2BackAtOriginal_TA = true;
            player2ReturnStartTime = 0;
        }
    }


//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion

    if (player1ReachedTarget_TA && !player3ReachedTarget1_TA) {
        //  moves to first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TA = 1100.0 + (player3TargetY_TA - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TA = 190.0 + (player3TargetX_TA - 190.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TA = player3TargetY_TA;
            player3OriginalX_TA = player3TargetX_TA;
            player3ReachedTarget1_TA = true;
            player3StartTime1 = 0;
        }
    } else if (player3ReachedTarget1_TA && !player3ReachedTarget2_TA) {
        //  moves to second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TA = player3TargetY_TA + (player3Target2Y_TA - player3TargetY_TA) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TA = player3Target2Y_TA;
            player3ReachedTarget2_TA = true;
            player3StartTime2 = 0;
        }
    } else if (player3ReachedTarget2_TA && !player3BackAtOriginal_TA && player1BackAtOriginal_TA) {
        //  returns to original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TA = player3Target2Y_TA + (1100.0 - player3Target2Y_TA) * (elapsedTime / duration4);
            player3OriginalX_TA = 125.0 + (190.0 - 125.0) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TA = 1100.0;
            player3OriginalX_TA = 190.0;
            player3BackAtOriginal_TA = true;
            player3ReturnStartTime = 0;
        }
    }





    drawLine1(125.0, player2Target2Y_TA, 125.0, currentY_TA);
    player(currentX_TA, currentY_TA, 1, energyStr);
    player(player2OriginalX_TA, player2OriginalY_TA, 1,energyStr2 );
    player(player3OriginalX_TA, player3OriginalY_TA, 1, energyStr3);
    playerNo(currentX_TA, currentY_TA, "1");
    playerNo(player2OriginalX_TA, player2OriginalY_TA, "2");
    playerNo(player3OriginalX_TA, player3OriginalY_TA, "3");




    if (!player1BackAtOriginal_TA || !player2BackAtOriginal_TA || !player3BackAtOriginal_TA) {
        glutPostRedisplay();
    }
}



//////////////////////////////motion player 1 is Jumper from team B///////////////////////////////////////////////////////////////////



void motion_team_B_player1_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB) {

    static float currentX_TB = 375.0, currentY_TB = 1000.0;
    static float player1TargetY_TB = 600.0;
    static float player1OriginalY_TB = 1000.0;
    static float player2OriginalX_TB = 310.0, player2OriginalY_TB = 1100.0;
    static float player2TargetX_TB = 375.0, player2TargetY_TB = 1000.0;
    static float player2Target2Y_TB = 1150.0;
    static float player3OriginalX_TB = 440.0, player3OriginalY_TB = 1100.0;
    static float player3TargetX_TB = 375.0, player3TargetY_TB = 1000.0;
    static float player3Target2Y_TB = 1150.0;



    // flags
    static bool player1ReachedTarget_TB = false;
    static bool player1BackAtOriginal_TB = false;
    static bool player2ReachedTarget1_TB = false, player2ReachedTarget2_TB = false;
    static bool player2BackAtOriginal_TB = false;
    static bool player3ReachedTarget1_TB = false, player3ReachedTarget2_TB = false;
    static bool player3BackAtOriginal_TB = false;
    static clock_t player1StartTime = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;


    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;


     char energy1 = healthplayer1_TB;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TB);

     char energy2 = healthplayer2_TB;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TB);

     char energy3 = healthplayer3_TB;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TB);







    //////////////////////////////////////////////////////////////////////////////////
     //player 1   (jumper) motion
    //  moves from original position to target1
    if (!player1ReachedTarget_TB) {
        if (player1StartTime == 0) {
            player1StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            currentY_TB = player1OriginalY_TB + (player1TargetY_TB - player1OriginalY_TB) * (elapsedTime / duration);
        } else {
            currentY_TB = player1TargetY_TB;
            player1ReachedTarget_TB = true;
            player1StartTime = 0;
        }
    }
    //  returns to original position
    else if (player2ReachedTarget1_TB && player3ReachedTarget1_TB && !player1BackAtOriginal_TB) {
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            currentY_TB = player1TargetY_TB + (player1OriginalY_TB - player1TargetY_TB) * (elapsedTime / duration1);
        } else {
            currentY_TB = player1OriginalY_TB;
            player1BackAtOriginal_TB = true;
            player1ReturnStartTime = 0;
        }
    }


//////////////////////////////////////////////////////////////////////////////////
     //player 2 motion

    if (player1ReachedTarget_TB && !player2ReachedTarget1_TB) {
        //  moves to first target
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TB = 1100.0 + (player2TargetY_TB - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TB = 310.0 + (player2TargetX_TB - 310.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TB = player2TargetY_TB;
            player2OriginalX_TB = player2TargetX_TB;
            player2ReachedTarget1_TB = true;
            player2StartTime1 = 0;
        }
    } else if (player2ReachedTarget1_TB && !player2ReachedTarget2_TB) {
        //  moves to second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TB = player2TargetY_TB + (player2Target2Y_TB - player2TargetY_TB) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TB = player2Target2Y_TB;
            player2ReachedTarget2_TB = true;
            player2StartTime2 = 0;
        }
    } else if ( player2ReachedTarget2_TB && !player2BackAtOriginal_TB && player1BackAtOriginal_TB) {
        // returns to original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TB = player2Target2Y_TB + (1100.0 - player2Target2Y_TB) * (elapsedTime / duration4);
            player2OriginalX_TB = 375.0 + (310.0 - 375.0) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TB = 1100.0;
            player2OriginalX_TB = 310.0;
            player2BackAtOriginal_TB = true;
            player2ReturnStartTime = 0;
        }
    }


//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion

    if (player1ReachedTarget_TB && !player3ReachedTarget1_TB) {
        //  moves to first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TB = 1100.0 + (player3TargetY_TB - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TB = 440.0 + (player3TargetX_TB - 440.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TB = player3TargetY_TB;
            player3OriginalX_TB = player3TargetX_TB;
            player3ReachedTarget1_TB = true;
            player3StartTime1 = 0;
        }
    } else if (player3ReachedTarget1_TB && !player3ReachedTarget2_TB) {
        //  moves to second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TB = player3TargetY_TB + (player3Target2Y_TB - player3TargetY_TB) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TB = player3Target2Y_TB;
            player3ReachedTarget2_TB = true;
            player3StartTime2 = 0;
        }
    } else if ( player3ReachedTarget2_TB && !player3BackAtOriginal_TB && player1BackAtOriginal_TB) {
        //  returns to original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TB = player3Target2Y_TB + (1100.0 - player3Target2Y_TB) * (elapsedTime / duration4);
            player3OriginalX_TB = 375.0 + (440.0 - 375.0) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TB = 1100.0;
            player3OriginalX_TB = 440.0;
            player3BackAtOriginal_TB = true;
            player3ReturnStartTime = 0;
        }
    }





    drawLine1(375, player2Target2Y_TB, 375, currentY_TB);
    player(currentX_TB, currentY_TB, 2, energyStr );
    player(player2OriginalX_TB, player2OriginalY_TB, 2, energyStr2);
    player(player3OriginalX_TB, player3OriginalY_TB, 2, energyStr3);
    playerNo(currentX_TB, currentY_TB, "1");
    playerNo(player2OriginalX_TB, player2OriginalY_TB, "2");
    playerNo(player3OriginalX_TB, player3OriginalY_TB, "3");


    if (!player1BackAtOriginal_TB || !player2BackAtOriginal_TB || !player3BackAtOriginal_TB) {
        glutPostRedisplay();
    }
}



//////////////////////////////motion player 1 is Jumper  from team C///////////////////////////////////////////////////////////////////



void motion_team_C_player1_jumper(float jumpTime , float osciTime , float pullTime,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC) {
    static float currentX_TC = 625.0, currentY_TC = 1000.0;
    static float player1TargetY_TC = 600.0;
    static float player1OriginalY_TC = 1000.0;
    static float player2OriginalX_TC = 560.0, player2OriginalY_TC = 1100.0;
    static float player2TargetX_TC = 625.0, player2TargetY_TC = 1000.0;
    static float player2Target2Y_TC = 1150.0;
    static float player3OriginalX_TC = 690.0, player3OriginalY_TC = 1100.0;
    static float player3TargetX_TC = 625.0, player3TargetY_TC = 1000.0;
    static float player3Target2Y_TC = 1150.0;

    // flags
    static bool player1ReachedTarget_TC = false;
    static bool player1BackAtOriginal_TC = false;
    static bool player2ReachedTarget1_TC = false, player2ReachedTarget2_TC = false;
    static bool player2BackAtOriginal_TC = false;
    static bool player3ReachedTarget1_TC = false, player3ReachedTarget2_TC = false;
    static bool player3BackAtOriginal_TC = false;
    static clock_t player1StartTime = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;




    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;

     char energy1 = healthplayer1_TC;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TC);

     char energy2 = healthplayer2_TC;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TC);

     char energy3 = healthplayer3_TC;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TC);






//////////////////////////////////////////////////////////////////////////////////
     //player 1 (jumper) motion

     // move form original to target 1
         if (!player1ReachedTarget_TC) {
        if (player1StartTime == 0) {
            player1StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            currentY_TC = player1OriginalY_TC + (player1TargetY_TC - player1OriginalY_TC) * (elapsedTime / duration);
        } else {
            currentY_TC = player1TargetY_TC;
            player1ReachedTarget_TC = true;
            player1StartTime = 0;
        }
    }

    // returns to original position
    else if (player2ReachedTarget1_TC && player3ReachedTarget1_TC && !player1BackAtOriginal_TC) {
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            currentY_TC = player1TargetY_TC + (player1OriginalY_TC - player1TargetY_TC) * (elapsedTime / duration1);
        } else {
            currentY_TC = player1OriginalY_TC;
            player1BackAtOriginal_TC = true;
            player1ReturnStartTime = 0;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 2 motion


         if (player1ReachedTarget_TC && !player2ReachedTarget1_TC) {
        //moves to first target
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TC = 1100.0 + (player2TargetY_TC - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TC = 560.0 + (player2TargetX_TC - 560.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TC = player2TargetY_TC;
            player2OriginalX_TC = player2TargetX_TC;
            player2ReachedTarget1_TC = true;
            player2StartTime1 = 0;
        }
    } else if (player2ReachedTarget1_TC && !player2ReachedTarget2_TC) {
        // moves to second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TC = player2TargetY_TC + (player2Target2Y_TC - player2TargetY_TC) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TC = player2Target2Y_TC;
            player2ReachedTarget2_TC = true;
            player2StartTime2 = 0;
        }
    } else if (player2ReachedTarget2_TC && !player2BackAtOriginal_TC && player1BackAtOriginal_TC) {
        //  returns to original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TC = player2Target2Y_TC + (1100.0 - player2Target2Y_TC) * (elapsedTime / duration4);
            player2OriginalX_TC = 625.0 + (560.0 - 625.0) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TC = 1100.0;
            player2OriginalX_TC = 560.0;
            player2BackAtOriginal_TC = true;
            player2ReturnStartTime = 0;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion


         if (player1ReachedTarget_TC && !player3ReachedTarget1_TC) {
        // moves to first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TC = 1100.0 + (player3TargetY_TC - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TC = 690.0 + (player3TargetX_TC - 690.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TC = player3TargetY_TC;
            player3OriginalX_TC = player3TargetX_TC;
            player3ReachedTarget1_TC = true;
            player3StartTime1 = 0;
        }
    } else if (player3ReachedTarget1_TC && !player3ReachedTarget2_TC) {
        //  moves to second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TC = player3TargetY_TC + (player3Target2Y_TC - player3TargetY_TC) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TC = player3Target2Y_TC;
            player3ReachedTarget2_TC = true;
            player3StartTime2 = 0;
        }
    } else if (player3ReachedTarget2_TC && !player3BackAtOriginal_TC && player1BackAtOriginal_TC) {
        //  returns to original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TC = player3Target2Y_TC + (1100.0 - player3Target2Y_TC) * (elapsedTime / duration4);
            player3OriginalX_TC = 625.0 + (690.0 - 625.0) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TC = 1100.0;
            player3OriginalX_TC = 690.0;
            player3BackAtOriginal_TC = true;
            player3ReturnStartTime = 0;
        }
    }




    drawLine1(625.0, player2Target2Y_TC, 625.0, currentY_TC);
    player(currentX_TC, currentY_TC, 3, energyStr);
    player(player2OriginalX_TC, player2OriginalY_TC, 3, energyStr2);
    player(player3OriginalX_TC, player3OriginalY_TC, 3, energyStr3);
    playerNo(currentX_TC, currentY_TC, "1");
    playerNo(player2OriginalX_TC, player2OriginalY_TC, "2");
    playerNo(player3OriginalX_TC, player3OriginalY_TC, "3");




    if (!player1BackAtOriginal_TC || !player2BackAtOriginal_TC || !player3BackAtOriginal_TC) {
        glutPostRedisplay();
    }
}






//////////////////////////////motion player 2 is Jumper  from team A///////////////////////////////////////////////////////////////////




void motion_team_A_player2_jumper(float jumpTime , float osciTime , float pullTime , int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA) {
    static float player1OriginalX_TA = 60.0, player1OriginalY_TA = 1100.0;
    static float player1TargetX1_TA = 125.0, player1TargetY1_TA = 1000.0;
    static float player1TargetY2_TA = 1150.0;
    static float player2OriginalX_TA = 125.0, player2OriginalY_TA = 1000.0;
    static float player2TargetY_TA = 600.0;
    static float player3OriginalX_TA = 190.0, player3OriginalY_TA = 1100.0;
    static float player3TargetX_TA = 125.0, player3TargetY_TA = 1000.0;
    static float player3TargetY2_TA = 1150.0;

    // flags
    static bool player1ReachedTarget1_TA = false, player1ReachedTarget2_TA = false, player1BackAtOriginal_TA = false;
    static bool player2ReachedTarget_TA = false, player2BackAtOriginal_TA = false;
    static bool player3ReachedTarget1_TA = false, player3ReachedTarget2_TA = false, player3BackAtOriginal_TA = false;
    static bool playersAtWaitPosition_TA = false;
    static bool readyToMoveTogether_TA = false;
    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;



    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;

    char energy1 = healthplayer1_TA;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TA);

     char energy2 = healthplayer2_TA;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TA);

     char energy3 = healthplayer3_TA;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TA);
//////////////////////////////////////////////////////////////////////////////////
     //player 2 (jumper) motion


    if (!player2ReachedTarget_TA) {
        // moving to the target position
        if (player2StartTime == 0) {
            player2StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player2OriginalY_TA = 1000.0 + (player2TargetY_TA - 1000.0) * (elapsedTime / duration);
        } else {
            player2OriginalY_TA = player2TargetY_TA;
            player2ReachedTarget_TA = true;
            player2StartTime = 0;
            playersAtWaitPosition_TA = true;
        }
    } else if (player2ReachedTarget_TA && player1ReachedTarget1_TA && player3ReachedTarget1_TA && !player2BackAtOriginal_TA) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player2OriginalY_TA = player2TargetY_TA + (1000.0 - player2TargetY_TA) * (elapsedTime / duration1);
        } else {
            player2OriginalY_TA = 1000.0;
            player2BackAtOriginal_TA = true;
            player2ReturnStartTime = 0;
            readyToMoveTogether_TA = true;
        }
    }




//////////////////////////////////////////////////////////////////////////////////
     //player 1 motion


    if (playersAtWaitPosition_TA && !player1ReachedTarget1_TA) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TA = 1100.0 + (player1TargetY1_TA - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TA = 60.0 + (player1TargetX1_TA - 60.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TA = player1TargetY1_TA;
            player1OriginalX_TA = player1TargetX1_TA;
            player1ReachedTarget1_TA = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TA && !player1ReachedTarget2_TA) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TA = player1TargetY1_TA + (player1TargetY2_TA - player1TargetY1_TA) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TA = player1TargetY2_TA;
            player1ReachedTarget2_TA = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TA && !player1BackAtOriginal_TA && player2BackAtOriginal_TA) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TA = player1TargetY2_TA + (1100.0 - player1TargetY2_TA) * (elapsedTime / duration4);
            player1OriginalX_TA = player1TargetX1_TA + (60.0 - player1TargetX1_TA) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TA = 1100.0;
            player1OriginalX_TA = 60.0;
            player1BackAtOriginal_TA = true;
            player1ReturnStartTime = 0;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion


    if (playersAtWaitPosition_TA && !player3ReachedTarget1_TA) {
        // moving to the first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TA = 1100.0 + (player3TargetY_TA - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TA = 190.0 + (player3TargetX_TA - 190.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TA = player3TargetY_TA;
            player3OriginalX_TA = player3TargetX_TA;
            player3ReachedTarget1_TA = true;
            player3StartTime1 = 0;
        }
    } else if ( player3ReachedTarget1_TA && !player3ReachedTarget2_TA) {
        // moving to the second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TA = player3TargetY_TA + (player3TargetY2_TA - player3TargetY_TA) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TA = player3TargetY2_TA;
            player3ReachedTarget2_TA = true;
            player3StartTime2 = 0;
        }
    } else if (player3ReachedTarget2_TA  && !player3BackAtOriginal_TA && player2BackAtOriginal_TA) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TA = player3TargetY2_TA + (1100.0 - player3TargetY2_TA) * (elapsedTime / duration4);
            player3OriginalX_TA = player3TargetX_TA + (190.0 - player3TargetX_TA) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TA = 1100.0;
            player3OriginalX_TA = 190.0;
            player3BackAtOriginal_TA = true;
            player3ReturnStartTime = 0;
        }
    }





    drawLine1(125.0, 1150.0, 125.0, player2OriginalY_TA);
    player(player1OriginalX_TA, player1OriginalY_TA, 1, energyStr);
    player(player2OriginalX_TA, player2OriginalY_TA, 1, energyStr2);
    player(player3OriginalX_TA, player3OriginalY_TA, 1, energyStr3);
    playerNo(player1OriginalX_TA, player1OriginalY_TA, "1");
    playerNo(player2OriginalX_TA, player2OriginalY_TA, "2");
    playerNo(player3OriginalX_TA, player3OriginalY_TA, "3");




    if (!player1BackAtOriginal_TA || !player2BackAtOriginal_TA || !player3BackAtOriginal_TA) {
        glutPostRedisplay();
    }
}




//////////////////////////////motion player 2 is Jumper  from team B///////////////////////////////////////////////////////////////////


void motion_team_B_player2_jumper(float jumpTime , float osciTime , float pullTime,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB) {
    static float player1OriginalX_TB = 310.0, player1OriginalY_TB = 1100.0;
    static float player1TargetX1_TB = 375.0, player1TargetY1_TB = 1000.0;
    static float player1TargetY2_TB = 1150.0;
    static float player2OriginalX_TB = 375.0, player2OriginalY_TB = 1000.0;
    static float player2TargetY_TB = 600.0;
    static float player3OriginalX_TB = 440.0, player3OriginalY_TB = 1100.0;
    static float player3TargetX_TB = 375.0, player3TargetY_TB = 1000.0;
    static float player3TargetY2_TB = 1150.0;




    // flags
    static bool player1ReachedTarget1_TB = false, player1ReachedTarget2_TB = false, player1BackAtOriginal_TB = false;
    static bool player2ReachedTarget_TB = false, player2BackAtOriginal_TB = false;
    static bool player3ReachedTarget1_TB = false, player3ReachedTarget2_TB = false, player3BackAtOriginal_TB = false;
    static bool playersAtWaitPosition_TB = false;
    static bool readyToMoveTogether_TB = false;



    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;



    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;
   char energy1 = healthplayer1_TB;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TB);

     char energy2 = healthplayer2_TB;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TB);

     char energy3 = healthplayer3_TB;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TB);


//////////////////////////////////////////////////////////////////////////////////
     //player 2 (jumper) motion


    if (!player2ReachedTarget_TB) {
        // moving to the target position
        if (player2StartTime == 0) {
            player2StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player2OriginalY_TB = 1000.0 + (player2TargetY_TB - 1000.0) * (elapsedTime / duration);
        } else {
            player2OriginalY_TB = player2TargetY_TB;
            player2ReachedTarget_TB = true;
            player2StartTime = 0;
            playersAtWaitPosition_TB = true;
        }
    } else if (player2ReachedTarget_TB && player1ReachedTarget1_TB && player3ReachedTarget1_TB && !player2BackAtOriginal_TB) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player2OriginalY_TB = player2TargetY_TB + (1000.0 - player2TargetY_TB) * (elapsedTime / duration1);
        } else {
            player2OriginalY_TB = 1000.0;
            player2BackAtOriginal_TB = true;
            player2ReturnStartTime = 0;
            readyToMoveTogether_TB = true;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 1 motion


         if (playersAtWaitPosition_TB && !player1ReachedTarget1_TB) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TB = 1100.0 + (player1TargetY1_TB - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TB = 310.0 + (player1TargetX1_TB - 310.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TB = player1TargetY1_TB;
            player1OriginalX_TB = player1TargetX1_TB;
            player1ReachedTarget1_TB = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TB && !player1ReachedTarget2_TB) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TB = player1TargetY1_TB + (player1TargetY2_TB - player1TargetY1_TB) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TB = player1TargetY2_TB;
            player1ReachedTarget2_TB = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TB  && !player1BackAtOriginal_TB && player2BackAtOriginal_TB) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TB = player1TargetY2_TB + (1100.0 - player1TargetY2_TB) * (elapsedTime / duration4);
            player1OriginalX_TB = player1TargetX1_TB + (310.0 - player1TargetX1_TB) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TB = 1100.0;
            player1OriginalX_TB = 310.0;
            player1BackAtOriginal_TB = true;
            player1ReturnStartTime = 0;
        }
    }



//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion

    if (playersAtWaitPosition_TB && !player3ReachedTarget1_TB) {
        // moving to the first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TB = 1100.0 + (player3TargetY_TB - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TB = 440.0 + (player3TargetX_TB - 440.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TB = player3TargetY_TB;
            player3OriginalX_TB = player3TargetX_TB;
            player3ReachedTarget1_TB = true;
            player3StartTime1 = 0;
        }
    } else if ( player3ReachedTarget1_TB && !player3ReachedTarget2_TB) {
        // moving to the second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TB = player3TargetY_TB + (player3TargetY2_TB - player3TargetY_TB) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TB = player3TargetY2_TB;
            player3ReachedTarget2_TB = true;
            player3StartTime2 = 0;
        }
    } else if (player3ReachedTarget2_TB && !player3BackAtOriginal_TB && player2BackAtOriginal_TB) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TB = player3TargetY2_TB + (1100.0 - player3TargetY2_TB) * (elapsedTime / duration4);
            player3OriginalX_TB = player3TargetX_TB + (440.0 - player3TargetX_TB) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TB = 1100.0;
            player3OriginalX_TB = 440.0;
            player3BackAtOriginal_TB = true;
            player3ReturnStartTime = 0;
        }
    }



    drawLine1(375.0, 1150.0, 375.0, player2OriginalY_TB);
    player(player1OriginalX_TB, player1OriginalY_TB, 2,energyStr);
    player(player2OriginalX_TB, player2OriginalY_TB, 2, energyStr2);
    player(player3OriginalX_TB, player3OriginalY_TB, 2, energyStr3);
    playerNo(player1OriginalX_TB, player1OriginalY_TB, "1");
    playerNo(player2OriginalX_TB, player2OriginalY_TB, "2");
    playerNo(player3OriginalX_TB, player3OriginalY_TB, "3");





    if (!player1BackAtOriginal_TB || !player2BackAtOriginal_TB || !player3BackAtOriginal_TB) {
        glutPostRedisplay();
    }
}







//////////////////////////////motion player 2 is Jumper  from team C///////////////////////////////////////////////////////////////////



void motion_team_C_player2_jumper(float jumpTime , float osciTime , float pullTime ,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC) {
    static float player1OriginalX_TC = 560.0, player1OriginalY_TC = 1100.0;
    static float player1TargetX1_TC = 625.0, player1TargetY1_TC = 1000.0;
    static float player1TargetY2_TC = 1150.0;
    static float player2OriginalX_TC = 625.0, player2OriginalY_TC = 1000.0;
    static float player2TargetY_TC = 600.0;
    static float player3OriginalX_TC = 690.0, player3OriginalY_TC = 1100.0;
    static float player3TargetX_TC = 625.0, player3TargetY_TC = 1000.0;
    static float player3TargetY2_TC = 1150.0;

    // flags
    static bool player1ReachedTarget1_TC = false, player1ReachedTarget2_TC = false, player1BackAtOriginal_TC = false;
    static bool player2ReachedTarget_TC = false, player2BackAtOriginal_TC = false;
    static bool player3ReachedTarget1_TC = false, player3ReachedTarget2_TC = false, player3BackAtOriginal_TC = false;
    static bool playersAtWaitPosition_TC = false;
    static bool readyToMoveTogether_TC = false;
    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player2StartTime = 0, player2ReturnStartTime = 0;
    static clock_t player3StartTime1 = 0, player3StartTime2 = 0, player3ReturnStartTime = 0;



    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;
    char energy1 = healthplayer1_TC;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TC);

     char energy2 = healthplayer2_TC;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TC);

     char energy3 = healthplayer3_TC;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TC);


    //////////////////////////////////////////////////////////////////////////////////
     //player 2 (jumper) motion

    if (!player2ReachedTarget_TC) {
        // moving to the target position
        if (player2StartTime == 0) {
            player2StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player2OriginalY_TC = 1000.0 + (player2TargetY_TC - 1000.0) * (elapsedTime / duration);
        } else {
            player2OriginalY_TC = player2TargetY_TC;
            player2ReachedTarget_TC = true;
            player2StartTime = 0;
            playersAtWaitPosition_TC = true;
        }
    } else if (player2ReachedTarget_TC && player1ReachedTarget1_TC && player3ReachedTarget1_TC && !player2BackAtOriginal_TC) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player2OriginalY_TC = player2TargetY_TC + (1000.0 - player2TargetY_TC) * (elapsedTime / duration1);
        } else {
            player2OriginalY_TC = 1000.0;
            player2BackAtOriginal_TC = true;
            player2ReturnStartTime = 0;
            readyToMoveTogether_TC = true;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////
     //player 1 motion

    if (playersAtWaitPosition_TC && !player1ReachedTarget1_TC) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TC = 1100.0 + (player1TargetY1_TC - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TC = 560.0 + (player1TargetX1_TC - 560.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TC = player1TargetY1_TC;
            player1OriginalX_TC = player1TargetX1_TC;
            player1ReachedTarget1_TC = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TC && !player1ReachedTarget2_TC) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TC = player1TargetY1_TC + (player1TargetY2_TC - player1TargetY1_TC) * (elapsedTime / duration3);

        } else {
            player1OriginalY_TC = player1TargetY2_TC;
            player1ReachedTarget2_TC = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TC && !player1BackAtOriginal_TC && player2BackAtOriginal_TC) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TC = player1TargetY2_TC + (1100.0 - player1TargetY2_TC) * (elapsedTime / duration4);
            player1OriginalX_TC = player1TargetX1_TC + (560.0 - player1TargetX1_TC) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TC = 1100.0;
            player1OriginalX_TC = 560.0;
            player1BackAtOriginal_TC = true;
            player1ReturnStartTime = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////
     //player 3 motion

    if (playersAtWaitPosition_TC && !player3ReachedTarget1_TC) {
        // moving to the first target
        if (player3StartTime1 == 0) {
            player3StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TC = 1100.0 + (player3TargetY_TC - 1100.0) * (elapsedTime / duration3);
            player3OriginalX_TC = 690.0 + (player3TargetX_TC - 690.0) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TC = player3TargetY_TC;
            player3OriginalX_TC = player3TargetX_TC;
            player3ReachedTarget1_TC = true;
            player3StartTime1 = 0;
        }
    } else if ( player3ReachedTarget1_TC && !player3ReachedTarget2_TC) {
        // moving to the second target
        if (player3StartTime2 == 0) {
            player3StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player3OriginalY_TC = player3TargetY_TC + (player3TargetY2_TC - player3TargetY_TC) * (elapsedTime / duration3);
        } else {
            player3OriginalY_TC = player3TargetY2_TC;
            player3ReachedTarget2_TC = true;
            player3StartTime2 = 0;
        }
    } else if (player3ReachedTarget2_TC &&  !player3BackAtOriginal_TC && player2BackAtOriginal_TC) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player3OriginalY_TC = player3TargetY2_TC + (1100.0 - player3TargetY2_TC) * (elapsedTime / duration4);
            player3OriginalX_TC = player3TargetX_TC + (690.0 - player3TargetX_TC) * (elapsedTime / duration4);
        } else {
            player3OriginalY_TC = 1100.0;
            player3OriginalX_TC = 690.0;
            player3BackAtOriginal_TC = true;
            player3ReturnStartTime = 0;
        }
    }

    drawLine1(625.0, 1150.0, 625.0, player2OriginalY_TC);
    player(player1OriginalX_TC, player1OriginalY_TC, 3, energyStr);
    player(player2OriginalX_TC, player2OriginalY_TC, 3, energyStr2);
    player(player3OriginalX_TC, player3OriginalY_TC, 3, energyStr3);
    playerNo(player1OriginalX_TC, player1OriginalY_TC, "1");
    playerNo(player2OriginalX_TC, player2OriginalY_TC, "2");
    playerNo(player3OriginalX_TC, player3OriginalY_TC, "3");



    if (!player1BackAtOriginal_TC || !player2BackAtOriginal_TC || !player3BackAtOriginal_TC) {
        glutPostRedisplay();
    }
}






//////////////////////////////motion player 3 is Jumper  from team A///////////////////////////////////////////////////////////////////




void motion_team_A_player3_jumper(float jumpTime , float osciTime , float pullTime , int healthplayer1_TA,int healthplayer2_TA ,int healthplayer3_TA) {


    static float player1OriginalX_TA = 60.0, player1OriginalY_TA = 1100.0;
    static float player1TargetX1_TA = 125.0, player1TargetY1_TA = 1000.0;
    static float player1TargetY2_TA = 1150.0;
    static float player3OriginalX_TA = 125.0, player3OriginalY_TA = 1000.0;
    static float player3TargetY_TA = 600.0;
    static float player2OriginalX_TA = 190.0, player2OriginalY_TA = 1100.0;
    static float player2TargetX_TA = 125.0, player2TargetY_TA = 1000.0;
    static float player2TargetY2_TA = 1150.0;

    // flags
    static bool player1ReachedTarget1_TA = false, player1ReachedTarget2_TA = false, player1BackAtOriginal_TA = false;
    static bool player3ReachedTarget_TA = false, player3BackAtOriginal_TA = false;
    static bool player2ReachedTarget1_TA = false, player2ReachedTarget2_TA = false, player2BackAtOriginal_TA = false;
    static bool playersAtWaitPosition_TA = false;
    static bool readyToMoveTogether_TA = false;
    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player3StartTime = 0, player3ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;

    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;

    char energy1 = healthplayer1_TA;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TA);

     char energy2 = healthplayer2_TA;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TA);

     char energy3 = healthplayer3_TA;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TA);





//////////////////////////////////////////////////////////////////////////////////
     //player 3(jump)  motion

         if (!player3ReachedTarget_TA) {
        // moving to the target position
        if (player3StartTime == 0) {
            player3StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player3OriginalY_TA = 1000.0 + (player3TargetY_TA - 1000.0) * (elapsedTime / duration);
        } else {
            player3OriginalY_TA = player3TargetY_TA;
            player3ReachedTarget_TA = true;
            player3StartTime = 0;
            playersAtWaitPosition_TA = true;
        }
    } else if (player3ReachedTarget_TA && player1ReachedTarget1_TA && player2ReachedTarget1_TA && !player3BackAtOriginal_TA) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player3OriginalY_TA = player3TargetY_TA + (1000.0 - player3TargetY_TA) * (elapsedTime / duration1);
        } else {
            player3OriginalY_TA = 1000.0;
            player3BackAtOriginal_TA = true;
            player3ReturnStartTime = 0;
            readyToMoveTogether_TA = true;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 1 motion

         if (playersAtWaitPosition_TA && !player1ReachedTarget1_TA) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TA = 1100.0 + (player1TargetY1_TA - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TA = 60.0 + (player1TargetX1_TA - 60.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TA = player1TargetY1_TA;
            player1OriginalX_TA = player1TargetX1_TA;
            player1ReachedTarget1_TA = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TA && !player1ReachedTarget2_TA) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TA = player1TargetY1_TA + (player1TargetY2_TA - player1TargetY1_TA) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TA = player1TargetY2_TA;
            player1ReachedTarget2_TA = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TA &&  !player1BackAtOriginal_TA && player3BackAtOriginal_TA) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TA = player1TargetY2_TA + (1100.0 - player1TargetY2_TA) * (elapsedTime / duration4);
            player1OriginalX_TA = player1TargetX1_TA + (60.0 - player1TargetX1_TA) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TA = 1100.0;
            player1OriginalX_TA = 60.0;
            player1BackAtOriginal_TA = true;
            player1ReturnStartTime = 0;
        }
    }

//////////////////////////////////////////////////////////////////////////////////
     //player 2 motion

         if (playersAtWaitPosition_TA && !player2ReachedTarget1_TA) {
        // moving to the first target
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TA = 1100.0 + (player2TargetY_TA - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TA = 190.0 + (player2TargetX_TA - 190.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TA = player2TargetY_TA;
            player2OriginalX_TA = player2TargetX_TA;
            player2ReachedTarget1_TA = true;
            player2StartTime1 = 0;
        }
    } else if ( player2ReachedTarget1_TA && !player2ReachedTarget2_TA) {
        // moving to the second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TA = player2TargetY_TA + (player2TargetY2_TA - player2TargetY_TA) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TA = player2TargetY2_TA;
            player2ReachedTarget2_TA = true;
            player2StartTime2 = 0;
        }
    } else if (player2ReachedTarget2_TA && !player2BackAtOriginal_TA && player3BackAtOriginal_TA) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TA = player2TargetY2_TA + (1100.0 - player2TargetY2_TA) * (elapsedTime / duration4);
            player2OriginalX_TA = player2TargetX_TA + (190.0 - player2TargetX_TA) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TA = 1100.0;
            player2OriginalX_TA = 190.0;
            player2BackAtOriginal_TA = true;
            player2ReturnStartTime = 0;
        }
    }







    drawLine1(125.0, 1150.0, 125.0, player3OriginalY_TA);
    player(player1OriginalX_TA, player1OriginalY_TA, 1, energyStr);
    player(player2OriginalX_TA, player2OriginalY_TA, 1, energyStr2);
    player(player3OriginalX_TA, player3OriginalY_TA, 1, energyStr3);
    playerNo(player1OriginalX_TA, player1OriginalY_TA, "1");
    playerNo(player2OriginalX_TA, player2OriginalY_TA, "2");
    playerNo(player3OriginalX_TA, player3OriginalY_TA, "3");

    if (!player1BackAtOriginal_TA || !player2BackAtOriginal_TA || !player3BackAtOriginal_TA) {
        glutPostRedisplay();
    }
}









//////////////////////////////motion player 3 is Jumper  from team B///////////////////////////////////////////////////////////////////




void motion_team_B_player3_jumper(float jumpTime , float osciTime , float pullTime,int healthplayer1_TB,int healthplayer2_TB ,int healthplayer3_TB) {
    // Position and motion settings for Team B after swap
    static float player1OriginalX_TB = 310.0, player1OriginalY_TB = 1100.0;
    static float player1TargetX1_TB = 375.0, player1TargetY1_TB = 1000.0;
    static float player1TargetY2_TB = 1150.0;
    static float player3OriginalX_TB = 375.0, player3OriginalY_TB = 1000.0;
    static float player3TargetY_TB = 600.0;
    static float player2OriginalX_TB = 440.0, player2OriginalY_TB = 1100.0;
    static float player2TargetX_TB = 375.0, player2TargetY_TB = 1000.0;
    static float player2TargetY2_TB = 1150.0;

    // flags
    static bool player1ReachedTarget1_TB = false, player1ReachedTarget2_TB = false, player1BackAtOriginal_TB = false;
    static bool player3ReachedTarget_TB = false, player3BackAtOriginal_TB = false;
    static bool player2ReachedTarget1_TB = false, player2ReachedTarget2_TB = false, player2BackAtOriginal_TB = false;
    static bool playersAtWaitPosition_TB = false;
    static bool readyToMoveTogether_TB = false;
    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player3StartTime = 0, player3ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;

    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;

    char energy1 = healthplayer1_TB;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TB);

     char energy2 = healthplayer2_TB;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TB);

     char energy3 = healthplayer3_TB;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TB);

//////////////////////////////////////////////////////////////////////////////////
     //player 3 motion (jumper)

    if (!player3ReachedTarget_TB) {
        // moving to the target position
        if (player3StartTime == 0) {
            player3StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player3OriginalY_TB = 1000.0 + (player3TargetY_TB - 1000.0) * (elapsedTime / duration);
        } else {
            player3OriginalY_TB = player3TargetY_TB;
            player3ReachedTarget_TB = true;
            player3StartTime = 0;
            playersAtWaitPosition_TB = true;
        }
    } else if (player3ReachedTarget_TB && player1ReachedTarget1_TB && player2ReachedTarget1_TB && !player3BackAtOriginal_TB) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player3OriginalY_TB = player3TargetY_TB + (1000.0 - player3TargetY_TB) * (elapsedTime / duration1);
        } else {
            player3OriginalY_TB = 1000.0;
            player3BackAtOriginal_TB = true;
            player3ReturnStartTime = 0;
            readyToMoveTogether_TB = true;
        }
    }

////////////////////////////////////////////////////////////////////////////////////////
     //player 1 motion

    if (playersAtWaitPosition_TB && !player1ReachedTarget1_TB) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TB = 1100.0 + (player1TargetY1_TB - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TB = 310.0 + (player1TargetX1_TB - 310.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TB = player1TargetY1_TB;
            player1OriginalX_TB = player1TargetX1_TB;
            player1ReachedTarget1_TB = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TB && !player1ReachedTarget2_TB) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TB = player1TargetY1_TB + (player1TargetY2_TB - player1TargetY1_TB) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TB = player1TargetY2_TB;
            player1ReachedTarget2_TB = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TB &&  !player1BackAtOriginal_TB && player3BackAtOriginal_TB) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TB = player1TargetY2_TB + (1100.0 - player1TargetY2_TB) * (elapsedTime / duration4);
            player1OriginalX_TB = player1TargetX1_TB + (310.0 - player1TargetX1_TB) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TB = 1100.0;
            player1OriginalX_TB = 310.0;
            player1BackAtOriginal_TB = true;
            player1ReturnStartTime = 0;
        }
    }



//////////////////////////////////////////////////////////////////////////////////
     //player 2 motion
    if (playersAtWaitPosition_TB && !player2ReachedTarget1_TB) {
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TB = 1100.0 + (player2TargetY_TB - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TB = 440.0 + (player2TargetX_TB - 440.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TB = player2TargetY_TB;
            player2OriginalX_TB = player2TargetX_TB;
            player2ReachedTarget1_TB = true;
            player2StartTime1 = 0;
        }
    } else if ( player2ReachedTarget1_TB && !player2ReachedTarget2_TB) {
        // moving to the second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TB = player2TargetY_TB + (player2TargetY2_TB - player2TargetY_TB) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TB = player2TargetY2_TB;
            player2ReachedTarget2_TB = true;
            player2StartTime2 = 0;
        }
    } else if (player2ReachedTarget2_TB &&  !player2BackAtOriginal_TB && player3BackAtOriginal_TB) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TB = player2TargetY2_TB + (1100.0 - player2TargetY2_TB) * (elapsedTime / duration4);
            player2OriginalX_TB = player2TargetX_TB + (440.0 - player2TargetX_TB) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TB = 1100.0;
            player2OriginalX_TB = 440.0;
            player2BackAtOriginal_TB = true;
            player2ReturnStartTime = 0;
        }
    }





    drawLine1(375.0, 1150.0, 375.0, player3OriginalY_TB);
    player(player1OriginalX_TB, player1OriginalY_TB, 2, energyStr);
    player(player2OriginalX_TB, player2OriginalY_TB, 2, energyStr2);
    player(player3OriginalX_TB, player3OriginalY_TB, 2, energyStr3);
    playerNo(player1OriginalX_TB, player1OriginalY_TB, "1");
    playerNo(player2OriginalX_TB, player2OriginalY_TB, "2");
    playerNo(player3OriginalX_TB, player3OriginalY_TB, "3");



    if (!player1BackAtOriginal_TB || !player2BackAtOriginal_TB || !player3BackAtOriginal_TB) {
        glutPostRedisplay();
    }
}


//////////////////////////////motion player 3 is Jumper  from team C///////////////////////////////////////////////////////////////////

void motion_team_C_player3_jumper(float jumpTime , float osciTime , float pullTime,int healthplayer1_TC,int healthplayer2_TC ,int healthplayer3_TC ) {
    // Position and motion settings for Team C after swapping Player 2 and Player 3
    static float player1OriginalX_TC = 560.0, player1OriginalY_TC = 1100.0;
    static float player1TargetX1_TC = 625.0, player1TargetY1_TC = 1000.0;
    static float player1TargetY2_TC = 1150.0;
    static float player3OriginalX_TC = 625.0, player3OriginalY_TC = 1000.0;
    static float player3TargetY_TC = 600.0;
    static float player2OriginalX_TC = 690.0, player2OriginalY_TC = 1100.0;
    static float player2TargetX_TC = 625.0, player2TargetY_TC = 1000.0;
    static float player2TargetY2_TC = 1150.0;

    // flags
    static bool player1ReachedTarget1_TC = false, player1ReachedTarget2_TC = false, player1BackAtOriginal_TC = false;
    static bool player3ReachedTarget_TC = false, player3BackAtOriginal_TC = false;
    static bool player2ReachedTarget1_TC = false, player2ReachedTarget2_TC = false, player2BackAtOriginal_TC = false;
    static bool playersAtWaitPosition_TC = false;
    static bool readyToMoveTogether_TC = false;
    static clock_t player1StartTime1 = 0, player1StartTime2 = 0, player1ReturnStartTime = 0;
    static clock_t player3StartTime = 0, player3ReturnStartTime = 0;
    static clock_t player2StartTime1 = 0, player2StartTime2 = 0, player2ReturnStartTime = 0;


    // jump time / pull time / oscillation time
    float duration = jumpTime;
    float duration1 = pullTime;
    float duration3 = osciTime;
    float duration4 = 0.05;
    char energy1 = healthplayer1_TC;
    char energyStr[10];
    sprintf(energyStr, "%d%%", healthplayer1_TC);

     char energy2 = healthplayer2_TC;
    char energyStr2[10];
    sprintf(energyStr2, "%d%%", healthplayer2_TC);

     char energy3 = healthplayer3_TC;
    char energyStr3[10];
    sprintf(energyStr3, "%d%%", healthplayer3_TC);

    ///////////////////////////////////////////////////////////////////////////
    //player 3 motion (jumper)

    if (!player3ReachedTarget_TC) {
        // Moving to the target position
        if (player3StartTime == 0) {
            player3StartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3StartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration) {
            player3OriginalY_TC = 1000.0 + (player3TargetY_TC - 1000.0) * (elapsedTime / duration);
        } else {
            player3OriginalY_TC = player3TargetY_TC;
            player3ReachedTarget_TC = true;
            player3StartTime = 0;
            playersAtWaitPosition_TC = true;
        }
    } else if (player3ReachedTarget_TC && player1ReachedTarget1_TC && player2ReachedTarget1_TC && !player3BackAtOriginal_TC) {
        // returning to the original position
        if (player3ReturnStartTime == 0) {
            player3ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player3ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration1) {
            player3OriginalY_TC = player3TargetY_TC + (1000.0 - player3TargetY_TC) * (elapsedTime / duration1);
        } else {
            player3OriginalY_TC = 1000.0;
            player3BackAtOriginal_TC = true;
            player3ReturnStartTime = 0;
            readyToMoveTogether_TC = true;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////
    //player 1 motion

    if (playersAtWaitPosition_TC && !player1ReachedTarget1_TC) {
        // moving to the first target
        if (player1StartTime1 == 0) {
            player1StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TC = 1100.0 + (player1TargetY1_TC - 1100.0) * (elapsedTime / duration3);
            player1OriginalX_TC = 560.0 + (player1TargetX1_TC - 560.0) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TC = player1TargetY1_TC;
            player1OriginalX_TC = player1TargetX1_TC;
            player1ReachedTarget1_TC = true;
            player1StartTime1 = 0;
        }
    } else if ( player1ReachedTarget1_TC && !player1ReachedTarget2_TC) {
        // moving to the second target
        if (player1StartTime2 == 0) {
            player1StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player1StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player1OriginalY_TC = player1TargetY1_TC + (player1TargetY2_TC - player1TargetY1_TC) * (elapsedTime / duration3);
        } else {
            player1OriginalY_TC = player1TargetY2_TC;
            player1ReachedTarget2_TC = true;
            player1StartTime2 = 0;
        }
    } else if (player1ReachedTarget2_TC &&  !player1BackAtOriginal_TC && player3BackAtOriginal_TC) {
        // returning to the original position
        if (player1ReturnStartTime == 0) {
            player1ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player1ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player1OriginalY_TC = player1TargetY2_TC + (1100.0 - player1TargetY2_TC) * (elapsedTime / duration4);
            player1OriginalX_TC = player1TargetX1_TC + (560.0 - player1TargetX1_TC) * (elapsedTime / duration4);
        } else {
            player1OriginalY_TC = 1100.0;
            player1OriginalX_TC = 560.0;
            player1BackAtOriginal_TC = true;
            player1ReturnStartTime = 0;
        }
    }
//////////////////////////////////////////////////////////////////
    //player 2 motion
    if (playersAtWaitPosition_TC && !player2ReachedTarget1_TC) {
        // Moving to the first target
        if (player2StartTime1 == 0) {
            player2StartTime1 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime1) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TC = 1100.0 + (player2TargetY_TC - 1100.0) * (elapsedTime / duration3);
            player2OriginalX_TC = 690.0 + (player2TargetX_TC - 690.0) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TC = player2TargetY_TC;
            player2OriginalX_TC = player2TargetX_TC;
            player2ReachedTarget1_TC = true;
            player2StartTime1 = 0;
        }
    } else if ( player2ReachedTarget1_TC && !player2ReachedTarget2_TC) {
        // Moving to the second target
        if (player2StartTime2 == 0) {
            player2StartTime2 = clock();
        }
        float elapsedTime = (float)(clock() - player2StartTime2) / CLOCKS_PER_SEC;
        if (elapsedTime < duration3) {
            player2OriginalY_TC = player2TargetY_TC + (player2TargetY2_TC - player2TargetY_TC) * (elapsedTime / duration3);
        } else {
            player2OriginalY_TC = player2TargetY2_TC;
            player2ReachedTarget2_TC = true;
            player2StartTime2 = 0;
        }
    } else if (player2ReachedTarget2_TC &&  !player2BackAtOriginal_TC && player3BackAtOriginal_TC) {
        // returning to the original position
        if (player2ReturnStartTime == 0) {
            player2ReturnStartTime = clock();
        }
        float elapsedTime = (float)(clock() - player2ReturnStartTime) / CLOCKS_PER_SEC;
        if (elapsedTime < duration4) {
            player2OriginalY_TC = player2TargetY2_TC + (1100.0 - player2TargetY2_TC) * (elapsedTime / duration4);
            player2OriginalX_TC = player2TargetX_TC + (690.0 - player2TargetX_TC) * (elapsedTime / duration4);
        } else {
            player2OriginalY_TC = 1100.0;
            player2OriginalX_TC = 690.0;
            player2BackAtOriginal_TC = true;
            player2ReturnStartTime = 0;
        }
    }







    drawLine1(625.0, 1150.0, 625.0, player3OriginalY_TC);



    player(player1OriginalX_TC, player1OriginalY_TC, 3, energyStr);
    player(player2OriginalX_TC, player2OriginalY_TC, 3, energyStr2);
    player(player3OriginalX_TC, player3OriginalY_TC, 3, energyStr3);
    playerNo(player1OriginalX_TC, player1OriginalY_TC, "1");
    playerNo(player2OriginalX_TC, player2OriginalY_TC, "2");
    playerNo(player3OriginalX_TC, player3OriginalY_TC, "3");


    if (!player1BackAtOriginal_TC || !player2BackAtOriginal_TC || !player3BackAtOriginal_TC) {
        glutPostRedisplay();
    }
}