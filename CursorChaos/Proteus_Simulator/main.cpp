#include <FEHLCD.h>
#include <FEHUtility.h>

void displayMainMenu();
void playGame();
void displayTrackingBall();
void displayShootingBalls();
void displayBackButton();
void displayHighScores();
void displayInstructions();
void displayCredits();


int trackingBallRadius = 5;
char highScores[5][20] = {"JNC 100", "AJC 99", "SRC 97", "JAC 92", "BSC 90"};

int main() {
	displayMainMenu();

	while (true) {
		LCD.Update();
	}

	return 0;
}

void displayMainMenu() {
	LCD.Clear(BLACK);
	LCD.WriteRC("CURSOR CHAOS", 1, 7);

	FEHIcon::Icon mainMenu[4];
	char label[4][20] = {"PLAY", "HIGH SCORES", "INSTRUCTIONS", "CREDITS"};
	FEHIcon::DrawIconArray(mainMenu, 4, 1, 60, 20, 60, 60, label, WHITE, WHITE);

	float x, y;
	while (true) {
		while (LCD.Touch(&x, &y)) {};
		while (!LCD.Touch(&x, &y)) {};

		if (mainMenu[0].Pressed(x, y, 0)) {
			playGame();
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

void playGame() {
	LCD.Clear(BLACK);
	displayTrackingBall();
	displayShootingBalls();
}

void displayTrackingBall() {
	float x, y;
	while (true) {
		LCD.Touch(&x, &y);
		LCD.Clear(BLACK);
		LCD.SetFontColor(YELLOW);
		LCD.FillCircle(x, y, 5);
	}
}

void displayShootingBalls() {
	float shootingBalls[100][3];

}

void displayBackButton() {
	FEHIcon::Icon back;
	back.SetProperties("BACK", 20, 200, 70, 30, WHITE, WHITE);
	back.Draw();

	float x, y;
	while (true) {
		while (LCD.Touch(&x, &y)) {};
		while (!LCD.Touch(&x, &y)) {};

		if (back.Pressed(x, y, 0)) {
			displayMainMenu();
			break;
		}
	}
}

void displayHighScores() {
	LCD.Clear(BLACK);
	LCD.WriteRC("HIGH SCORES", 1, 7);

	LCD.WriteRC(highScores[0], 3, 9);
	LCD.WriteRC(highScores[1], 4, 9);
	LCD.WriteRC(highScores[2], 5, 9);   
	LCD.WriteRC(highScores[3], 6, 9);
	LCD.WriteRC(highScores[4], 7, 9);
	
	displayBackButton();
}

void displayInstructions() {
	LCD.Clear(BLACK);
	LCD.WriteRC("INSTRUCTIONS", 1, 7);

	LCD.WriteRC("You are a yellow ball.", 3, 0);
	LCD.WriteRC("You move with the cursor.", 5, 0);
	LCD.WriteRC("Other balls will move.", 7, 0);
	LCD.WriteRC("Avoid them to stay alive.", 9, 0);

	displayBackButton();
}

void displayCredits() {
	LCD.Clear(BLACK);

	LCD.WriteRC("CREDITS", 1, 10);
	LCD.WriteRC("Jacob Cohn", 4, 7);
	LCD.WriteRC("Alex Honigford", 6, 7);

	displayBackButton();
}
