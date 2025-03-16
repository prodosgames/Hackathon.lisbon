#include <U8glib.h>


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

#define LDR_PIN A0
#define RESET_BUTTON 2 

int dinoY = 50;
int dinoVelocity = 0;
int dinoGravity = 1;
bool isJumping = false;
bool invincible = false;
unsigned long invincibleStartTime = 0;
int jumpThreshold = 450;

int obstacleX = 128;
int obstacleSpeed = 2;

int groundOffset = 0;

void setup() {
  pinMode(LDR_PIN, INPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
  
  displayLoadingScreen();  
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);

  if (digitalRead(RESET_BUTTON) == LOW) {
    displayLoadingScreen();  
    resetGame();
    delay(500);
  }

  if (ldrValue > jumpThreshold && !isJumping) {
    isJumping = true;
    dinoVelocity = 5;
    invincible = true;
    invincibleStartTime = millis();
  }

  if (isJumping) {
    dinoY -= dinoVelocity;
    dinoVelocity -= dinoGravity;
  }

  if (dinoY >= 50) {
    dinoY = 50;
    dinoVelocity = 0;
    isJumping = false;
  }

  if (invincible && millis() - invincibleStartTime > 2000) {
    invincible = false;
  }

  obstacleX -= obstacleSpeed;
  if (obstacleX < 0) {
    obstacleX = 128;
  }
  if (!invincible && obstacleX < 45 && obstacleX + 8 > 35 && dinoY + 14 > 50) {
    displayGameOver();
    delay(1000);
    displayLoadingScreen();  
    resetGame();
  }

  groundOffset -= obstacleSpeed;
  if (groundOffset < -10) {
    groundOffset = 0;
  }

  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());
}

void draw() {
  for (int i = groundOffset; i < 128; i += 10) {
    u8g.drawLine(i, 63, i + 5, 63);
  }

  drawDino(40, dinoY);

  drawCactus(obstacleX, 50);
}

void drawDino(int x, int y) {
  
  u8g.drawBox(x, y, 10, 10);

  u8g.drawBox(x + 7, y - 5, 5, 5);

  u8g.drawBox(x + 2, y + 10, 2, 4);
  u8g.drawBox(x + 6, y + 10, 2, 4);
}

void drawCactus(int x, int y) {
  u8g.drawBox(x, y, 6, 16);
  u8g.drawBox(x - 2, y + 5, 2, 6);
  u8g.drawBox(x + 6, y + 5, 2, 6);
}

void displayGameOver() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(30, 30, "Game Over!");
  } while (u8g.nextPage());
}

void resetGame() {
  obstacleX = 128;
  dinoY = 50;
  dinoVelocity = 0;
  isJumping = false;
  invincible = false;
}

void displayLoadingScreen() {
  for (int i = 0; i <= 3; i++) {  
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_unifont);
      u8g.drawStr(20, 30, "Loading");
      for (int j = 0; j < i; j++) {
        u8g.drawStr(70 + j * 6, 30, ".");
      }
    } while (u8g.nextPage());
    delay(1000);
  }
}
