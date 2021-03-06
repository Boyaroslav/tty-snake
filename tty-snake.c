#include<stdio.h>
//#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>

#define SIZEBUF 100 // max size of game zone 
#define TIME 2
#define CYCLE 2000


#define Orange "\033[32m"
#define Black "\033[39m"
#define Default "\033[39m"
#define Blue "\033[85m"
#define Magenta "\033[36m"


char move = 'r';
char *block = "█";
int clockbin = 0;
int not_in_snake, ins;
int game = 1;
int SIZEX = 20, SIZEY = 20; // default size

struct sbody
{
    int x;
    int y;
    char *d; // how to draw
};

struct sbody snake[SIZEBUF * SIZEBUF];
int leng = 1;

int draw(struct sbody apple){
    system("clear");
    if(snake[0].x < 0 || snake[0].x >= SIZEX || snake[0].y < 0 || snake[0].y >= SIZEY){
        game = 1;
        return 1;
    }
    printf("%s", Orange);
    for(int i = 0;i<SIZEX + 2;i++){printf("%s", block);}
    printf("score - %d\n%s", leng, Black);
    // рисование бортиков
int game = 1;

    for(int i=0; i<SIZEY; i++){
        printf("%s%s%s", Orange, block, Black);
        for(int j=0;j<SIZEX;j++){
            int in = 0;
            if(j == apple.x && i == apple.y){printf("%s%s", Blue, apple.d);continue;}
            printf(Black);
            for(int s=0;s<leng;s++){
                if(j == snake[s].x && i == snake[s].y){in = 1;printf("%s%s", Magenta, snake[s].d);printf(Black);continue;}
            }
            if(in == 0){printf(Black);putchar(' ');}
        }
        printf("%s%s%s\n", Orange, block,  Black);

    }
    printf("%s", Orange);
    for(int i = 0;i<SIZEX + 2;i++){printf("%s", block);}putchar('\n'); // рисование бортиков
    
    return 0;
}

void moving(struct sbody *apple){
    // checking apple's eating
    if(apple->x == snake[0].x && apple->y == snake[0].y){
        struct sbody x = {1, -1, block};
        snake[leng] = x;
        leng++; 
	not_in_snake=0;
        while(not_in_snake == 0){
        	apple->x = rand() % SIZEX;
        	apple->y = rand() % SIZEY;
		ins = 0;
		for(int i = 0;i<leng;i++){
			if(snake[i].x == apple->x && snake[i].y == apple->y){ins=1;}
		}
		if(ins == 0){not_in_snake=1;}
        }

    }

    // hui
    for(int i = leng - 1; i > 0;i--){
        snake[i] = snake[i - 1];
    }
    if(move == 'r'){
        snake[0].x = snake[0].x + 1;
    }
    else if(move == 'l'){
        snake[0].x = snake[0].x - 1;
    }
    else if(move == 'u'){
        snake[0].y = snake[0].y - 1;
    }
    else if(move == 'd'){
        snake[0].y = snake[0].y + 1;
    }
    // checking eating by yourself
    for(int i=1;i<leng;i++){
        if(apple->x == snake[i].x && apple->y == snake[i].y){
            apple->x = rand() % SIZEX;
            apple->y = rand() % SIZEY;
        }

        if(snake[i].x == snake[0].x && snake[i].y == snake[0].y){
            printf("%s", Black);
            game = 0;
            return;
        }
    }

}
#if _WIN32
block = "#"; // на винде дефолтный шрифт не тянет обычный block
char geti(){
    #include<conio.h>
    return getch();
}

#else
#include<termios.h>
char geti(){
    // setting terminal icanonical mode
    struct termios new, old;
    tcgetattr(STDERR_FILENO, &old);
    new = old;
    //new.c_lflag &= ~(ICANON | XCASE); // ECHO задает не выводить введенное на экран
    new.c_cc[VMIN]  = 1;
    new.c_cc[VTIME] = 0;
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    char i = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    tcsetattr(STDERR_FILENO, TCSANOW, &old);
    return i;
}
#endif

// I just want to do it by myself
int toint(char *x){
    int n=0, j=0;
    while(x[j] >= '0' && x[j] <= '9'){
        n = (n * 10) + (x[j] - '0'); j++;

    }
    return n;
}




int main(int argc, char **argv){
    // рекомендую немного увеличить ширину сетки символов в терминале, для более красивой картинки. Удачи!
    
    for(int i=0; i<argc; i++){

        if(strcmp(argv[i], "-b") == 0){block = argv[i + 1];}
        if(strcmp(argv[i], "-x") == 0){SIZEX = toint(argv[i + 1]); if(SIZEX > 100){SIZEX = 100;}}
        if(strcmp(argv[i], "-y") == 0){SIZEY = toint(argv[i + 1]); if(SIZEY > 100){SIZEY = 100;}}
    }

    srand(time(NULL));
    struct sbody head = {1, 5, block};
    snake[0] = head;
    struct sbody apple;
    //apple.x = rand() % SIZE;
    //apple.y = rand() % SIZE;
    not_in_snake=0;
    while(not_in_snake == 0){
    	apple.x = rand() % SIZEX;
        apple.y = rand() % SIZEY;
        ins = 0;
        for(int i = 0;i<leng;i++){
        	if(snake[i].x == apple.x && snake[i].y == apple.y){ins=1;}
        }
        if(ins == 0){not_in_snake=1;}
    }
    apple.d = block;

    int isq = 0; char g; char b;
    while (game == 1){
        
        b = geti();
        if(b == 'w' || b == 'a' || b == 's' || b == 'd'){
            g = b;
        }

        fflush(stdout);
        fflush(stdin);
        fflush(stdout);
        if(isq == 1){
            printf(Black);
            break;
        }
        usleep(TIME);
        if(clockbin % CYCLE == 0){
            // если менять при нажатии, то порой бывают "казусы"
            if(g == 'a' && move != 'r'){move = 'l';}
            else if(g == 'd' && move != 'l'){move = 'r';}
            else if(g == 'w' && move != 'd'){move = 'u';}
            else if(g == 's' && move != 'u'){move = 'd';}
            isq = draw(apple);
            moving(&apple);
            clockbin = 0;
        }
        clockbin++;

    }
    
    printf("your score is - %d\n", leng);
    return 0;
}

