#include "console.h"

#define BOARD_SIZE 20
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

//입력한 크기에 맞춰 random 위치 반환
int randomIdx(int start_num, int end_num);

//score 출력
void printScore(int score, int size);

//시작 위치
int x = BOARD_SIZE/2;                              
int y = BOARD_SIZE/2;

//apple 위치
int rand_idx = 0;
//apple 저장 가능 위치
int apple_x = 0;
int apple_y = 0;

//snake위치 저장
int snake_arr[(BOARD_SIZE-2)*(BOARD_SIZE-2)][2] = {x, y}; 
//뱀의 몸 길이 
int snake_size = 0;              

//점수 저장
int score = 0;         

//key입력 저장
bool key_left = false;                             
bool key_right = false;
bool key_up = false;
bool key_down = false;
bool key_esc = false;
bool key_enter = false;

//game play상태 저장
bool play = false;                                 

void handleInput() {
  //뱀의 크기가 2 이상일 때 반대로 이동하지 않도록 함.
  if (console::key(console::K_LEFT) && (!key_right || snake_size == 0)) {
    key_left = console::key(console::K_LEFT);
    key_right = false;
    key_up = false;
    key_down = false;
  }
  else if (console::key(console::K_RIGHT) && (!key_left || snake_size == 0)) {
    key_right = console::key(console::K_RIGHT);
    key_left = false;
    key_up = false;
    key_down = false;
  }
  else if (console::key(console::K_UP) && (!key_down  || snake_size == 0)) {
    key_up = console::key(console::K_UP);
    key_right = false;
    key_left = false;
    key_down = false;
  }
  else if (console::key(console::K_DOWN) && (!key_up  || snake_size == 0)) {
    key_down = console::key(console::K_DOWN);
    key_right = false;
    key_left = false;
    key_up = false;
  }
  else if (console::key(console::K_ESC)){
    key_esc = true;
  }

  //입력받은 방향으로 이동
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
  else{
    key_right = true;
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

void snakePos() {
  for(int i = snake_size; i > 0; i--){    //뱀의 머리를 따라가는 뱀의 몸 위치
    snake_arr[i][0] = snake_arr[i-1][0];
    snake_arr[i][1] = snake_arr[i-1][1];
  }
  snake_arr[0][0] = x;
  snake_arr[0][1] = y;
}

void drawSnake() {
  for(int i = 0; i <= snake_size; i++){
    //뱀의 몸을 그린다.
    console::draw(snake_arr[i][0], snake_arr[i][1], SNAKE_STRING);
  }
}

void applePos() {
  bool apple_arr[BOARD_SIZE-2][BOARD_SIZE-2] = {false};
  for(int i = 0; i <= snake_size; i++){
    apple_arr[snake_arr[i][0] - 1][snake_arr[i][1] - 1] = true;
  }
  
  int isFalseCount = 0;
  for(int i = 0; i < BOARD_SIZE-2; i++){
    for(int j = 0; j < BOARD_SIZE-2; j++){
      if(!apple_arr[i][j]){
        isFalseCount++;
        snake_arr[snake_size + isFalseCount][0] = i + 1;
        snake_arr[snake_size + isFalseCount][1] = j + 1;
      }
    }
  }
  rand_idx = randomIdx(snake_size + 1, (BOARD_SIZE-2)*(BOARD_SIZE-2));
  apple_x = snake_arr[rand_idx][0];
  apple_y = snake_arr[rand_idx][1];
}

void drawApple(){
  // apple을 그린다.
  if(x == apple_x && y == apple_y){
    snake_size += 1;
    score += 10;
    snakePos();
    applePos();
  }
  else{
    snakePos();
  }

  console::draw(apple_x, apple_y, APPLE_STRING);
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
  for(int i = 2; i <= snake_size; i++){
    //뱀의 머리와 몸이 부딪혔을 경우
    if(snake_arr[i][0] == x && snake_arr[i][1] == y){
      play = false;
    }
  }
  //게임 종료 조건(뱀이 맵 밖으로 나간 경우, 뱀의 머리와 몸이 부딪힌 경우, 최대 점수를 달성한 경우)
  if(x >= BOARD_SIZE - 1 || x <= 0 || y >= BOARD_SIZE - 1 || y <= 0 || !play || score >= (BOARD_SIZE-2)*(BOARD_SIZE-2) * 10 - 10){
    while(true){
      //Enter입력 시 재시작, esc입력 시 종료.
      if(score == (BOARD_SIZE-2)*(BOARD_SIZE-2) * 10 - 10){
        console::draw(BOARD_SIZE/2 - 4, BOARD_SIZE/2 - 1, "YOU WIN!");
        console::draw(BOARD_SIZE/2 - 10, BOARD_SIZE/2, "Play new game (Enter)");
      }
      else{
        console::draw(BOARD_SIZE/2 - 4, BOARD_SIZE/2 - 1, "YOU LOSE!");
        console::draw(BOARD_SIZE/2 - 9, BOARD_SIZE/2, "Try again? (Enter)");
      }
      if(console::key(console::K_ENTER)){
        x = BOARD_SIZE/2;    //변수 초기화
        y = BOARD_SIZE/2;
        score = 0;
        snake_size = 0; 
        snake_arr[0][0] = x;
        snake_arr[0][1] = y;
        applePos();
        key_left = false;
        key_right = false;
        key_up = false;
        key_down = false;
        key_enter = true;
        break;
      }
      else if(console::key(console::K_ESC)){
        key_esc = true;
        break;
      }
      else{
        console::wait();
      }
    }
  } 
}


void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();

  applePos();

  while (true) {
    // 화면을 지운다.
    console::clear();

    play = true;

    
    drawWall();     //게임 범위 벽 생성
    handleInput();  //이동 제어
    restrictInScreen(); //최대 범위 밖으로 나가지 않도록 함.
    drawApple();            //apple을 random 위치에 생성
    drawSnake();    //뱀 생성
    printScore(score, BOARD_SIZE);
    gameOver();         //벽에 부딪히거나 몸에 부딪히면 게임 오버.
    
       

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    if(key_esc){
      break;
    }
    else if(key_enter){
      key_enter = false;
    }
    else{
      for(int i = 0; i < MOVE_DELAY; i++){
        console::wait();
      }
    }
  }
}
