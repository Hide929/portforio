#include "mbed.h" //本番用

#define ELE_U 0.001329
#define ELE_halfU 0.001437    
#define ELE_N 0.001545
#define ELE_halfD 0.001771
#define ELE_D 0.001998

#define RUD_R 0.001358
#define RUD_halfR 0.001544
#define RUD_N 0.001730
#define RUD_halfL 0.001868
#define RUD_L 0.002006

#define MIN_ANGLE 0.00115
#define MAX_ANGLE 0.00235 

#define er_range1 0.93
#define er_range2 0.60
#define er_range3 0.40
#define er_range4 0.01

#define rr_range1 0.93
#define rr_range2 0.60
#define rr_range3 0.40
#define rr_range4 0.01

#define ELE_TRIM 0.000032

#define count 2


DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

PwmOut pwm_ele(p21);
PwmOut pwm_rud(p22);

AnalogIn joy_ele(p19);
AnalogIn joy_rud(p20);

Serial pc(USBTX,USBRX);

DigitalIn ER_UP(p24);
DigitalIn ER_DOWN(p23);




Ticker t;

/*-----pwmの値-----*/

double pw_ele; 
double pw_rud; 

/*-----count変数-----*/
int a=0; 
int b=0;
int c=0;
int d=0;
int e=0;
int f=0;
int g=0;
int h=0;
int i=0;
int j=0;

/*-----トリム変数-----*/
double x=0;

/*-----ERのアナログ値-----*/
double er;  
double rr;  
void start();
void control();

/*-----リセット関数-----*/
void aExcept0();
void bExcept0();
void cExcept0();
void dExcept0();
void eExcept0();
void fExcept0();
void gExcept0();
void hExcept0();
void iExcept0();
void jExcept0();


void TRIM();

void ER_trim_UP();
void ER_trim_DOWN();

int main()
{
    wait(0.1);
    t.attach(&TRIM,0.25);


    while(1) {
        control();
        wait(0.1);
        
    }
}

void start(){


    pw_ele = ELE_N;
    pw_rud = RUD_N;

    pwm_ele.pulsewidth(pw_ele);
    pwm_rud.pulsewidth(pw_rud);

}

void control(){
    er = joy_ele.read();
    rr = joy_rud.read();


    //pc.printf("er: %lf\r\n",er);
    //pc.printf("rr: %lf\r\n",rr);

    if(er > er_range1){
        aExcept0();
        a++;
        //pc.printf("a: %d\r\n",a);
        if(a>count){
            pw_ele =  ELE_D + x;
        }
        
        
    } else if(er < er_range1 && er_range2 < er){
        bExcept0();
        b++;
        //pc.printf("b: %d\r\n",b);
        if(b>count){
        pw_ele =  ELE_halfD + x;
        }

    } else if(er < er_range2 && er_range3 < er){
        cExcept0();
        c++;
        if(c>count){
        pw_ele =  ELE_N  + x;
        }

    } else if(er < er_range3 && er_range4 < er){
        dExcept0();
        d++;
        //pc.printf("d: %d\r\n",d);
        if(d>count){
        pw_ele =  ELE_halfU  + x;  
        }
        
    } else if(er_range4 > er){
        eExcept0();
        e++;
       // c.printf("e: %d\r\n",e);
        if(e>count){
        pw_ele =  ELE_U + x;  //ELE_U
        }
    }


    if(rr > rr_range1){
        fExcept0();
        f++;
        //pc.printf("f: %d\r\n",f);
        if(f>count){
        pw_rud =  RUD_R;
        }
        
    } else if(rr < rr_range1 && rr_range2 < rr){
        gExcept0();
        g++;
        //pc.printf("g: %d\r\n",g);
        if(g>count){
        pw_rud =  RUD_halfR;
        }

    } else if(rr < rr_range2 && rr_range3 < rr){
        hExcept0();
        h++;
        if(h>count){
        pw_rud =  RUD_N;
        }

    } else if(rr < rr_range3 && rr_range4 < rr){
        iExcept0();
        i++;
        //pc.printf("i: %d\r\n",i);
        if(i>count){
        pw_rud =  RUD_halfL;
        }

    } else if(rr_range4 > rr){
        jExcept0();
        j++;
        //pc.printf("j: %d\r\n",j);
        if(j>count){
        pw_rud =  RUD_L;
        }
    }




    if(pw_ele<MIN_ANGLE) {
        pw_ele = MIN_ANGLE;
    }
    if(pw_ele>MAX_ANGLE) {
        pw_ele = MAX_ANGLE;
    }
    if(pw_rud<MIN_ANGLE) {
        pw_rud = MIN_ANGLE;
    }
    if(pw_rud>MAX_ANGLE) {
        pw_rud = MAX_ANGLE;
    }

   /* pc.printf("a: %d\r\n",a);
    pc.printf("b: %d\r\n",b);
    pc.printf("c: %d\r\n",c);
    pc.printf("d: %d\r\n",d);
    pc.printf("e: %d\r\n",e);
    pc.printf("f: %d\r\n",f);
    pc.printf("g: %d\r\n",g);
    pc.printf("h: %d\r\n",h);
    pc.printf("i: %d\r\n",i);
    pc.printf("j: %d\r\n",j); */

    pc.printf("ele_pwidth: %lf\r\n",pw_ele); //エレベータの出力値
    pc.printf("rud_pwidth: %lf\r\n",pw_rud); //ラダーの出力値

    printf("%lf\r\n",x);  //トリム変数

    pwm_ele.pulsewidth(pw_ele);
    pwm_rud.pulsewidth(pw_rud);
}

void aExcept0(){
    b=0;
    c=0;
    d=0;
    e=0;

}

void bExcept0(){
    a=0; 
    c=0;
    d=0;
    e=0;
}

void cExcept0(){
    a=0; 
    b=0;
    d=0;
    e=0;
  
}

void dExcept0(){
    a=0; 
    b=0;
    c=0;
    e=0;
}

void eExcept0(){
    a=0; 
    b=0;
    c=0;
    d=0;
}

void fExcept0(){
    g=0;
    h=0;
    i=0;
    j=0;
}

void gExcept0(){
    f=0;
    h=0;
    i=0;
    j=0;
}

void hExcept0(){
    f=0;
    g=0;
    i=0;
    j=0;
}

void iExcept0(){
    f=0;
    g=0;
    h=0;
    j=0;
}

void jExcept0(){
    f=0;
    g=0;
    h=0;
    i=0;
}

void ER_trim_UP()
{
    if(ER_UP.read() == 1){
        x = x - ELE_TRIM;
        led1 = 1;
        //wait(0.25);
        
    }else{
        led1 = 0;
    }
}

void ER_trim_DOWN()
{
    if(ER_DOWN.read() == 1){
        x = x + ELE_TRIM;
        led2 = 1;
        //wait(0.25);
    }else{
        led2 = 0;
    }
}

void TRIM(){
     ER_trim_UP();
     ER_trim_DOWN();
}
