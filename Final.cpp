#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define Map_X_MAX 80
#define Map_Y_MAX 40
#define Max_value Map_X_MAX * Map_Y_MAX
#define MAXBULLET 10
#define MAXENEMY 10
#define MAXENEMYBULLET MAXBULLET * 100

char mapData[Max_value];

int b_index = 0; //캐릭터 총알 인덱스
int i_index = 0; //적 수 인덱스
int eb_index = 0; //적 총알 인덱스
int score = 0; // 점수
int gameStatus = 0;

#define BLACK 0
#define BLUE1 1
#define GREEN1 2
#define CYAN1 3
#define RED1 4
#define MAGENTA1 5
#define YELLOW1 6
#define GRAY1 7
#define GRAY2 8
#define BLUE2 9
#define GREEN2 10
#define CYAN2 11
#define RED2 12
#define MAGENTA2 13
#define YELLOW2 14
#define WHITE 15





typedef struct Character {
	int x, y, size_x, size_y;
	char sprite[6];
}plane;

plane myPlane;

typedef struct bullet{
	int x, y;
	bool active;
}Bullet;

Bullet Bullet_info[MAXBULLET];
Bullet Enemy[MAXENEMY];
Bullet Enemy_info[MAXENEMY];
Bullet Enemy_Bullet[MAXBULLET*10000];


void textcolor(int fg_color, int bg_color) {
	// fg_color는 글씨의 색깔, bg_color는 배경의 색깔 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
} 

void FillMap(char str[], char str_s, int max_value) {
    for (int i = 0; i < max_value; i++) {
        str[i] = str_s;
    }
}

//맵 데이터를 수정하는 함수
void EditMap(int x, int y, char str) {
    if (x > 0 && y > 0 && x - 1 < Map_X_MAX - 1 && y - 1 < Map_Y_MAX - 1)
        mapData[(y - 1) * Map_X_MAX + x - 1] = str;
}
//텍스트를 이미지화 하여 출력하는 함수
void DrawSprite(int x, int y, int size_x, int size_y, char spr[]) {
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            EditMap(x + j, y + i, spr[i * size_x + j]);
        }
    }
}

void Enemy_Create(Bullet *Enemy, int count, int difficulty) {
	int a = rand() % 60 + 10;
	int b = rand() % 10 + 5;
	int number = 0;
	
	// EASY = 200(약 11초), NORMAL = 140(약 8초), HARD = 80(약 5초);   	계산값과 약 1초의 오차가 있음 
	if(difficulty == 1) {
		number = 200;
	} else if(difficulty == 2) {
		number = 140;
	} else if(difficulty == 3) {
		number = 80;
	}
	
	if(count % number == 0) { 						// 50ms마다 함수가 호출되므로 적 생성 주기는 50ms * 200 = 10s이다. 
		// 적 생성 범위 설정 a, b 
		Enemy[i_index].x = a;
		Enemy[i_index].y = b;
		Enemy[i_index].active = TRUE;
		i_index++;
	}
	
	for(int i = 0; i < MAXENEMY; i++) { 
		if(Enemy[i].active) {
			mapData[Enemy[i].y * Map_X_MAX + Enemy[i].x - 1] = 'U';
		}
	}
	
	
}

void Enemy_Move(Bullet *Enemy) {
	
	for(int i = 0; i < MAXENEMY; i++) {
		
		if(Enemy[i].active == TRUE){
			if(Enemy[i].x >= 80) {
				Enemy[i].x--;
			} else if(Enemy[i].x <= 0) {
				Enemy[i].x++;
			} else if(Enemy[i].y <= 0) {
				Enemy[i].y++;
			} else if(Enemy[i].y >= 16) {
				Enemy[i].y--;
			} else {
				if (rand() % 4 == 0) {
                    Enemy[i].x++;
                }
                else if (rand() % 4 == 1) {
                    Enemy[i].x--;
                }
                else if (rand() % 4 == 2) {
                    Enemy[i].y++;
                }
                else if (rand() % 4 == 3) {
                    Enemy[i].y--;
                }
			}
		}
	}
	
	
	
}



int condition = 0; 

void game_over(){
     FillMap(mapData, ' ', Max_value);
    textcolor(YELLOW1, BLACK);
    DrawSprite(36, 10, 9, 1, "GAME OVER");
    DrawSprite(24, 15, 35, 16, 
                              "                                   "
                              "    ■                      ■     "
                              "  ■■                      ■■   "
                              "      ■                  ■       "
                        "        ■    ■■■    ■         "
                        "          ■■■■■■■           "
                        "          ■    ■    ■           "
                        "          ■    ■    ■           "
                        "          ■■■■■■■           "
                        "            ■■  ■■             "
                        "            ■  ■  ■             "
                        "         ■   ■■■   ■          "
                        "       ■                ■        "
                        "     ■                    ■      "
                        " ■■                        ■■  "
                        "   ■                        ■    " );

    
}


void Enemy_Attack(Bullet* Enemy_Bullet, Bullet* Enemy, plane* Plane) {
	
	for(int i = 0; i < MAXENEMYBULLET; i++) {
	
		if(Enemy_Bullet[i].active) {
			Enemy_Bullet[i].y++;
			mapData[Enemy_Bullet[i].y * Map_X_MAX + Enemy_Bullet[i].x - 1] = '!';
			
			if(Enemy_Bullet[i].y >= 37) {
				Enemy_Bullet[i].active = false;
			}
			if (Plane->x == Enemy_Bullet[i].x && Plane->y == Enemy_Bullet[i].y) {
                   gameStatus = 3;
               }
               if (Plane->x + 1 == Enemy_Bullet[i].x && Plane->y == Enemy_Bullet[i].y) {
                  gameStatus = 3;
               }
               if (Plane->x + 2 == Enemy_Bullet[i].x && Plane->y == Enemy_Bullet[i].y) {
                 	gameStatus = 3;
               }
		}
	}
	
	if(rand() % 10 != 0) return;
	if(eb_index == MAXBULLET - 1) eb_index = 0;
	
	for(int i = 0; i < MAXBULLET; i++) {
		if(Enemy[i].active) {
			Enemy_Bullet[eb_index].x = Enemy[i].x;
			Enemy_Bullet[eb_index].y = Enemy[i].y + 1;
			Enemy_Bullet[eb_index].active = true;
			eb_index++;
		}
	}
}


//콘솔 창의 크기와 제목을 지정하는 함수
void SetConsole() {
    system("title Shoothig Game!"); // 콘솔창 이름 설정
    system("mode con:cols=80 lines=40"); // 콘솔창 크기설정
    
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 0;
    ConsoleCursor.dwSize = 1;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
} 

void controlcharacter(plane* pPlane) {
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (pPlane->y > 0)
            pPlane->y--;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (pPlane->y < Map_Y_MAX - 1)
            pPlane->y++;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (pPlane->x > 0)
            pPlane->x -= 2;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (pPlane->x < Map_X_MAX - 2)
            pPlane->x += 2;
    }
}





void BulletCount() {
	
	DrawSprite(2, 39, 7, 1, "총알 : ");
	for(int i = 0; i < 10; i++) {
		if(b_index - 1 >= i) {
			EditMap(11 + i, 39, 'X');
		} else {
			EditMap(11 + i, 39, 'O');
		}
	}
}

void Help(){
	
	textcolor(WHITE, BLACK);
		
	gotoxy(20, 5); printf("  ■  ■    ■■■    ■        ■■■ ");
	gotoxy(20, 6); printf("  ■  ■    ■        ■        ■  ■  ");
	gotoxy(20, 7); printf("  ■■■    ■■■    ■        ■■■  ");
	gotoxy(20, 8); printf("  ■  ■    ■        ■        ■      ");
	gotoxy(20, 9); printf("  ■  ■    ■■■    ■■■■  ■      ");
	
	gotoxy(15,17); printf("이동"); gotoxy(20, 17); printf(":"); 
	gotoxy(52, 17); printf("←");  gotoxy(62, 17); printf("→");
	gotoxy(57, 15); printf("↑");  gotoxy(57, 19); printf("↓"); 
	
	gotoxy(15, 24); printf("공격"); gotoxy(20, 24); printf(":");
	gotoxy(55, 24); printf("SPACE");

	gotoxy(15, 31); printf("장전"); gotoxy(20, 31); printf(":");  
	gotoxy(55, 31); printf("CTRL");
	
	gotoxy(36, 38); printf("   ");
}

void Bullet_Shoot(Bullet* pBullet, plane *pPlane, Bullet *Enemy){
	
	for(int i = 0; i < MAXBULLET; i++){
		if(pBullet[i].active){
			pBullet[i].y--;
			mapData[pBullet[i].y * Map_X_MAX + pBullet[i].x] = '*';
			
			if(pBullet[i].y < 1) {
				pBullet[i].active = false;
			}
			
			// 총알 적 충 돌 
			for(int j = 0; j < 10; j++) {
				if(Enemy[j].active && Enemy[j].x == pBullet[i].x && Enemy[j].y == pBullet[i].y) {
					Enemy[j].active = false;
					pBullet[j].active = false;
					condition++;		// 적이 총알에 맞은 수 
				}
			}
		}
	}	
	
	// 플레이어의 움직임에도 총알이 영향을 받기 때문에 새 변수 선언 
	int a = pPlane->x;
	int b = pPlane->y;
	
	if(GetAsyncKeyState(VK_SPACE)){
		if(b_index > 10) return;
		pBullet[b_index].x = a; 
		pBullet[b_index].y = b;
		pBullet[b_index].active = true;
		b_index++;
 	}
 	
 	if(GetAsyncKeyState(VK_CONTROL)){
 		for(int i = 0; i < 10; i++) {
 			pBullet[b_index].active = false;
		 }
		 b_index = 0;
	 } 
}


void mainScreen() {
   
  
   gotoxy(20,1); textcolor(RED1, BLACK); printf("    ■    ■■■  ■■■      ■■■    ■■    ■■■     ■■   ■■■  ");
   gotoxy(20,2); textcolor(RED1, BLACK); printf ("  ■  ■    ■    ■   ■     ■      ■    ■  ■   ■   ■      ■      ");
   gotoxy(20,3); textcolor(YELLOW1, BLACK); printf ("  ■■■    ■    ■■■      ■■■  ■    ■  ■■■    ■      ■■■  ");
   gotoxy(20,4); textcolor(YELLOW1, BLACK); printf ("  ■  ■    ■    ■   ■     ■      ■    ■  ■   ■   ■      ■      ");
   gotoxy(20,5); textcolor(GREEN1, BLACK); printf ("  ■  ■  ■■■  ■   ■     ■        ■■    ■   ■    ■■   ■■■  ");
   gotoxy(50,10); printf("1. 시작");
   gotoxy(50,15); printf("2. 도움말");
   gotoxy(50,20); printf("3. 종료\n");
   
   return; 

}

void gameclear() {
	FillMap(mapData, ' ', Max_value);
    textcolor(YELLOW1, BLACK);
     DrawSprite(36, 10, 9, 1, "VICTORY!!");
	DrawSprite(24, 15, 34, 19, "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□■■■■□■■□□□□□□□■■□■■■■□■■□□□□□□□■■□■■■■■□□□□□□□□□□□■■■■■■■■□□□□□□□■■■■■■■■■■■□□□□□■■■■■■■■■■■■■□□□■■■■■■■■■■■■■■■□■■■■■■■■■■■■■■■■□■■■■■■■■■■■■■■■■□■■■■■■■■■■■■■■■■□■■■■■■■■■■■■■■□□□□□■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
   
}




int option = 0;
int difficulty = 0; 

void StartMenu() {
	int a;
	
	
	while(1) {
		
		
		gotoxy(45, 25);
		printf("번호를 입력하시오: ");
		scanf("%d", &a);
		
		// 난이도 변수 입력 받기 
		if(a == 1) {	
		gotoxy(30, 8);
		printf("난이도를 선택하세요 EASY(1) | NORMAL(2) | HARD(3) : ");
		scanf("%d", &difficulty);
		}
			
		if(a == 1){
			option = 1;
			gameStatus = 1;
			break;
		} else if(a == 2) {
			option = 2;
						
			break;  
		} else if(a == 3) {
			break;
		}
	}
}


int main(){
	
	
	mainScreen();
	StartMenu();
	SetConsole();
	plane myPlane;
	myPlane.x = Map_X_MAX / 2 + 1;
	myPlane.y = Map_Y_MAX / 3 * 2 + 1;
	myPlane.size_x = 3;
	myPlane.size_y = 2;
	strncpy(myPlane.sprite, "_^_^^^", sizeof(myPlane.sprite));
	int count = 0;

	
	
	
	if(option == 1) {
		 
		while(1) {
			while(gameStatus == 1) {
			FillMap(mapData,' ', Max_value);
			controlcharacter(&myPlane);
			textcolor(RED1, BLACK);
			DrawSprite(myPlane.x, myPlane.y, myPlane.size_x, myPlane.size_y, myPlane.sprite);
			
			Bullet_Shoot(&Bullet_info[0], &myPlane, &Enemy_info[0]);
			
			BulletCount();
			Enemy_Create(&Enemy_info[0], count, difficulty);
			Enemy_Move(&Enemy_info[0]);
			Enemy_Attack(&Enemy_Bullet[0], &Enemy_info[0], &myPlane);
			if(condition == 10) {
				gameStatus = 2;
			} 
			printf("%s", mapData);
			Sleep(50); // 한 번 호출에 소요되는 시간은 50ms
			

			count++;
			}
			while(gameStatus == 2) {
				gameclear();
				printf("%s", mapData);
				exit(1);
			}
			if(gameStatus == 3){
				FillMap(mapData, ' ', Max_value);
            	printf("%s", mapData);
            	while (gameStatus == 3) {
                game_over();
                printf("%s", mapData);
                
            	}
			}
			
			
			
		}
	}
	
	
	if(option == 2){
		Help();
	}
	
	
	return 0;
}
