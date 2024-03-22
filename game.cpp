#include "console.h"
#include <string>
#include <cstdlib>
#include <ctime>

#define BOARD_SIZE 10
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int x = BOARD_SIZE/2;                   //시작 위치
int y = BOARD_SIZE/2;

int i = 0;        //apple 위치
int j = 0;
int apple_arr[BOARD_SIZE-1][BOARD_SIZE-1][2];   //apple 저장

int body = 0;                           //뱀의 몸 길이

int score = 0;                          //점수 저장

bool key_left = false;                  //key입력 저장
bool key_right = false;
bool key_up = false;
bool key_down = false;
bool key_esc = false;

bool play = false;                      //game play상태 저장



void handleInput() {
  if (console::key(console::K_LEFT)) {
    key_left = console::key(console::K_LEFT);
    key_right = false;
    key_up = false;
    key_down = false;
  }
  else if (console::key(console::K_RIGHT)) {
    key_right = console::key(console::K_RIGHT);
    key_left = false;
    key_up = false;
    key_down = false;
  }
  else if (console::key(console::K_UP)) {
    key_up = console::key(console::K_UP);
    key_right = false;
    key_left = false;
    key_down = false;
  }
  else if (console::key(console::K_DOWN)) {
    key_down = console::key(console::K_DOWN);
    key_right = false;
    key_left = false;
    key_up = false;
  }
  else if (console::key(console::K_ESC)){
    key_esc = true;
  }


  if(key_left){
    x--;
  }
  else if(key_right){
    x++;
  }
  else if(key_up){
    y--;
  }
  else if(key_down){
    y++;
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

void drawSnakeHead() {
  // x, y 위치에 뱀의 머리를 그린다.
  console::draw(x, y, SNAKE_STRING);
}

void drawWall() {
  // 지정된 크기만큼의 게임 공간을 그린다.
  console::draw(0, 0, WALL_LEFT_TOP_STRING);
  console::draw(0, BOARD_SIZE-1, WALL_LEFT_BOTTOM_STRING);
  console::draw(BOARD_SIZE-1, 0, WALL_RIGHT_TOP_STRING);
  console::draw(BOARD_SIZE-1, BOARD_SIZE-1, WALL_RIGHT_BOTTOM_STRING);
  for(int i = 1; i < BOARD_SIZE-1; i++){
    console::draw(0, i, WALL_VERTICAL_STRING);
    console::draw(BOARD_SIZE-1, i, WALL_VERTICAL_STRING);
    console::draw(i, 0, WALL_HORIZONTAL_STRING);
    console::draw(i, BOARD_SIZE-1, WALL_HORIZONTAL_STRING);
  }
}

void gameOver(){
  //game over시 문구를 출력하고 게임 진행 후 사용된 변수들을 초기화한다.
  if(x >= BOARD_SIZE-1 || x <= 0 || y >= BOARD_SIZE || y <= 0){
    console::draw(BOARD_SIZE/2 - 4, BOARD_SIZE/2 - 1, "YOU LOSE!");
    console::draw(BOARD_SIZE/2 - 8, BOARD_SIZE/2, "Try again? (Enter)");
    i = rand() % (BOARD_SIZE-2) + 1;
    j = rand() % (BOARD_SIZE-2) + 1;
    score = 0;
    body = 0;
    key_left = false;
    key_right = false;
    key_up = false;
    key_down = false;
    play = false;
  }
}

void applePosition(){
  for(int i = 0; i < BOARD_SIZE-1; i++){
    for(int j = 0; j < BOARD_SIZE-1; j++){
      apple_arr[i][j][0] = i;
      apple_arr[i][j][1] = j;
    }
  }
}

void apple(){
  //apple을 그린다.
  console::draw(i, j, APPLE_STRING);
  if(x == i && y == j){
    body += 1;
    score += 10;
    i = rand() % (BOARD_SIZE-2) + 1;
    j = rand() % (BOARD_SIZE-2) + 1;
  }
}

void printScore(){
  //score를 출력한다.
  std::string sc = "score: ";
  sc = sc + std::to_string(score);
  console::draw(BOARD_SIZE/2 -  sc.length()/2, BOARD_SIZE, sc);
}

void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();
  
  applePosition();

  std::srand(time(NULL));
  i = rand() % (BOARD_SIZE-2) + 1;
  j = rand() % (BOARD_SIZE-2) + 1;

  while (true) {
    // 화면을 지운다.
    console::clear();

    play = true;
    if(key_esc){
      break;
    }

    drawWall();     //게임 범위 벽 생성
    handleInput();  //이동 제어
    restrictInScreen(); //최대 범위 밖으로 나가지 않도록 함.
    apple();            //apple을 random 위치에 생성
    drawSnakeHead();    //뱀 머리 생성
    printScore();
    gameOver();         //벽에 부딪히거나 몸에 부딪히면 게임 오버.    

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    for(int i = 0; i < MOVE_DELAY; i++){
      console::wait();
    }
    
    while(!play){
      if(console::key(console::K_ENTER)){
        x = BOARD_SIZE/2;
        y = BOARD_SIZE/2;
        break;
      }
      else if(console::key(console::K_ESC)){
        key_esc = true;
        break;
      }
    }
  }
}
