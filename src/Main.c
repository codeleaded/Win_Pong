#include "/home/codeleaded/System/Static/Library/ComponentPack.h"
#include "/home/codeleaded/System/Static/Library/ComponentML.h"
#include "/home/codeleaded/System/Static/Library/ComponentDefines.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"

#define PADDLE_1_POSX       0.05f
#define PADDLE_2_POSX       0.95f
#define PADDLE_WIDTH        0.025f
#define PADDLE_HEIGHT       0.18f
#define PADDLE_SPEED        0.50f

#define BALL_WIDTH          0.0125f
#define BALL_HEIGHT         0.025f
#define BALL_STARTSPEED     0.1f
#define SPEED_ACCSPEED      0.05f

#define PADDLE_1_COLOR      BLUE
#define PADDLE_2_COLOR      RED
#define BALL_COLOR          WHITE

typedef struct PongObject{
    Vec2 p;
    Vec2 d;
    Vec2 v;
    Pixel c;
} PongObject;

PongObject paddle1;
PongObject paddle2;
PongObject ball;
int score1 = 0;
int score2 = 0;

void Reset(){
    paddle1.p.y = 0.5f - PADDLE_HEIGHT * 0.5f;
    paddle2.p.y = 0.5f - PADDLE_HEIGHT * 0.5f;
    
    ball.p.x = 0.5f - BALL_WIDTH * 0.5f;
    ball.p.y = 0.5f - BALL_HEIGHT * 0.5f;
    ball.v.x = BALL_STARTSPEED;
    ball.v.y = 0.0f;
}

PongObject PongObject_New(Vec2 p,Vec2 d,Vec2 v,Pixel c){
    PongObject po;
    po.p = p;
    po.d = d;
    po.v = v;
    po.c = c;
    return po;
}
void PongObject_Update(PongObject* po,float t){
    po->p = Vec2_Add(po->p,Vec2_Mulf(po->v,t));

    if(po->p.x < -po->d.x){
        score2++;
        Reset();
    }
    if(po->p.x > 1.0f){
        score1++;
        Reset();
    }
    if(po->p.y < 0.0f){
        po->p.y = 0.0f;
        po->v.y *= -1.0f;
    }
    if(po->p.y > 1.0f - po->d.y){
        po->p.y = 1.0f - po->d.y;
        po->v.y *= -1.0f;
    }
}
void PongObject_Collision(PongObject* po,PongObject* target){
    if(Overlap_Rect_Rect((Rect){ po->p,po->d },(Rect){ target->p,target->d })){
        float s = Vec2_Mag(target->v);
        target->v.x *= -1.0f;
        target->v = Vec2_Mulf(Vec2_Norm(target->v),s + SPEED_ACCSPEED);
        target->v.y += po->v.y;
    }
}
void PongObject_Render(PongObject* po){
    RenderRect(po->p.x * GetWidth(),po->p.y * GetHeight(),po->d.x * GetWidth(),po->d.y * GetHeight(),po->c);
}


void Setup(AlxWindow* w){
    paddle1 = PongObject_New((Vec2){ PADDLE_1_POSX,0.0f },(Vec2){ PADDLE_WIDTH,PADDLE_HEIGHT },(Vec2){ 0.0f,0.0f },PADDLE_1_COLOR);
    paddle2 = PongObject_New((Vec2){ PADDLE_2_POSX,0.0f },(Vec2){ PADDLE_WIDTH,PADDLE_HEIGHT },(Vec2){ 0.0f,0.0f },PADDLE_2_COLOR);
    ball = PongObject_New((Vec2){ 0.0f,0.0f },(Vec2){ BALL_WIDTH,BALL_HEIGHT },(Vec2){ 0.0f,0.0f },BALL_COLOR);

    Reset();
}
void Update(AlxWindow* w){
    ReloadAlxFont(GetWidth() * 0.05f,GetHeight() * 0.1f);

    if(Stroke(ALX_KEY_W).DOWN)          paddle1.v.y = -PADDLE_SPEED;
    else if(Stroke(ALX_KEY_S).DOWN)     paddle1.v.y = PADDLE_SPEED;
    else                                paddle1.v.y = 0.0f;

    if(Stroke(ALX_KEY_UP).DOWN)         paddle2.v.y = -PADDLE_SPEED;
    else if(Stroke(ALX_KEY_DOWN).DOWN)  paddle2.v.y = PADDLE_SPEED;
    else                                paddle2.v.y = 0.0f;

    PongObject_Update(&paddle1,w->ElapsedTime);
    PongObject_Update(&paddle2,w->ElapsedTime);
    PongObject_Update(&ball,w->ElapsedTime);
    
    PongObject_Collision(&paddle1,&ball);
    PongObject_Collision(&paddle2,&ball);

	Clear(BLACK);

    PongObject_Render(&paddle1);
    PongObject_Render(&paddle2);
    PongObject_Render(&ball);

    String str = String_Format("%d : %d",score1,score2);
    RenderCStrSize(str.Memory,str.size,(GetWidth() - str.size * GetAlxFont()->CharSizeX) * 0.5f,0.0f,WHITE);
    String_Free(&str);
}
void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("Pong",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}