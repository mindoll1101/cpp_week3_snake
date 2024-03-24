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
