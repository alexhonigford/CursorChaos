// authors: Alex Honigford and Jacob Cohn (Group C2)

// importing necessary libraries
#include <FEHLCD.h>
#include <FEHRandom.h>
#include <FEHUtility.h>
#include <math.h>

// function prototypes
void displayMainMenu();
void displayModes(); 
void playGame(); 
void init(); 
void initMode(); 
void initBeginnerMode(); 
void initEasyMode(); 
void initNormalMode(); 
void initHardMode(); 
void initImpossibleMode(); 
void initTrackingBall(); 
void initShootingBalls(); 
int getIntBetweenXAndY(int, int); 
void initFollowingBalls(); 
void initScore();
void updateGame(); 
void updateTrackingBall(); 
void updateShootingBalls(); 
void updateFollowingBalls(); 
void updateScore(); 
float getDistanceFromTracking(float, float); 
void displayGame(); 
void displayTrackingBall(); 
void displayShootingBalls(); 
void displayFollowingBalls(); 
void displayScoreInUpperLeftCorner(); 
bool isGameOver(); 
bool isGameOverFromWalls(); 
bool isGameOverFromShootingBalls(); 
bool isGameOverFromFollowingBalls(); 
void updateHighScores(); 
void displayGameOver(); 
void displayBackButton(); 
void displayHighScores(); 
void displayInstructions(); 
void displayCredits(); 

// global variables used in functions
const int MAX_SHOOTING_BALLS = 20;
const int MAX_FOLLOWING_BALLS = 20;
const int X_BORDER_MIN = 0;
const int X_BORDER_MAX = 319;
const int Y_BORDER_MIN = 0;
const int Y_BORDER_MAX = 239;
const int SPAWN_BUFFER_FROM_WALLS = 10;

int mode;
const int BEGINNER = 0;
const int EASY = 1;
const int NORMAL = 2;
const int HARD = 3;
const int IMPOSSIBLE = 4;
const int NUM_MODES = 5;
float highScores[NUM_MODES] = {0, 0, 0, 0, 0};
char modes[5][20] = {"BEGINNER", "EASY", "NORMAL", "HARD", "IMPOSSIBLE"};

float start;
float score;

// tracking ball struct using coordinates and a radius
struct TrackingBall {
  float x;
  float y;
  float radius;
};

// shooting ball struct using coordinates, a speed vector, its radius, and its color as a data members
struct ShootingBall {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
  float radius;
  float color;
};

// following ball struct using coordinates, a speed, its radius, and its color as a data members
struct FollowingBall {
  float x;
  float y;
  float speed;
  float radius;
  float color;
};

// struct containing given mode's information
struct ModeInfo {
  int numShootingBalls;
  int numFollowingBalls;
  int trackingBallRadius;
  int noSpawnRadius;
  int maxShootingBallSpeed;
  int minShootingBallRadius;
  int maxShootingBallRadius;
  int maxFollowingBallSpeed;
  int minFollowingBallSpeed;
  int minFollowingBallRadius;
  int maxFollowingBallRadius;
  int followingWithinDistance;
};

// structs of the balls
TrackingBall trackingBall;
ShootingBall shootingBalls[MAX_SHOOTING_BALLS];
FollowingBall followingBalls[MAX_FOLLOWING_BALLS];
ModeInfo modeInfo;

// calls the display main menu function in the main which has all of the functions nested inside of it
int main() {
  displayMainMenu();
  // constant refresh
  while (true) {
    LCD.Update();
  }

  return 0;
}

// displays the 4 menu options with a header displaying the game name
void displayMainMenu() {
  // game header
  LCD.Clear(BLACK);
  LCD.WriteRC("CURSOR CHAOS", 1, 7);

  // char array used for the text in the boxes
  FEHIcon::Icon mainMenu[4];
  char label[4][20] = {"PLAY", "HIGH SCORES", "INSTRUCTIONS", "CREDITS"};
  FEHIcon::DrawIconArray(mainMenu, 4, 1, 60, 20, 60, 60, label, WHITE, WHITE);

  int x, y;
  while (true) {
    // waiting for a click
    while (LCD.Touch(&x, &y)) {};
    while (!LCD.Touch(&x, &y)) {};

    // checks where the touch was made and if it ends in the inteded rectangle, calling the necessary function
    if (mainMenu[0].Pressed(x, y, 0)) {
      displayModes();
      break;
    } else if (mainMenu[1].Pressed(x, y, 0)) {
      displayHighScores();
      break;
    } else if (mainMenu[2].Pressed(x, y, 0)) {
      displayInstructions();
      break;
    } else if (mainMenu[3].Pressed(x, y, 0)) {
      displayCredits();
      break;
    }
  }
}

// displays 5 menu options to choose which mode to pick
void displayModes() {
  LCD.Clear(BLACK);

  FEHIcon::Icon modeIcons[5];
  FEHIcon::DrawIconArray(modeIcons, 5, 1, 20, 20, 60, 60, modes, WHITE, WHITE);

  int x, y;
  while (true) {
    // waiting for a click
    while (LCD.Touch(&x, &y)) {};
    while (!LCD.Touch(&x, &y)) {};

    // checks where the touch was made and if it ends in the inteded rectangle, setting the correct mode
    if (modeIcons[0].Pressed(x, y, 0)) {
      mode = BEGINNER;
    } else if (modeIcons[1].Pressed(x, y, 0)) {
      mode = EASY;
    } else if (modeIcons[2].Pressed(x, y, 0)) {
      mode = NORMAL;
    } else if (modeIcons[3].Pressed(x, y, 0)) {
      mode = HARD;
    } else if (modeIcons[4].Pressed(x, y, 0)) {
      mode = IMPOSSIBLE;
    }

    playGame();
  }
}

void playGame() {
  // initializes the balls data members
  init();

  while (true) {
    // updates locations of balls and displays them
    updateGame();
    displayGame();

    // if the user gets hit or hits a wall, ends the game and updates the scores if necessary
    if (isGameOver()) {
      updateHighScores();
      break;
    }
  }
  displayGameOver();
}

// calls functions initializing the mode, score, and balls data members
void init() {
  initMode();
  initTrackingBall();
  initShootingBalls();
  initFollowingBalls();
  initScore();
}

// depending on which mode was pressed, calls its function
void initMode() {
  switch (mode) {
    case BEGINNER:
      initBeginnerMode();
      break;
    case EASY:
      initEasyMode();
      break;
    case NORMAL:
      initNormalMode();
      break;
    case HARD:
      initHardMode();
      break;
    case IMPOSSIBLE:
      initImpossibleMode();
      break;
  }
}

// initializes data members for each mode depending on whether or not it was selected
void initBeginnerMode() {
  modeInfo.numShootingBalls = 1;
  modeInfo.numFollowingBalls = 1;
  modeInfo.trackingBallRadius = 5;
  modeInfo.noSpawnRadius = 150;
  modeInfo.maxShootingBallSpeed = 3;
  modeInfo.minShootingBallRadius = 5;
  modeInfo.maxShootingBallRadius = 10;
  modeInfo.maxFollowingBallSpeed = 1;
  modeInfo.minFollowingBallSpeed = 1;
  modeInfo.minFollowingBallRadius = 5;
  modeInfo.maxFollowingBallRadius = 10;
  modeInfo.followingWithinDistance = 70;
}

void initEasyMode() {
  modeInfo.numShootingBalls = 3;
  modeInfo.numFollowingBalls = 2;
  modeInfo.trackingBallRadius = 5;
  modeInfo.noSpawnRadius = 120;
  modeInfo.maxShootingBallSpeed = 3;
  modeInfo.minShootingBallRadius = 5;
  modeInfo.maxShootingBallRadius = 15;
  modeInfo.maxFollowingBallSpeed = 1;
  modeInfo.minFollowingBallSpeed = 1;
  modeInfo.minFollowingBallRadius = 5;
  modeInfo.maxFollowingBallRadius = 10;
  modeInfo.followingWithinDistance = 80;
}

void initNormalMode() {
  modeInfo.numShootingBalls = 5;
  modeInfo.numFollowingBalls = 5;
  modeInfo.trackingBallRadius = 5;
  modeInfo.noSpawnRadius = 120;
  modeInfo.maxShootingBallSpeed = 3;
  modeInfo.minShootingBallRadius = 3;
  modeInfo.maxShootingBallRadius = 20;
  modeInfo.maxFollowingBallSpeed = 1;
  modeInfo.minFollowingBallSpeed = 1;
  modeInfo.minFollowingBallRadius = 5;
  modeInfo.maxFollowingBallRadius = 15;
  modeInfo.followingWithinDistance = 100;
}

void initHardMode() {
  modeInfo.numShootingBalls = 8;
  modeInfo.numFollowingBalls = 5;
  modeInfo.trackingBallRadius = 5;
  modeInfo.noSpawnRadius = 100;
  modeInfo.maxShootingBallSpeed = 4;
  modeInfo.minShootingBallRadius = 3;
  modeInfo.maxShootingBallRadius = 20;
  modeInfo.maxFollowingBallSpeed = 2;
  modeInfo.minFollowingBallSpeed = 1;
  modeInfo.minFollowingBallRadius = 5;
  modeInfo.maxFollowingBallRadius = 15;
  modeInfo.followingWithinDistance = 100;
}

void initImpossibleMode() {
  modeInfo.numShootingBalls = 8;
  modeInfo.numFollowingBalls = 8;
  modeInfo.trackingBallRadius = 5;
  modeInfo.noSpawnRadius = 100;
  modeInfo.maxShootingBallSpeed = 4;
  modeInfo.minShootingBallRadius = 3;
  modeInfo.maxShootingBallRadius = 20;
  modeInfo.maxFollowingBallSpeed = 2;
  modeInfo.minFollowingBallSpeed = 1;
  modeInfo.minFollowingBallRadius = 5;
  modeInfo.maxFollowingBallRadius = 15;
  modeInfo.followingWithinDistance = 100;
}

// updates location of the cursor ball and sets the radius
void initTrackingBall() {
  updateTrackingBall();
  trackingBall.radius = modeInfo.trackingBallRadius;
}

// initializes the shooting balls colors, speeds, radii, coordinates, and speeds
void initShootingBalls() {
  const int colorsLength = 3;
  int colors[colorsLength] = {BLUE, GREEN, BLUEVIOLET};

  for (int i = 0; i < modeInfo.numShootingBalls; i++) {
    do {
      shootingBalls[i].x = getIntBetweenXAndY(X_BORDER_MIN + SPAWN_BUFFER_FROM_WALLS, X_BORDER_MAX - SPAWN_BUFFER_FROM_WALLS);
      shootingBalls[i].y = getIntBetweenXAndY(Y_BORDER_MIN + SPAWN_BUFFER_FROM_WALLS, Y_BORDER_MAX - SPAWN_BUFFER_FROM_WALLS);
    } while (getDistanceFromTracking(shootingBalls[i].x, shootingBalls[i].y) <= modeInfo.noSpawnRadius);

    shootingBalls[i].xSpeed = getIntBetweenXAndY(-modeInfo.maxShootingBallSpeed, modeInfo.maxShootingBallSpeed);
    shootingBalls[i].ySpeed = getIntBetweenXAndY(-modeInfo.maxShootingBallSpeed, modeInfo.maxShootingBallSpeed);
    shootingBalls[i].radius = getIntBetweenXAndY(modeInfo.minShootingBallRadius, modeInfo.maxShootingBallRadius);
    shootingBalls[i].color = colors[getIntBetweenXAndY(0, colorsLength - 1)];
  }
}

// uses the random integer function to use random generation for the enemy balls
int getIntBetweenXAndY(int x, int y) {
  return x + Random.RandInt() % ((y - x) + 1);
}

// initializes the following balls colors, speeds, radii, coordinates, and speeds
void initFollowingBalls() {
  const int colorsLength = 3;
  int colors[colorsLength] = {RED, ORANGE, ORANGERED};

  for (int i = 0; i < modeInfo.numFollowingBalls; i++){
    do {
      followingBalls[i].x = getIntBetweenXAndY(X_BORDER_MIN + SPAWN_BUFFER_FROM_WALLS, X_BORDER_MAX - SPAWN_BUFFER_FROM_WALLS);
      followingBalls[i].y = getIntBetweenXAndY(Y_BORDER_MIN + SPAWN_BUFFER_FROM_WALLS, Y_BORDER_MAX - SPAWN_BUFFER_FROM_WALLS);
    } while (getDistanceFromTracking(followingBalls[i].x, followingBalls[i].y) <= modeInfo.noSpawnRadius);

    followingBalls[i].speed = getIntBetweenXAndY(modeInfo.minFollowingBallSpeed, modeInfo.maxFollowingBallSpeed);
    followingBalls[i].radius = getIntBetweenXAndY(modeInfo.minFollowingBallRadius, modeInfo.maxFollowingBallRadius);
    followingBalls[i].color = colors[getIntBetweenXAndY(0, colorsLength - 1)];
  }
}

// starts the timer
void initScore() {
  start = TimeNow();
  updateScore();
}

// calls all of the necessary update functions to update the screen
void updateGame() {
  updateTrackingBall();
  updateShootingBalls();
  updateFollowingBalls();
  updateScore();
}

// tracks the cursor to the ball
void updateTrackingBall() {
  LCD.Touch(&trackingBall.x, &trackingBall.y);
}

// uses speeds to update location of the shooting balls
void updateShootingBalls() {
  for (int i = 0; i < modeInfo.numShootingBalls; i++) {
    shootingBalls[i].x += shootingBalls[i].xSpeed;
    shootingBalls[i].y += shootingBalls[i].ySpeed;
   
    // wrap around the x axis
    if (shootingBalls[i].x <= X_BORDER_MIN){
      shootingBalls[i].x = X_BORDER_MAX;
    } else if (shootingBalls[i].x >= X_BORDER_MAX){
      shootingBalls[i].x = X_BORDER_MIN;
    }

    // wrap around the y axis
    if (shootingBalls[i].y <= Y_BORDER_MIN){
      shootingBalls[i].y = Y_BORDER_MAX;
    } else if (shootingBalls[i].y >= Y_BORDER_MAX){
      shootingBalls[i].y = Y_BORDER_MIN;
    }
  }
}

// updates the location of the following balls using trigonometry
void updateFollowingBalls() {
  for (int i = 0; i < modeInfo.numFollowingBalls; i++) {
    float distance = getDistanceFromTracking(followingBalls[i].x, followingBalls[i].y);

    if (distance < modeInfo.followingWithinDistance) {
      float xDistance = trackingBall.x - followingBalls[i].x;
      float yDistance = trackingBall.y - followingBalls[i].y;
      float theta = atan2(yDistance, xDistance);

      float xSpeed = cos(theta) * followingBalls[i].speed;
      float ySpeed = sin(theta) * followingBalls[i].speed;

      followingBalls[i].x += xSpeed;
      followingBalls[i].y += ySpeed;
    }
  }
}

// gives the current score using relative timing, the score is the runnning time
void updateScore() {
  score = TimeNow() - start;
}

// uses the distance equation to get the distance
float getDistanceFromTracking(float x, float y) {
  float xDistance = trackingBall.x - x;
  float yDistance = trackingBall.y - y;
  float distance = sqrt(xDistance * xDistance + yDistance * yDistance);
  return distance;
}

// calls the boolean-return functions to determine if the game is over
bool isGameOver() {
  if (isGameOverFromWalls()) return true;
  if (isGameOverFromShootingBalls()) return true;
  if (isGameOverFromFollowingBalls()) return true;

  return false;
}

// if the cursor hits the sides of the screen (walls), the game is over
bool isGameOverFromWalls() {
  bool wallFound = false;
  if (trackingBall.x <= X_BORDER_MIN || trackingBall.x >= X_BORDER_MAX){
    wallFound = true;
  }
  if (trackingBall.y <= Y_BORDER_MIN || trackingBall.y >= Y_BORDER_MAX){
    wallFound = true;
  }
  return wallFound;
}

// if a shooting ball makes contact with the cursor ball, game is over
bool isGameOverFromShootingBalls() {
  for (int i = 0; i < modeInfo.numShootingBalls;  i++) {
    if (getDistanceFromTracking(shootingBalls[i].x, shootingBalls[i].y) <= (shootingBalls[i].radius + trackingBall.radius)) {
      return true;
    }
  }
  return false;
}

// if a following ball makes contact with the cursor ball, game is over
bool isGameOverFromFollowingBalls() {
  for (int i = 0; i < modeInfo.numFollowingBalls; i++) {
    if (getDistanceFromTracking(followingBalls[i].x, followingBalls[i].y) <= (followingBalls[i].radius + trackingBall.radius)) {
      return true;
    }
  }
  return false;
}

// if the current score is higher than the mode’s high score, update the high score
void updateHighScores() {
  if (score > highScores[mode]) {
    highScores[mode] = score;
  }
}

// calls the necessary display functions to update the screen
void displayGame() {
  LCD.Clear(BLACK);
  displayTrackingBall();
  displayShootingBalls();
  displayFollowingBalls();
  displayScoreInUpperLeftCorner();
};

// displays cursor ball to the screen
void displayTrackingBall() {
  LCD.SetFontColor(YELLOW);
  LCD.FillCircle(trackingBall.x, trackingBall.y, trackingBall.radius);
}

// displays the shooting balls in the struct array
void displayShootingBalls() {
  for (int i = 0; i < modeInfo.numShootingBalls; i++) {
    LCD.SetFontColor(shootingBalls[i].color);
    LCD.FillCircle(shootingBalls[i].x, shootingBalls[i].y, shootingBalls[i].radius);
  }
}

// displays the following balls in the struct array
void displayFollowingBalls() {
  for (int i = 0; i < modeInfo.numFollowingBalls; i++) {
    LCD.SetFontColor(followingBalls[i].color);
    LCD.FillCircle(followingBalls[i].x, followingBalls[i].y, followingBalls[i].radius);
  }
}

// displays the current score using WriteRC
void displayScoreInUpperLeftCorner() {
  LCD.SetFontColor(WHITE);
  LCD.WriteRC(score, 1, 20);
}

// displays the game over text and score earned on the attempt
void displayGameOver() {
  LCD.Clear(BLACK);
  LCD.SetFontColor(ORANGERED);
  LCD.WriteRC("GAME OVER", 3, 7);
  LCD.SetFontColor(WHITE);
  LCD.WriteRC("SCORE: ", 5, 7);
  LCD.WriteRC(score, 5, 14);
  displayBackButton();
}

// displays the back button to be inputted on a lot of pages
void displayBackButton() {
  FEHIcon::Icon back;
  back.SetProperties("BACK", 20, 200, 70, 30, WHITE, WHITE);
  back.Draw();

  int x, y;
  while (true) {
    // waiting for click
    while (LCD.Touch(&x, &y)) {};
    while (!LCD.Touch(&x, &y)) {};

    if (back.Pressed(x, y, 0)) {
      displayMainMenu();
      break;
    }
  }
}

// gives the high scores for each mode
void displayHighScores() {
  LCD.Clear(BLACK);
  LCD.WriteRC("HIGH SCORES", 1, 7);

  for (int i = 0; i < NUM_MODES; i++) {
    LCD.WriteRC(modes[i], 3 + i, 5);
    LCD.WriteRC(highScores[i], 3 + i, 16);
  }
 
  displayBackButton();
}

// displays game instructions
void displayInstructions() {
  LCD.Clear(BLACK);
  LCD.WriteRC("INSTRUCTIONS", 1, 7);

  LCD.WriteRC("You are a yellow ball.", 3, 0);
  LCD.WriteRC("You move with the cursor.", 5, 0);
  LCD.WriteRC("Other balls will move.", 7, 0);
  LCD.WriteRC("Avoid them to stay alive.", 9, 0);

  displayBackButton();
}

// displays the credits
void displayCredits() {
  LCD.Clear(BLACK);
  LCD.SetFontColor(WHITE);

  LCD.WriteRC("CREDITS", 1, 10);
  LCD.WriteRC("Jacob Cohn", 4, 7);
  LCD.WriteRC("Alex Honigford", 6, 7);

  displayBackButton();
}