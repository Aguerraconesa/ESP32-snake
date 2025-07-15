#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// definicion de las direcciones
#define botright 4
#define botup 25
#define botdown 26
#define botleft 27

// definicion de la pantalla oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Snake
int snakeX[100], snakeY[100];
int snakelength = 3;
// 0=arriba, 1=derecha, 2=abajo, 3=izquierda
int direction = 1;

// food generation
int foodX, foodY;
bool foodneed = true;

void foodgeneration();
void drawfood();
void readdirection();
void movesnake();
void drawsnake();
bool checkselfcolision();
void losescreen();
void eatfood();

void setup() {
  pinMode(botright, INPUT_PULLUP);
  pinMode(botup, INPUT_PULLUP);
  pinMode(botdown, INPUT_PULLUP);
  pinMode(botleft, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Iniciando la comunicacion Serial: ");
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("Error OLED"));
    for(;;);
  }

  // Posición inicial (cabeza en (64, 32), cuerpo seguido)
  snakeX[0] = 64; snakeY[0] = 32;  // Cabeza
  snakeX[1] = 58; snakeY[1] = 32;  // Segmento 1
  snakeX[2] = 52; snakeY[2] = 32;  // Segmento 2

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hola Snake");
  display.display();
  delay(1000);
}

void loop() {
  // food
  foodgeneration();

  // Read Botons and update direction
  readdirection();

  // Move the snake
  snakemove();

  // ver si te has topado con el cuerpo
  checkselfcolision();

  // losescreen
  losescreen();

  //comer fruta
  eatfood();

  display.clearDisplay();
  // Draw the snake in the display
  drawsnake();

  // draw food
  drawfood();

  display.display();
  // it controls the speed
  delay(200);
}

void foodgeneration()
{
  if (foodneed)
  {
    bool validposition;
    do
    {
      validposition = true;
      foodX = random(0, SCREEN_WIDTH-6); 
      foodY = random(0, SCREEN_HEIGHT-6);
      for(int i = 0; i < snakelength; i++)
      {
        if(abs(foodY - snakeY[i]) < 6 && abs(foodX - snakeX[i]) < 6)
        {
          validposition = false;
          break;
        }
      }
    } while (!validposition);
    foodneed = false;
  }
}

void readdirection()
{
  if (digitalRead(botright) == LOW)
  {
    Serial.println("right");
    direction = 1;
  }
  if (digitalRead(botup) == LOW)
  {
    Serial.println("up");
    direction = 0;
  }
  if (digitalRead(botdown) == LOW)
  {
    Serial.println("down");
    direction = 2;
  }
  if (digitalRead(botleft) == LOW)
  {
    Serial.println("left");
    direction = 3;
  }
}

void snakemove()
{
  // Firstly, in this function we move the "body" of the snake to the head (from the tail to the head)
  for (int i = snakelength - 1; i > 0; i--)
  {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Secondly, we move the head of the snake
   switch (direction)
  {
    // Up
    case 0: snakeY[0] -= 6; break;
    // Right
    case 1: snakeX[0] += 6; break;
    // Down
    case 2: snakeY[0] += 6; break;
    // Left
    case 3: snakeX[0] -= 6; break;
  }
  // Aqui con constrain estamos diciendo que no se salga de unos limites que nosotros establecemos
  snakeX[0] = constrain(snakeX[0], 0, SCREEN_WIDTH - 6);
  snakeY[0] = constrain(snakeY[0], 0, SCREEN_HEIGHT - 6);
} 


bool checkselfcolision()
{
  for(int i = 1; i < snakelength; i++)
  {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
    {
      return true;
    }
  }
  return false;
}

void losescreen()
{
  if(checkselfcolision())
  {
    display.clearDisplay();
    display.setTextSize(4);
    display.setCursor(0,0);
    display.println("GAME OVER");
    display.display();
    snakelength = 3;
    snakeX[0] = 64; snakeY[0] = 32;
  }
}

void eatfood()
{
  if(abs(foodX - snakeX[0] < 4) && abs(foodY - snakeY[0]) < 4)
  {
    display.clearDisplay();
    foodneed = true;
    snakelength++;
  }
}

void drawsnake()
{
  for (int i = 0; i < snakelength; i++) 
  {
    display.fillRect(snakeX[i], snakeY[i], 6, 6, WHITE);
  }
}

void drawfood()
{
  display.fillCircle(foodX, foodY, 3, WHITE);
}
