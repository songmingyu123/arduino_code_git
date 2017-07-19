#include <stdlib.h>
int times_open = 0; //时间
int times; //点亮循环次数
int i, j; //light二维数组的坐标
int row[8] = { 12, 11, 10, 9, A0, A1, A2, A3 };
int line[8] = { 8, 7, 6, 5, 4, 3, 2, A5 };
int power_pause_Pin = 13;//控制电源和暂停按钮
int turn_left_Pin = A4;//左转按钮
int turn_right_Pin = A6;//右转按钮
int turn_up_Pin = A7;
int turn_down_Pin = A8;
int left = 0; //左
int right = 0; //右
int up = 0; //shang
int down = 0;//xia
int random_x;
int random_y;//获取食物随机的x,y坐标是
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4 //蛇的状态，U：上 ；D：下；L:左 R：右
int status;//当前蛇的正方向
typedef struct SNAKE //蛇身的一个节点
{
    int x;
    int y;
    struct SNAKE *next;
} snake;

snake *head, *food;//蛇头指针和食物指针
snake *q; //遍历蛇的时候用到的指针
int sleeptime = 500;//每次运行的时间间隔
int game_over = 0; //判断游戏是否结束

int aixin1[8][8] =                         //大“心型”的数据
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

int aixin2[8][8] =                         //小“心型”的数据
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0
};


int table[8][8] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
int bad[8][8] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

int power = 0;//判断是否开启电源   0关闭   1开启
int stop_game = 0;//暂停标志

void setup()
{
    Serial.begin(9600); // 打开串口，设置波特率为9600 bps
    pinMode(A5, OUTPUT);//第八列灯
    pinMode(A6, INPUT);//控制右转按钮
    for ( i = 2; i <= 12; ++i )
    {
        pinMode( i, OUTPUT );
        /* 控制列 */
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(13, INPUT);
}



void loop()
{
    for (; power == 0;) //关机循环函数  作用：屏幕熄灭
    {
        times_open++;
        Clear();//屏幕是熄灭的。
        if (digitalRead(power_pause_Pin) == HIGH)  //13接口
        {
            while (digitalRead(power_pause_Pin) == HIGH)
            {
                delay(1);
            }
            power = 1; //电源启动
        }
    }//关机循环函数（结束）

    left = 0; //左转
    right = 0; //右转
    game_over = 0;
    stop_game = 0;
    for (; power == 1; )//游戏循环,当power==0时，退出循环
    {
        play_view();
        game_start();//游戏初始化
        play_game();//玩游戏函数
    }
    free(food);
    while (head != NULL)
    {
        q = head->next;
        free(head);
        head = q;
    }
    for (int i = 0; i < 8; ++i)//初始化矩阵
    {
        for (int j = 0; j < 8; ++j)
        {
            table[i][j] = 0;
        }
    }
}

void game_start()//初始化游戏函数
{
    initsnake();//初始化蛇
    create_food();//创建食物
}

void initsnake()//初始化蛇
{
    snake *tail;
    tail = (snake *)malloc(sizeof(snake)); //从蛇尾开始，头插法，以x,y设定开始的位置//
    tail->x = 0;
    tail->y = 0;
    tail->next = NULL;
    for (i = 1; i <= 3; i++) //蛇的长度为4。初始化位置table[0][0]~[0][3]
    {
        head = (snake *)malloc(sizeof(snake));
        head->next = tail;
        head->x = 0 + i;
        head->y = 0;
        tail = head;
    }
    while (tail != NULL) //从头到为，输出蛇身
    {
        table[tail->y][tail->x] = 1;
        tail = tail->next;
    }
}


void create_food()//创建食物
{
    int judge = 0; //判断食物是否在蛇身上   0是   1不是
    snake *food_1;
    times_open++;
    food_1 = (snake *)malloc(sizeof(snake));
    food_1->next = NULL;
    for (; judge == 0;)
    {
        times_open++;
        get_random_number();
        food_1->x = random_x;
        food_1->y = random_y;
        q = head;
        while (q != NULL) //遍历蛇判断食物是不是在蛇身上
        {
            if (q->x == food_1->x && q->y == food_1->y)
            {
                judge = 0;
                break;
            }
            q = q->next;
            judge = 1;
        }
    }
    food = food_1;
    //table[food->y][food->x] = 1;//食物所在位置
}

void get_random_number()//获得食物的随机坐标
{
    randomSeed(times_open);
    random_x = random(0, 8);
    random_y = random(0, 8);
}


void play_game()//进入玩游戏函数
{
    status = RIGHT;
    for (;;)
    {
        display1();
        if (digitalRead(power_pause_Pin) == HIGH)
        {
            while (digitalRead(power_pause_Pin) == HIGH)
            {
                display1();
            }
            break;
        }
    }
    while (1)
    {
        left = 0;
        right = 0;
        up = 0;
        down = 0;
        //Reassign the variable to 0, indicating that the button has not been pressed
        for (times = 0; times < 150; ++times)
        {
            display1();
            control_snake();//按键控制
            if (game_over == 1)
                break;
        }
        if (game_over == 1)
        {
            for (int i = 0; i < 300; ++i)
            {
                display2();
                power = 0;
            }
            break;
        }
        change_snake_coordinate();//改变蛇的坐标
    }
}

void display1()//点亮灯的函数
{
    for (int i = 0; i < 8; ++i)
    {
        digitalWrite(line[i], LOW);
        for (int j = 0; j < 8; ++j)
        {
            digitalWrite(row[j], table[j][i]);
        }
        delay(1);
        Clear();
    }
    digitalWrite(line[food->x], LOW);
    digitalWrite(row[food->y], HIGH);
    Clear();
}

void display2()
{
    for (int i = 0; i < 8; ++i)
    {
        digitalWrite(line[i], LOW);
        for (int j = 0; j < 8; ++j)
        {
            digitalWrite(row[j], bad[j][i]);
        }
        delay(1);
        Clear();
    }
}

void Clear()                                                                                                        //清空显示
{
    for (int i = 0; i < 8; ++i)
    {
        digitalWrite(row[i], LOW);
    }
    for (int i = 0; i < 8; ++i)
    {
        digitalWrite(line[i], HIGH);
    }
}

void control_snake()//控制按钮
{
    if (digitalRead(turn_left_Pin) == HIGH)//左转按钮被按下
    {
        while (digitalRead(turn_left_Pin) == HIGH)
        {
            display1();
        }
        left = 1;//左转标志
    }
    else if (digitalRead(turn_right_Pin) == HIGH)
    {
        while (digitalRead(turn_right_Pin) == HIGH)
            display1();
        right = 1;
    }
    else if (digitalRead(turn_up_Pin) == HIGH)
    {
        while (digitalRead(turn_up_Pin) == HIGH)
            display1();
        up = 1;
    }
    else if (digitalRead(turn_down_Pin) == HIGH)
    {
        while (digitalRead(turn_down_Pin) == HIGH)
            display1();
        down = 1;
    }
    else if (digitalRead(power_pause_Pin) == HIGH)//电源按钮被按下
    {
        int k = 0;
        stop_game = 1;//游戏暂停
        while (digitalRead(power_pause_Pin) == HIGH) //暂停
        {
            k++;
            display1();
            if (k > 300)
            {
                stop_game = 0;
                game_over = 1;
                while (digitalRead(power_pause_Pin) == HIGH)
                {
                    display2();
                }
            }
        }

        for (; stop_game == 1; )
        {
            display1();
            if (digitalRead(power_pause_Pin) == HIGH)
            {
                int k = 0;
                while (digitalRead(power_pause_Pin) == HIGH)
                {
                    k++;
                    display1();
                    if (k > 300)
                    {
                        stop_game = 0;
                        game_over = 1;
                        while (digitalRead(power_pause_Pin) == HIGH)
                        {
                            display2();
                        }
                    }
                }
                stop_game = 0;
            }

        }
    }
}

void change_snake_coordinate() //改变蛇的坐标
{
    snake *nexthead;
    nexthead = (snake *)malloc(sizeof(snake));
    if (left == 1 && status != RIGHT)//if the left button is pressed
    {
        status = LEFT;
        nexthead->x = head->x - 1;
        nexthead->y = head->y;
    }//if the left button is pressed (END)
    else if (right == 1 && status != LEFT)
    {
        status = RIGHT;
        nexthead->x = head->x + 1;
        nexthead->y = head->y;
    }
    else if (up == 1 && status != DOWN) //////////没有按下方向键
    {
        status = UP;
        nexthead->x = head->x;
        nexthead->y = head->y - 1;
    }
    else if (down == 1 && status != UP)
    {
        status = DOWN;
        nexthead->x = head->x;
        nexthead->y = head->y + 1;
    }
    else // if no buttons are pressed
    {
        if (status == LEFT)//If the positive direction of the current movement is left
        {
            nexthead->x = head->x - 1;
            nexthead->y = head->y;
        }
        if (status == RIGHT)//If the positive direction of the current movement is left
        {
            nexthead->x = head->x + 1;
            nexthead->y = head->y;
        }
        if (status == UP)//If the positive direction of the current movement is left
        {
            nexthead->x = head->x;
            nexthead->y = head->y - 1;
        }
        if (status == DOWN)//If the positive direction of the current movement is left
        {
            nexthead->x = head->x;
            nexthead->y = head->y + 1;
        }
    }
    if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个位置有食物
    {
        nexthead->next = head;
        head = nexthead;
        q = head;
        while (q != NULL)
        {
            table[q->y][q->x] = 1;
            q = q->next;
        }
        create_food();
    }
    else
    {
        nexthead->next = head;
        head = nexthead;
        q = head;
        while (q->next->next != NULL) //先把尾巴的位置为0
        {
            q = q->next;
        }
        table[q->next->y][q->next->x] = 0;
        q = head;
        while (q->next->next != NULL)
        {
            table[q->y][q->x] = 1;
            q = q->next;
        }
        q->next = NULL;
    }
    Hit_wall();
    Hit_self();
}



void Hit_wall()//判断是否撞墙
{
    if (head->x == -1 || head->x == 8 || head->y == -1 || head->y == 8) //蛇头撞墙了
    {
        game_over = 1;//判断游戏是否结束
    }
}

void Hit_self()//判断是否撞到自己
{
    snake *self;
    self = head->next;
    while (self != NULL)
    {
        if (self->x == head->x && self->y == head->y)
        {
            game_over = 1;
            break;
        }
        self = self->next;
    }
}

void play_view()
{
    for (int p = 0; p < 2; ++p)
    {
        for (int i = 0; i < 200; ++i)
        {
            display3(aixin1);
        }
        for (int i = 0; i < 200; ++i)
        {
            display3(aixin2);
        }
    }

}

void display3(int aixin[8][8])
{
    for (int i = 0; i < 8; ++i)
    {
        digitalWrite(line[i], LOW);
        for (int j = 0; j < 8; ++j)
        {
            digitalWrite(row[j], aixin[j][i]);
        }
        delay(1);
        Clear();
    }
}
