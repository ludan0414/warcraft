#include <bits/stdc++.h>
#define maxn 35
#define rep(a,b,c)	for (int (a)=b;a<=c;a++)
#define per(a,b,c)	for (int (a)=b;a>=c;a--)
using namespace std;
int t;
string rord[5]={"iceman","lion","wolf","ninja","dragon"};
string bord[5]={"lion","dragon","ninja","iceman","wolf"};
map<string,int> hp,atk,cnt;
int M,N,R,K,T;
struct Warrior{
    string name;
    bool color;//0是红，1是蓝
    int id;
    mutable int strength;
    int place;
    int dir;
    int weapon[3];
    int attk;
    mutable int swordatk;
    mutable int arrowrem;
    int icestep;//iceman steps
    int loyalty;
    double morale;
    Warrior(string s,int ptr,int M_have,int c){
        name=s;
        id=ptr;
        color=c;
        icestep=0;
        loyalty=0;
        if (!c)  place=0,dir=1;
        else    place=N+1,dir=-1;
        weapon[0]=weapon[1]=weapon[2]=0;
        attk=atk[s];
        strength=hp[s];
        if (s=="dragon"){
            weapon[ptr%3]=1;
            morale=1.0*M_have/hp[s];
        }
        if (s=="ninja"){
            weapon[ptr%3]=weapon[(ptr+1)%3]=1;
        }
        if (s=="iceman"){
            weapon[ptr%3]=1;
        }
        if (s=="lion"){
            loyalty=M_have;
        }
        if (weapon[0])  swordatk=0.2*attk;
        else    swordatk=0;
        if (weapon[2])  arrowrem=3;
        else    arrowrem=0;
    }
    bool operator < (const Warrior &b)const{
        if (place==b.place){
            if (color==b.color){
                return name<b.name;
            }
            return color<b.color;
        }
        return place<b.place;
    }
};
struct City{
    int flag;//是0表示没有旗子，是1表示红方，是2表示蓝方
    int m;
    int pos;
    int prevwinner;//是0表示平局，是1表示红方，是2表示蓝方
    City(int p=0){
        flag=0;
        m=0;
        pos=p;
        prevwinner=0;
    }
};
City city[maxn];
set<Warrior> samurai;
struct Base{
    int m;
    bool down;
    int ptr;//现在应该让第多少个武士出来
    int enemycnt;
    Base(int _M){
        m=_M;
        down=0;
        ptr=0;
        enemycnt=0;
    }
    Warrior build(bool color){
        if (!color){//红
            string name=rord[ptr%5];
            if (m>=hp[name]){
                m-=hp[name];
                auto person=Warrior(name,ptr+1,m,0);
                samurai.emplace(person);
                ptr++;
                return person;
            }
            return Warrior("no",0,0,0);
        }
        else{
            string name=bord[ptr%5];
            if (m>=hp[name]){
                m-=hp[name];
                auto person=Warrior(name,ptr+1,m,1);
                samurai.emplace(person);
                ptr++;
                return person;
            }
            return Warrior("no",0,0,0);
        }
    }
};
void init(){
    rep(i,0,N+1){
        city[i]=City(i);
    }
    hp.clear();
    atk.clear();
    samurai.clear();
}
bool virtualfight(int place,Warrior wblue,Warrior wred,int col){//如果1使用炸弹，2表示不使用炸弹
    if (wred.color) swap(wred,wblue);
    Warrior mred=wred,mblue=wblue;
    bool first;//是0表示红方先手进攻，是1表示蓝方先手进攻
    if ((city[place].flag==0&&place&1)||city[place].flag==1){//红方开始进攻
        first=0;
        wblue.strength-=wred.attk+wred.swordatk;
        wred.swordatk*=0.8;
        if (wblue.strength>0){
            if (wblue.name!="ninja"){//蓝方反击
                wred.strength-=wblue.attk/2+wblue.swordatk;
                wblue.swordatk*=0.8;
            }
        }
    }
    if ((city[place].flag==0&&place&1^1)||city[place].flag==2){//蓝方开始进攻
        first=1;
        wred.strength-=wblue.attk+wblue.swordatk;
        wblue.swordatk*=0.8;
        if (wred.strength>0){
            if (wred.name!="ninja"){
                wblue.strength-=wred.attk/2+wred.swordatk;
                wred.swordatk*=0.8;
            }
        }
    }
    if (wred.color==col&&wred.strength<=0){
        return 1;
    }
    if (wblue.color==col&&wblue.strength<=0){
        return 1;
    }
    return 0;
}
void solve(){
    scanf("%d%d%d%d%d",&M,&N,&R,&K,&T);
    init();
    Base rbase(M),bbase(M);
    int tmp;
    scanf("%d",&tmp);hp["dragon"]=tmp;
    scanf("%d",&tmp);hp["ninja"]=tmp;
    scanf("%d",&tmp);hp["iceman"]=tmp;
    scanf("%d",&tmp);hp["lion"]=tmp;
    scanf("%d",&tmp);hp["wolf"]=tmp;
    scanf("%d",&tmp);atk["dragon"]=tmp;
    scanf("%d",&tmp);atk["ninja"]=tmp;
    scanf("%d",&tmp);atk["iceman"]=tmp;
    scanf("%d",&tmp);atk["lion"]=tmp;
    scanf("%d",&tmp);atk["wolf"]=tmp;
    for (int t=0;t<=T;t++){
        int hour=t/60;
        int minute=t-hour*60;
        switch (minute){
            case (0):{
                auto p=rbase.build(0);
                if (p.name!="no")   printf("%.3d:%.2d red %s %d born\n",hour,minute,p.name.c_str(),p.id);
                if (p.name=="dragon")   printf("Its morale is %.2lf\n",p.morale);
                if (p.name=="lion")   printf("Its loyalty is %d\n",p.loyalty);
                p=bbase.build(1);
                if (p.name!="no")   printf("%.3d:%.2d blue %s %d born\n",hour,minute,p.name.c_str(),p.id);
                if (p.name=="dragon")   printf("Its morale is %.2lf\n",p.morale);
                if (p.name=="lion")   printf("Its loyalty is %d\n",p.loyalty);
                break;
            }
            case (5):{
                vector<Warrior> tobeerased;
                for (auto i:samurai){
                    if (i.name=="lion"&&i.loyalty<=0){
                        if (i.place==0&&!i.color){
                            printf("%.3d:%.2d %s %s %d ran away\n",hour,minute,i.color?"blue":"red",i.name.c_str(),i.id);
                            tobeerased.push_back(i);
                        }
                        if (i.place==N+1&&i.color){
                            printf("%.3d:%.2d %s %s %d ran away\n",hour,minute,i.color?"blue":"red",i.name.c_str(),i.id);
                            tobeerased.push_back(i);
                        }
                        if (i.place>=1&&i.place<=N){
                            printf("%.3d:%.2d %s %s %d ran away\n",hour,minute,i.color?"blue":"red",i.name.c_str(),i.id);
                            tobeerased.push_back(i);
                        }
                    }
                }
                for (auto i:tobeerased){
                    samurai.erase(i);
                }
                break;
            }
            case (10):{
                set<Warrior> tobeerased;
                for (auto i:samurai){
                    if (!i.color&&i.place==N+1) continue;
                    if (i.color&&i.place==0)    continue;
                    auto k=i;
                    k.place+=k.dir;
                    tobeerased.emplace(k);
                }
                for (auto k:tobeerased){
                    auto tmp=k;
                    tmp.place-=tmp.dir;
                    samurai.erase(tmp);
                    if (k.name=="iceman"){
                        k.icestep++;
                        if (k.icestep&1^1){
                            k.strength-=9;
                            k.attk+=20;
                            if (k.strength<=0)  k.strength=1;
                        }
                    }
                    if (k.place==0||k.place==N+1){
                        printf("%.3d:%.2d %s %s %d reached %s headquarter with %d elements and force %d\n",
                            hour,minute,k.color?"blue":"red",k.name.c_str(),k.id,k.color?"red":"blue",k.strength,k.attk);
                        if (k.color)    rbase.enemycnt++;
                        else    bbase.enemycnt++;
                        if (rbase.enemycnt>=2&&!rbase.down){
                            printf("%.3d:%.2d red headquarter was taken\n",hour,minute);
                            rbase.down=1;
                        }
                        if (bbase.enemycnt>=2&&!bbase.down){
                            printf("%.3d:%.2d blue headquarter was taken\n",hour,minute);
                            bbase.down=1;
                        }
                    }
                    else{
                        printf("%.3d:%.2d %s %s %d marched to city %d with %d elements and force %d\n",
                            hour,minute,k.color?"blue":"red",k.name.c_str(),k.id,k.place,k.strength,k.attk);
                    }
                    samurai.emplace(k);
                }
                if (rbase.down||bbase.down)    return;
                break;
            }
            case (20):{
                rep(i,1,N){
                    city[i].m+=10;
                }
                break;
            }
            case (30):{
                vector<Warrior> cnt[maxn];
                for (auto i:samurai){
                    cnt[i.place].push_back(i);
                }
                rep(i,1,N){
                    if (cnt[i].size()==1){
                        printf("%.3d:%.2d %s %s %d earned %d elements for his headquarter\n",
                            hour,minute,cnt[i][0].color?"blue":"red",cnt[i][0].name.c_str(),cnt[i][0].id,city[i].m);
                        if (cnt[i][0].color){
                            bbase.m+=city[i].m;
                            city[i].m=0;
                        }
                        else{
                            rbase.m+=city[i].m;
                            city[i].m=0;
                        }
                    }
                }
                break;
            }
            case (35):{
                set<Warrior> tobechanged;
                for (auto i=samurai.begin();i!=samurai.end();i++){
                    if (i->arrowrem){//还有箭
                        for (auto j=samurai.begin();j!=samurai.end();j++){
                            if (j->color!=i->color&&j->place==i->place+i->dir&&i->place+i->dir!=0&&i->place+i->dir!=N+1){//下一个城市有敌人
                                if (j->strength>R)   printf("%.3d:%.2d %s %s %d shot\n",hour,minute,i->color?"blue":"red",i->name.c_str(),i->id);
                                else    printf("%.3d:%.2d %s %s %d shot and killed %s %s %d\n"
                                    ,hour,minute,i->color?"blue":"red",i->name.c_str(),i->id,j->color?"blue":"red",j->name.c_str(),j->id);
                                i->arrowrem-=1;
                                j->strength-=R;
                            }
                        }
                    }
                }
                break;
            }
            case (38):{
                set<Warrior> tobeerased;
                for (auto i:samurai){
                    for (auto j:samurai){
                        if (i.place==j.place&&i.color!=j.color&&i.weapon[1]){//i向j是否炸
                            if (i.strength<=0||j.strength<=0)   continue;
                            if (tobeerased.count(i)||tobeerased.count(j))   continue;
                            Warrior wblue=i,wred=j;
                            if (wred.color) swap(wred,wblue);
                            bool use=virtualfight(i.place,wblue,wred,i.color);
                            if (use){
                                printf("%.3d:%.2d %s %s %d used a bomb and killed %s %s %d\n",
                                    hour,minute,i.color?"blue":"red",i.name.c_str(),i.id,j.color?"blue":"red",j.name.c_str(),j.id);
                                tobeerased.emplace(i);
                                tobeerased.emplace(j);
                            }
                        }
                    }
                }
                for (auto i:tobeerased){
                    samurai.erase(i);
                }
                break;
            }
            case (40):{
                set<Warrior> tobeerased,tobeadded;
                set<Warrior> tobebluerewarded,toberedrewarded;
                int bluem=bbase.m,redm=rbase.m;
                for (auto i:samurai){
                    for (auto j:samurai){
                        if (i.color!=j.color&&i.place==j.place&&i.color){//i和j发生一起争斗
                            tobeerased.emplace(i);
                            tobeerased.emplace(j);
                            int place=i.place;
                            Warrior wred=i,wblue=j;
                            if (wred.color) swap(wred,wblue);
                            Warrior mred=wred,mblue=wblue;
                            bool first=(city[place].flag==0&&place&1^1)||city[place].flag==2;//是0表示红方先手进攻，是1表示蓝方先手进攻
                            if (wred.strength<=0&&wblue.strength<=0)   continue;
                            if (wred.strength<=0&&wblue.strength>0) goto BlueSurvived;
                            if (wred.strength>0&&wblue.strength<=0) goto RedSurvived;
                            if ((city[place].flag==0&&place&1)||city[place].flag==1){//红方开始进攻
                                first=0;
                                printf("%.3d:%.2d red %s %d attacked blue %s %d in city %d with %d elements and force %d\n",
                                    hour,minute,wred.name.c_str(),wred.id,wblue.name.c_str(),wblue.id,place,wred.strength,wred.attk);
                                wblue.strength-=wred.attk+wred.swordatk;
                                wred.swordatk*=0.8;
                                if (wblue.strength>0){
                                    if (wblue.name!="ninja"){//蓝方反击
                                        printf("%.3d:%.2d blue %s %d fought back against red %s %d in city %d\n",
                                            hour,minute,wblue.name.c_str(),wblue.id,wred.name.c_str(),wred.id,place);
                                        wred.strength-=wblue.attk/2+wblue.swordatk;
                                        wblue.swordatk*=0.8;
                                    }
                                }
                            }
                            if ((city[place].flag==0&&place&1^1)||city[place].flag==2){//蓝方开始进攻
                                first=1;
                                printf("%.3d:%.2d blue %s %d attacked red %s %d in city %d with %d elements and force %d\n",
                                    hour,minute,wblue.name.c_str(),wblue.id,wred.name.c_str(),wred.id,place,wblue.strength,wblue.attk);
                                wred.strength-=wblue.attk+wblue.swordatk;
                                wblue.swordatk*=0.8;
                                if (wred.strength>0){
                                    if (wred.name!="ninja"){
                                        printf("%.3d:%.2d red %s %d fought back against blue %s %d in city %d\n",
                                            hour,minute,wred.name.c_str(),wred.id,wblue.name.c_str(),wblue.id,place);
                                        wblue.strength-=wred.attk/2+wred.swordatk;
                                        wred.swordatk*=0.8;
                                    }
                                }
                            }
                            //检测谁还活着
                            if (wred.strength>0&&wblue.strength<=0){//红的活着
                                printf("%.3d:%.2d blue %s %d was killed in city %d\n",hour,minute,wblue.name.c_str(),wblue.id,place);
                                if (wblue.name=="lion") wred.strength+=mblue.strength;//lion吸血
                                RedSurvived:
                                if (wred.name=="wolf"){//wolf自动拾取
                                    if (wred.swordatk<=0)   wred.swordatk=wblue.swordatk;//sword
                                    if (wred.arrowrem<=0)   wred.arrowrem=wblue.arrowrem;//arrow
                                    wred.weapon[1]|=wblue.weapon[1];//bomb
                                }
                                if (wred.name=="dragon")  wred.morale+=0.2;//dragon欢呼
                                if (wred.name=="dragon"&&first==0&&wred.morale>0.8){
                                    printf("%.3d:%.2d red dragon %d yelled in city %d\n",hour,minute,wred.id,place);
                                }
                                if (city[place].m){// 武士拾取生命元
                                    printf("%.3d:%.2d red %s %d earned %d elements for his headquarter\n",
                                        hour,minute,wred.name.c_str(),wred.id,city[place].m);
                                    rbase.m+=city[place].m;
                                    city[place].m=0;
                                }
                                if (city[place].flag!=1&&city[place].prevwinner==1){// 更换旗帜
                                    printf("%.3d:%.2d red flag raised in city %d\n",hour,minute,place);
                                    city[place].flag=1;
                                }
                                city[place].prevwinner=1;
                                toberedrewarded.emplace(wred);
                                tobeadded.emplace(wred);
                            }
                            if (wred.strength<=0&&wblue.strength>0){//蓝的活着
                                printf("%.3d:%.2d red %s %d was killed in city %d\n",hour,minute,wred.name.c_str(),wred.id,place);
                                if (wred.name=="lion")  wblue.strength+=mred.strength;//lion吸血
                                BlueSurvived:
                                if (wblue.name=="wolf"){//wolf自动拾取
                                    if (wblue.swordatk<=0)   wblue.swordatk=wred.swordatk;//sword
                                    if (wblue.arrowrem<=0)   wblue.arrowrem=wred.arrowrem;//arrow
                                    wblue.weapon[1]|=wred.weapon[1];//bomb
                                }
                                if (wblue.name=="dragon")  wblue.morale+=0.2;//dragon欢呼
                                if (wblue.name=="dragon"&&first==1&&wblue.morale>0.8){
                                    printf("%.3d:%.2d blue dragon %d yelled in city %d\n",hour,minute,wblue.id,place);
                                }
                                if (city[place].m){// 武士拾取生命元
                                    printf("%.3d:%.2d blue %s %d earned %d elements for his headquarter\n",
                                        hour,minute,wblue.name.c_str(),wblue.id,city[place].m);
                                    bbase.m+=city[place].m;
                                    city[place].m=0;
                                }
                                if (city[place].flag!=2&&city[place].prevwinner==2){// 更换旗帜
                                    printf("%.3d:%.2d blue flag raised in city %d\n",hour,minute,place);
                                    city[place].flag=2;
                                }
                                city[place].prevwinner=2;
                                tobebluerewarded.emplace(wblue);
                                tobeadded.emplace(wblue);
                            }
                            if (wred.strength>0&&wblue.strength>0){//都活着
                                city[place].prevwinner=0;
                                if (wred.name=="dragon")    wred.morale-=0.2;
                                if (wblue.name=="dragon")    wblue.morale-=0.2;
                                if (wred.name=="dragon"&&first==0&&wred.morale>0.8){
                                    printf("%.3d:%.2d red dragon %d yelled in city %d\n",hour,minute,wred.id,place);
                                }
                                if (wblue.name=="dragon"&&first==1&&wblue.morale>0.8){
                                    printf("%.3d:%.2d blue dragon %d yelled in city %d\n",hour,minute,wblue.id,place);
                                }
                                if (wred.name=="lion")  wred.loyalty-=K;
                                if (wblue.name=="lion") wblue.loyalty-=K;
                                tobeadded.emplace(wred);
                                tobeadded.emplace(wblue);
                            }
                        }
                    }
                }
                for (auto i:tobeerased) samurai.erase(i);
                for (auto i:tobeadded) samurai.emplace(i);
                tobeerased.clear();
                for (auto i:samurai){//删一下被射死但是没有发生争斗的
                    if (i.strength<=0)  tobeerased.emplace(i);
                }
                for (auto i:tobeerased) samurai.erase(i);
                for (auto i=tobebluerewarded.begin();i!=tobebluerewarded.end();i++){
                    if (bluem>=8){
                        bluem-=8;
                        bbase.m-=8;
                        auto ptr=samurai.find(*i);
                        ptr->strength+=8;
                    }
                }
                for (auto i=toberedrewarded.rbegin();i!=toberedrewarded.rend();i++){
                    if (redm>=8){
                        redm-=8;
                        rbase.m-=8;
                        auto ptr=samurai.find(*i);
                        ptr->strength+=8;
                    }
                }
                break;
            }
            case (50):{
                printf("%.3d:%.2d %d elements in red headquarter\n",hour,minute,rbase.m);
                printf("%.3d:%.2d %d elements in blue headquarter\n",hour,minute,bbase.m);
                break;
            }
            case (55):{
                for (auto i=samurai.begin();i!=samurai.end();i++){
                    if (!i->color){
                        Warrior k=*i;
                        k.weapon[0]=k.swordatk;
                        k.weapon[2]=k.arrowrem;
                        bool first=1;
                        printf("%.3d:%.2d red %s %d has ",hour,minute,k.name.c_str(),k.id);
                        if (k.weapon[2]){
                            printf("arrow(%d)",k.weapon[2]);
                            first=0;
                        }
                        if (k.weapon[1]){
                            if (!first)  printf(",");
                            printf("bomb");
                            first=0;
                        }
                        if (k.weapon[0]){
                            if (!first)  printf(",");
                            printf("sword(%d)",k.weapon[0]);
                            first=0;
                        }
                        if (first)  printf("no weapon");
                        printf("\n");
                    }
                }
                for (auto i=samurai.begin();i!=samurai.end();i++){
                    if (i->color){
                        Warrior k=*i;
                        k.weapon[0]=k.swordatk;
                        k.weapon[2]=k.arrowrem;
                        bool first=1;
                        printf("%.3d:%.2d blue %s %d has ",hour,minute,k.name.c_str(),k.id);
                        if (k.weapon[2]){
                            printf("arrow(%d)",k.weapon[2]);
                            first=0;
                        }
                        if (k.weapon[1]){
                            if (!first)  printf(",");
                            printf("bomb");
                            first=0;
                        }
                        if (k.weapon[0]){
                            if (!first)  printf(",");
                            printf("sword(%d)",k.weapon[0]);
                            first=0;
                        }
                        if (first)  printf("no weapon");
                        printf("\n");
                    }
                }
            }
        }
    }
}
int main(){
    int t;
    scanf("%d",&t);
    rep(i,1,t){
        printf("Case %d:\n",i);
        solve();
    }
    return 0;
}