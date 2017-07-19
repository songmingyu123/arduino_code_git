
//设置阴极接口
int a = A2;
int b = 2;
int c = 3; 
int d = 4;
int e = 5;
int f = A0;
int g = 7;
int p = 8;
//设置阳极接口
int d4 = 9;
int d3 = 10;     //数码管3接口
int d2 = 11;     //数码管2接口
int d1 = 12;    //数码管1接口
//设置变量
int del = 100;
long n = 0;
int x = 100;
int delsmall = 1000;

//设置钟表数字变量
int min_Ten = 1; //分的十位
int min_Bit = 0; //分的个位
int second_Ten = 0; //秒的十位
int second_Bit = 0; //秒的个位
int min_t = 10;  //分
int second_t = 0; //秒
byte segs[7] = { a, b, c, d, e, f, g };
int seven_seg_digits[10][7] = { { 0, 0, 0, 0, 0, 0, 1 }, // = 0
  { 1, 0, 0, 1, 1, 1, 1 }, // = 1
  { 0, 0, 1, 0, 0, 1, 0 }, // = 2
  { 0, 0, 0, 0, 1, 1, 0 }, // = 3
  { 1, 0, 0, 1, 1, 0, 0 }, // = 4
  { 0, 1, 0, 0, 1, 0, 0 }, // = 5
  { 0, 1, 0, 0, 0, 0, 0 }, // = 6
  { 0, 0, 0, 1, 1, 1, 1 }, // = 7
  { 0, 0, 0, 0, 0, 0, 0 }, // = 8
  { 0, 0, 0, 0, 1, 0, 0 } // = 9
};
void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(13, INPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);  //串口波特率为9600
  pinMode(A1, INPUT); //设置引脚为输入模式
}



///////////


int u = 0;//判断是暂停还是开始
int judge_on_stopwatch = 0;
int loopcount = 0;  //开机动画的循环次数
int off_by_stopwatch = 0;//在秒表函数中关闭程序
int change_func = 0;  //改变倒计时和秒表的功能的标志
double k = 0; //判断暂停长按的时间
unsigned long duration;
unsigned long judge;
int judge_on = 0;//判断程序启动
int judge_time = 0; //计算按键按下的时间

void loop()
{
  if (digitalRead(A1) == HIGH)
  {
    while (digitalRead(A1) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
    {
      print_number_now();
    }
    judge_on = 0;
  }
  //以下控制关机
  //在judge_on==0的时候，只执行这个for语句，中的101~104行，是屏幕熄灭，直到A1引脚接收到按钮的信息，执行105的if语句，使屏幕亮起。
  //关机
  for (; judge_on == 0;)
  {
    digitalWrite(d1, LOW);
    digitalWrite(d2, LOW);
    digitalWrite(d3, LOW);
    digitalWrite(d4, LOW);
    //开机
    if (digitalRead(A1) == HIGH) //检测电源按钮是不是电平变为高了。是的话，就是刚好按钮按下了。
    {
      while (digitalRead(A1) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
      {
        //进入这个if和while语句就说明了数码管将要被启动了，初始化时间为10:00，
        min_Ten = 1; //分的十位
        min_Bit = 0; //分的个位
        second_Ten = 0; //秒的十位
        second_Bit = 0; //秒的个位
        min_t = 10;  //分
        second_t = 0; //秒
        u = 0;  //u%2==0 表示现在时间是暂停的。
        judge_on_stopwatch = 0;
        off_by_stopwatch = 0;
        roulette(2);  //轮转效果
        circles(2);   //圈状效果
        print_number_now();
      }
      judge_on = 1; //judge_on==1,
    }
    
  } //关机状态函数结束



  //得到时间各位
  min_Ten = find_Ten(min_t);
  min_Bit = find_Bit(min_t);
  second_Ten = find_Ten(second_t);
  second_Bit = find_Bit(second_t);
  //得到时间各位结束
  //显示数字部分
  //
  //
  //
  for (int i = 0; i <= 250 && off_by_stopwatch == 0; i++)
  {
    print_number_now();
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //秒表
    if (digitalRead(A3) == HIGH)
    {
      if (u%2 == 0)
      {  
        change_func = 0;
        u=0;
        judge_on_stopwatch = 1;
        while (digitalRead(A3) == HIGH)
        {
          min_t = 0;
          second_t = 0;
          print_number_now();
        }
        for (;judge_on_stopwatch == 1;)
        {
          //得到时间各位
          min_Ten = find_Ten(min_t);
          min_Bit = find_Bit(min_t);
          second_Ten = find_Ten(second_t);
          second_Bit = find_Bit(second_t);
          //得到时间各位结束
          for (int m = 0; m <= 250; m++)
          {
            print_number_now();
            if (digitalRead(13) == HIGH) //然后检测是不是电平变为高了。是的话，就是刚好按钮按下了。
            {
              k=0;
              while (digitalRead(13) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
              {
                print_number_now();
                k++;
                if (k >= 400)
                {
                  min_Ten = 0; //分的十位
                  min_Bit = 0; //分的个位
                  second_Ten = 0; //秒的十位
                  second_Bit = 0; //秒的个位
                  min_t = 0;  //分
                  second_t = 0; //秒
                  u = 1;
                }
              }
              u++;
            }
            if (u % 2 == 0 )
              m--;
                    ///////////////////////////////////////////////////////////////////////////////
            if (digitalRead(A1) == HIGH)
            {
              while (digitalRead(A1) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
              {
                print_number_now();
              }
              judge_on_stopwatch = 0;
              off_by_stopwatch = 1;
              judge_on = 0;
              second_t = 1;
              min_t = 0;
              break;//当程序暂停的时候，按电源键退出显示数字函数                 暂停的时候和运行的时候关机有问题
            }
            if (digitalRead(A3) == HIGH)
            {
              while (digitalRead(A3) == HIGH)
              {
                print_number_now();
              }
                min_Ten = 1; //分的十位
                min_Bit = 0; //分的个位
                second_Ten = 0; //秒的十位
                second_Bit = 0; //秒的个位
                min_t = 10;  //分
                second_t = 0; //秒
                u = 0;  //u%2==0 表示现在时间是暂停的。
                change_func = 1;
                break;
            }
          }//显示一个数值的循环for
          if (change_func == 1)
          {
            break;
          }
          change_time_forward();
       }//秒表大循环
      }  
    }//判断A3按钮是否按下的if语句结束
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //判断暂停按钮是否按下按钮
    if (digitalRead(13) == HIGH) //然后检测是不是电平变为高了。是的话，就是刚好按钮按下了。
    {
      k = 0;
      while (digitalRead(13) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
      {
        print_number_now();
        k++;
        if (k >= 400)
        {
          min_Ten = 1; //分的十位
          min_Bit = 0; //分的个位
          second_Ten = 0; //秒的十位
          second_Bit = 0; //秒的个位
          min_t = 10;  //分
          second_t = 0; //秒
          u = 1;
        }
      }
      u++;
    }
    if (u % 2 == 0 )
      i--;
    ///////////////////////////////////////////////////////////////////////////////
    if (digitalRead(A1) == HIGH)
    {
      while (digitalRead(A1) == HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
      {
        print_number_now();
      }
      judge_on = 0;
      second_t = 1;
      min_t = 0;
      break;//当程序暂停的时候，按电源键退出显示数字函数                 暂停的时候和运行的时候关机有问题
    }
  }
  //显示数字部分结束

  //改变时间
  change_time();
  //改变时间结束
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///////////////////////////////
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///

void pickDigit(int x)  //定义pickDigit(x),其作用是开启dx端口
{
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
  switch (x)
  {
    case 1:
      digitalWrite(d1, HIGH);
      break;
    case 2:
      digitalWrite(d2, HIGH);
      break;
    case 3:
      digitalWrite(d3, HIGH);
      break;
    default:
      digitalWrite(d4, HIGH);
      break;
  }
}
void dispDec(int x)  //设定开启小数点
{
  if (second_t % 2 == 0)
    digitalWrite(p, LOW);
  else
    digitalWrite(p, HIGH);
}
void clearLEDs()  //清屏
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, HIGH);
}
// 点亮对应数字的数码管
void lightSegments(int x) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segs[i], seven_seg_digits[x][i]);
  }
}


int find_Ten(int Time)  //获取分和秒的十位
{
  int j;
  j = (Time / 10); //获取十位
  return j;     //返回十位
}

int find_Bit(int Time)   //获取分和秒的个位
{
  int j;
  j = (Time % 10);
  return j;   //返回个位
}

int change_time()   //改变时间
{
  second_t--;
  if (second_t == 0 && min_t != 0)
  {
    digitalWrite(6, HIGH);
  }

  if (second_t < 0)
  {
    second_t = 59;
    min_t--;
    if (min_t < 0)
    {
      second_t = 0;
      min_t = 0;
    }
  }

  if (second_t == 58)
  {
    digitalWrite(6, LOW);
  }
}

void print_number_now()
{
  clearLEDs();
  pickDigit(1);
  n = min_Ten;
  lightSegments(n);
  delayMicroseconds(delsmall);
  clearLEDs();
  pickDigit(2);
  dispDec(2);
  n = min_Bit;
  lightSegments(n);
  delayMicroseconds(delsmall);
  clearLEDs();
  pickDigit(3);
  n = second_Ten;
  lightSegments(n);
  delayMicroseconds(delsmall);
  clearLEDs();
  pickDigit(4);
  n = second_Bit;
  lightSegments(n);
  delayMicroseconds(delsmall);
}

void roulette(int x)  //设置轮转效果
{
  loopcount = 0;
  while (loopcount < x)
  {
    digitalWrite(a, LOW);
    pickDigit(1);
    delay(del);
    pickDigit(2);
    delay(del);
    pickDigit(3);
    delay(del);
    pickDigit(4);
    delay(del);
    digitalWrite(a, HIGH);
    digitalWrite(b, LOW);
    delay(del);
    digitalWrite(b, HIGH);
    digitalWrite(c, LOW);
    delay(del);
    digitalWrite(c, HIGH);
    digitalWrite(d, LOW);
    delay(del);
    pickDigit(3);
    delay(del);
    pickDigit(2);
    delay(del);
    pickDigit(1);
    delay(del);
    digitalWrite(d, HIGH);
    digitalWrite(e, LOW);
    delay(del);
    digitalWrite(e, HIGH);
    digitalWrite(f, LOW);
    delay(del);
    clearLEDs();
    loopcount++;
  }
}

void circles(int x)  //设置圈状效果
{
  loopcount = 0;
  while (loopcount < x)
  {
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(f, LOW);
    digitalWrite(g, LOW);
    pickDigit(1);
    delay(250);
    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    pickDigit(2);
    delay(250);
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(f, LOW);
    digitalWrite(c, HIGH);
    digitalWrite(d, HIGH);
    digitalWrite(e, HIGH);
    pickDigit(3);
    delay(250);
    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    digitalWrite(e, LOW);
    pickDigit(4);
    delay(250);
    clearLEDs();
    loopcount++;
  }
}
void change_time_forward()
{
  second_t++;
  if (second_t == 0 && min_t != 0)
  {
    digitalWrite(6, HIGH);
  }

  if (second_t >59)
  {
    second_t = 0;
    min_t++;
  }

  if (second_t == 1)
  {
    digitalWrite(6, LOW);
  }  
}



