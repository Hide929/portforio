#include <mbed.h> //受信側プログラム
Serial pc(USBTX, USBRX);
Serial Android_twe(p9, p10); // tx Android rx twelite
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
// DigitalOut led1(p15);

Ticker ticker_update;


#define speed_ReceptLength 6
#define ReceptLength 6
#define SendLength 7
/*高度計*/
#define so 195.998
#define do1 261.626
#define mi 329.628
#define sifratto 466.164
#define dofratto 493.883

#define sosyapu 207.652
#define dosyapu 277.183
#define misyapu 349.228
#define si 493.883
#define do2 523.251

Ticker dis;


DigitalOut myled(LED1);
float mm[] = {so, do1, mi, sifratto, dofratto, sosyapu, dosyapu, misyapu, si, do2 * 2};
AnalogIn vin(p20);
PwmOut sp1(p26);
double ain = 0;

/* プロトタイプ宣言 */
void twe_rpm();
void twe_recieve();
void update();
void send_to_android();
void send();
void distance();
int spdPacket[12]; //速度データを入れる配列です
// int altPacket[6]; //高度データを入れる配列です
int spdCk; //速度データのチェックサムを入れる変数です
int rpmCk; //回転数のデータのチェックサムを入れる変数です
int speed_2;
double speed_1;
double altval;
char rec_deta[ReceptLength] = {};

int rpm;
int rpm2;
void cal();
void receive();
void alt();
void senda(double);

int main()
{
    /*-------ポートレート設定--------*/
    pc.baud(115200);
    Android_twe.baud(115200);

    /*-------シリアル通信設定-------*/
    Android_twe.format(8, Serial::None, 1);
    Android_twe.attach(receive, Serial::RxIrq);

    myled1 = 1;

    ticker_update.attach(&update, 0.5);

    while (1)
    {
    }
}

void update()
{

    myled2 = 1;
    
    send_to_android();
    distance();
    myled2 = 0; //送信完了
}

void send_to_android()
{
    spdPacket[0] = 'S'; //速度データの始まりを示します
    spdPacket[3] = speed_2 % 10;
    speed_2 /= 10;               // 1の位
    spdPacket[2] = speed_2 % 10; // 10の位
    speed_2 /= 10;               // 10の位
    spdPacket[1] = speed_2 % 10;
    spdCk = spdPacket[1] + spdPacket[2] + spdPacket[3]; //チェックサムを計算します
    spdPacket[4] = (spdCk / 10);                        //チェックサム10の位
    spdPacket[5] = (spdCk % 10);                        //チェックサム1の位

    spdPacket[6] = 'R';       //同様に回転数データの始まりを示します
    spdPacket[9] = rpm2 % 10; // 1の位
    rpm2 /= 10;
    spdPacket[8] = rpm2 % 10; // 10の位
    rpm2 /= 10;
    spdPacket[7] = rpm2 % 10; // 100の位
    rpmCk = spdPacket[7] + spdPacket[8] + spdPacket[9];
    spdPacket[10] = (rpmCk / 10);
    spdPacket[11] = (rpmCk % 10);

    

    for (int i = 0; i < 12; i++)
    {
        Android_twe.putc(spdPacket[i]); // putcで一文字ずつ送ります。
    }

    //pc.printf("\r\n");
}

void receive()
{
    if (Android_twe.getc() == 0xA5)
    {
        if (Android_twe.getc() == 0x5A)
        {
            // pc.printf("succcess!\r\n");
            for (int i = 0; i < ReceptLength; i++)
            {
                rec_deta[i] = Android_twe.getc(); //ヘッダ以降のデータ格納
            }
            myled3 = 1;
            wait(0.01);
            myled3 = 0;
        }
    }
    cal();
    
}

void cal()
{
    if (rec_deta[2] == 0x01)
    {
        speed_2 = rec_deta[5] - '0' + 48; // char型からint型に変換
        speed_1 = (float)speed_2;
        speed_1 = speed_1 / 100;
        pc.printf("%.2lf m/s\r\n", speed_1);
    }

    if (rec_deta[2] == 0x04)
    {
        rpm = rec_deta[5] - '0';
        rpm2 = rpm + 47; // -1~126--ミス--
        pc.printf("rpm:%d\r\n", rpm2);
    }
}

void distance()
{
    ain = vin.read() * 3.3;
    altval = ain*1000/1.6/100;
    printf("ain=%.4fV,altitude=%.1f\r\n", ain, altval);
    if(altval<2.0 or altval>10.0)
    {
        for (int i = 0; i < 9; i++)
        {
            sp1.period(1 / mm[i]);
            sp1.write(0.1);
            wait(0.05);
        }
    }
    //senda(ain*1000/1.6/100);
    sp1.write(0.0);
    
}
/*void senda(double val)
{
   
    val *= 10;
    int index[3];
    index[0] = val/ 100;
    index[1] = (val - index[0] * 100)/10;
    index[2] = (val-index[0]*100 -index[1]*10) / 1;
   
    
}*/

