#define pinENA    5  //A轮速度控制
#define pinENB    6  //B轮
#define pinIN1    8
#define pinIN2    9
#define pinIN3    10
#define pinIN4    11
#define line_tracking_right 2
#define line_tracking_left  4
int TrigPin_forward = 2;//shuchu
int EchoPin_forward = 3;//jieshou
int TrigPin_left = 14;//shuchu
int EchoPin_left = 15;//jieshou
float cm_forward = 0, cm_left = 0; //juli
void setup()
{
    pinMode(TrigPin_forward, OUTPUT);
    pinMode(EchoPin_forward, INPUT);
    pinMode(TrigPin_left, OUTPUT);
    pinMode(EchoPin_left, INPUT);

    pinMode(line_tracking_left, INPUT);
    pinMode(line_tracking_right, INPUT);

    pinMode(pinIN1, OUTPUT);
    pinMode(pinIN2, OUTPUT);
    //右轮

    pinMode(pinIN3, OUTPUT);
    pinMode(pinIN4, OUTPUT);
    //左轮
 
    pinMode(pinENA, OUTPUT);
    pinMode(pinENB, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    if (digitalRead(line_tracking_left) == 0 && digitalRead(line_tracking_right) == 0) //左边感应到黑边，就右转
    {
        while (digitalRead(line_tracking_left) == 0 && digitalRead(line_tracking_right) == 0)
        {
            forward();
        }
    }

    if (digitalRead(line_tracking_right) == 0  &&  digitalRead(line_tracking_left) == 1)
    {
        while (digitalRead(line_tracking_right) == 0  &&  digitalRead(line_tracking_left) == 1)
        {
            left_circle();
            delay(230);
            forward();
            delay(100);
        }
    }

    if (digitalRead(line_tracking_right) == 1  &&  digitalRead(line_tracking_left) == 0)
    {
        while (digitalRead(line_tracking_right) == 1  &&  digitalRead(line_tracking_left) == 0)
        {
            right_circle();
            delay(230);
            forward();
            delay(100);            
        }
    }
}

void judge_distance_forward()
{
    digitalWrite(TrigPin_forward, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin_forward, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin_forward, LOW);
    cm_forward = pulseIn(EchoPin_forward, HIGH) / 58.0; //将回波时间换算成cm
    cm_forward = (int(cm_forward * 100.0)) / 100.0; //保留两位小数
}

void judge_distance_left()
{
    digitalWrite(TrigPin_left, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin_left, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin_left, LOW);
    cm_left = pulseIn(EchoPin_left, HIGH) / 58.0; //将回波时间换算成cm
    cm_left = (int(cm_left * 100.0)) / 100.0; //保留两位小数
}


void forward()
{
    digitalWrite(pinIN1, 0);
    digitalWrite(pinIN2, 1);
    digitalWrite(pinIN3, 1);
    digitalWrite(pinIN4, 0);
    analogWrite(pinENA, 140);
    analogWrite(pinENB, 140);
}

void backward()
{
    digitalWrite(pinIN1, 1);
    digitalWrite(pinIN2, 0);
    digitalWrite(pinIN3, 0);
    digitalWrite(pinIN4, 1);
    analogWrite(pinENA, 150);
    analogWrite(pinENB, 150);
}

void left_circle()
{
    digitalWrite(pinIN1, 1);
    digitalWrite(pinIN2, 0);
    digitalWrite(pinIN3, 1);
    digitalWrite(pinIN4, 0);
    analogWrite(pinENA, 140);
    analogWrite(pinENB, 140);
}

void right_circle()
{
    digitalWrite(pinIN1, 0);
    digitalWrite(pinIN2, 1);
    digitalWrite(pinIN3, 0);
    digitalWrite(pinIN4, 1);
    analogWrite(pinENA, 140);
    analogWrite(pinENB, 140);
}
void stop1()
{
    digitalWrite(pinIN1, 1);
    digitalWrite(pinIN2, 1);
    digitalWrite(pinIN3, 1);
    digitalWrite(pinIN4, 1);
}
