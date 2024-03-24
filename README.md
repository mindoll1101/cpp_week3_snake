snake게임을 main.cpp, game.cpp, util.cpp/console.cpp, console.h를 이용하여 완성함.
main.cpp에서는 게임을 실행.
game.cpp은 게임의 설정 및 실행 흐름이 작성됨.
util.cpp은 game.cpp을 보조하기 위한 코드.

맵의 크기를 변경하려면 game.cpp의 BOARD_SIZE를 변경(4~20)
딜레이 프레임을 설정하려면 game.cpp의 MOVE_DELAY 변경

실행 방법:
g++ -Werror -std=c++11 -c *.cpp
g++ -o main.exe *.o
./main.exe

게임 시작 시 뱀의 최초 이동 방향은 오른쪽.
사과를 먹으면 score가 10 증가.
score가 (BOARD_SIZE-2)**2 * 10 - 1이 되면(뱀이 맵을 가득 채우면) 게임 종료, 플레이어 승리.
벽 또는 뱀의 몸에 머리가 부딪힐 시 게임 종료, 플레이어 패배.
위 아래 왼쪽 오른쪽 이동 가능, 사과를 하나라도 먹을 시 진행 방향의 반대 방향으로 이동 불가.
사과는 뱀이 없는 위치에 랜덤으로 생성.
