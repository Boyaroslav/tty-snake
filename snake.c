#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include <fcntl.h>
#include <time.h>

#define SIZE 20
#define TIME 10
#define CYCLE 2000

#define Orange "\033[96m"
#define Black "\033[40m"
#define Default "\033[39m"
#define Blue "\033[94m"
#define Magenta "\033[36m"


char move = 'r';
int clockbin = 0;

struct sbody
{
    int x;
    int y;
    char d; // how to draw
};

struct sbody snake[SIZE * SIZE];
int leng = 1;

int draw(struct sbody apple){
    system("clear");
    if(snake[0].x < 0 || snake[0].x >= SIZE || snake[0].y < 0 || snake[0].y >= SIZE){
        return 1;
    }

    for(int i = 0;i<SIZE;i++){putchar('#');}
    printf("score - %d\n%s", leng, Black);
    // рисование бортиков


    for(int i=0; i<SIZE; i++){
        printf("%s#%s", Magenta, Black);
        for(int j=0;j<SIZE;j++){
            int in = 0;
            if(j == apple.x && i == apple.y){printf("%s%c", Blue, apple.d);continue;}
            printf(Black);
            for(int s=0;s<leng;s++){
                if(j == snake[s].x && i == snake[s].y){in = 1;printf("%s%c", Orange, snake[s].d);printf(Black);continue;}
            }
            if(in == 0){printf(Black);putchar(' ');}
        }
        printf("%s#%s\n", Magenta, Black);

    }
    for(int i = 0;i<SIZE;i++){putchar('#');}putchar('\n'); // рисование бортиков
    
    return 0;
}

void moving(struct sbody *apple){
    // checking apple's eating
    if(apple->x == snake[0].x && apple->y == snake[0].y){
        struct sbody x = {1, -1, '#'};
        snake[leng] = x;
        leng++; 
        
        apple->x = rand() % SIZE;
        apple->y = rand() % SIZE;
        

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
            apple->x = rand() % SIZE;
            apple->y = rand() % SIZE;
        }

        if(snake[i].x == snake[0].x && snake[i].y == snake[0].y){
            printf(Black);
            return;
        }
    }

}

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



int main(){
    srand(time(NULL));
    struct sbody head = {1, 5, '#'};
    snake[0] = head;
    struct sbody apple;
    apple.x = rand() % SIZE;
    apple.y = rand() % SIZE;
    apple.d = '*';

    int game = 1;
    int isq = 0; char g;
    while (game == 1){
        
        g = geti();

        if(g == 'a' && move != 'r'){move = 'l';}
        else if(g == 'd' && move != 'l'){move = 'r';}
        else if(g == 'w' && move != 'd'){move = 'u';}
        else if(g == 's' && move != 'u'){move = 'd';}
        fflush(stdout);
        fflush(stdin);
        fflush(stdout);
        if(isq == 1){
            printf(Black);
            break;
        }
        usleep(TIME);
        if(clockbin % CYCLE == 0){
            isq = draw(apple);
            moving(&apple);
            clockbin = 0;
        }
        clockbin++;

    }
    

    return 0;
}