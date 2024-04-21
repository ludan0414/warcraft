#include <bits/stdc++.h>
#define maxn 35
#define rep(a,b,c)	for (int (a)=b;a<=c;a++)
#define per(a,b,c)	for (int (a)=b;a>=c;a--)
using namespace std;
int t;
string rord[5]={"iceman","lion","wolf","ninja","dragon"};
string bord[5]={"lion","dragon","ninja","iceman","wolf"};
map<string,int> hp,atk;
int M,N,R,K,T;
bool cmp(pair<int,int> a,pair<int,int> b){
    if (a.first==b.first){
        return a.second>b.second;
    }
    return a.first<b.first;
}
bool cmp2(pair<int,int> a,pair<int,int> b){
    return a.second<b.second;
}
struct Warrior{
    string name;
    bool color;//0是红，1是蓝
    int id;
    mutable int strength;
    int place;
    int dir;
    int weapon[3];
    //我们把武器弄成一个pair<int,int> 其中第一项是id，第二项是剩余使用次数
    mutable deque<pair<int,int> > weaponlist;
    int attk;
    int loyalty;
    Warrior(string s,int ptr,int M,int c){
        name=s;
        id=ptr;
        color=c;
        loyalty=0;
        if (!c)  place=0,dir=1;
        else    place=N+1,dir=-1;
        weapon[0]=weapon[1]=weapon[2]=0;
        attk=atk[s];
        strength=hp[s];
        if (s=="dragon"){
            weapon[ptr%3]=1;
        }
        if (s=="ninja"){
            weapon[ptr%3]=weapon[(ptr+1)%3]=1;
        }
        if (s=="iceman"){
            weapon[ptr%3]=1;
        }
        if (s=="lion"){
            weapon[ptr%3]=1;
            loyalty=M;
        }
        if (weapon[0])  weaponlist.push_back(make_pair(0,114514));
        if (weapon[1])  weaponlist.push_back(make_pair(1,1));
        if (weapon[2])  weaponlist.push_back(make_pair(2,2));
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
    void pickweaponsort()const{
        sort(weaponlist.begin(),weaponlist.end(),cmp);
    }
    void sortweapon()const{
        sort(weaponlist.begin(),weaponlist.end());
    }
    void throwweaponsort()const{
        sort(weaponlist.begin(),weaponlist.end(),cmp2);
    }
};
set<Warrior> samurai;
struct Base{
    int m;
    bool down;
    int ptr;//现在应该让第多少个武士出来
    int enemycnt;
    bool still;
    Base(int M){
        m=M;
        down=0;
        ptr=0;
        enemycnt=0;
        still=1;
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
    hp.clear();
    atk.clear();
    samurai.clear();
}
void solve(){
    scanf("%d%d%d%d",&M,&N,&K,&T);
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
                if (rbase.still){
                    auto p=rbase.build(0);
                    if (p.name!="no")   printf("%.3d:%.2d red %s %d born\n",hour,minute,p.name.c_str(),p.id);
                    else rbase.still=0;
                    if (p.name=="lion")   printf("Its loyalty is %d\n",p.loyalty);
                }
                if (bbase.still){
                    auto p=bbase.build(1);
                    if (p.name!="no")   printf("%.3d:%.2d blue %s %d born\n",hour,minute,p.name.c_str(),p.id);
                    else bbase.still=0;
                    if (p.name=="lion")   printf("Its loyalty is %d\n",p.loyalty);
                }
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
                bool end=0;
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
                        int del=k.strength/10;
                        k.strength-=del;
                    }
                    if (k.name=="lion"){
                        k.loyalty-=K;
                    }
                    if (k.place==0||k.place==N+1){
                        printf("%.3d:%.2d %s %s %d reached %s headquarter with %d elements and force %d\n",
                            hour,minute,k.color?"blue":"red",k.name.c_str(),k.id,k.color?"red":"blue",k.strength,k.attk);
                        if (k.color)    rbase.enemycnt++;
                        else    bbase.enemycnt++;
                        if (rbase.enemycnt>=1&&!rbase.down){
                            rbase.down=1;
                            printf("%.3d:%.2d red headquarter was taken\n",hour,minute);
                            end=1;
                        }
                        if (bbase.enemycnt>=1&&!bbase.down){
                            bbase.down=1;
                            printf("%.3d:%.2d blue headquarter was taken\n",hour,minute);
                            end=1;
                        }
                    }
                    else{
                        printf("%.3d:%.2d %s %s %d marched to city %d with %d elements and force %d\n",
                            hour,minute,k.color?"blue":"red",k.name.c_str(),k.id,k.place,k.strength,k.attk);
                    }
                    samurai.emplace(k);
                }
                if (end)    return;
                break;
            }
            case (35):{
                for (auto i=samurai.begin();i!=samurai.end();i++){
                    if (i->name!="wolf") continue;
                    for (auto j=samurai.begin();j!=samurai.end();j++){
                        if (j->place==i->place&&j->color!=i->color){
                            if (j->name=="wolf") continue;//wolf不能抢wolf
                            //i抢j的武器
                            i->throwweaponsort();
                            while (!i->weaponlist.empty()&&i->weaponlist.front().second==0) i->weaponlist.pop_front();
                            j->throwweaponsort();
                            while (!j->weaponlist.empty()&&j->weaponlist.front().second==0) j->weaponlist.pop_front();
                            j->pickweaponsort();
                            if (!j->weaponlist.empty()){
                                int wid=j->weaponlist.front().first;
                                if (wid==0){
                                    int cc=0;
                                    while (!j->weaponlist.empty()&&j->weaponlist.front().first==0&&i->weaponlist.size()<=10){
                                        i->weaponlist.push_back(j->weaponlist.front());
                                        j->weaponlist.pop_front();
                                        cc++;
                                    }
                                    printf("%.3d:%.2d %s wolf %d took %d sword from %s %s %d in city %d\n",
                                        hour,minute,i->color?"blue":"red",i->id,cc,j->color?"blue":"red",j->name.c_str(),j->id,i->place);
                                }
                                if (wid==1){
                                    int cc=0;
                                    while (!j->weaponlist.empty()&&j->weaponlist.front().first==1&&i->weaponlist.size()<=10){
                                        i->weaponlist.push_back(j->weaponlist.front());
                                        j->weaponlist.pop_front();
                                        cc++;
                                    }
                                    printf("%.3d:%.2d %s wolf %d took %d bomb from %s %s %d in city %d\n",
                                        hour,minute,i->color?"blue":"red",i->id,cc,j->color?"blue":"red",j->name.c_str(),j->id,i->place);
                                }
                                if (wid==2){
                                    int cc=0;
                                    while (!j->weaponlist.empty()&&j->weaponlist.front().first==2&&i->weaponlist.size()<=10){
                                        i->weaponlist.push_back(j->weaponlist.front());
                                        j->weaponlist.pop_front();
                                        cc++;
                                    }
                                    printf("%.3d:%.2d %s wolf %d took %d arrow from %s %s %d in city %d\n",
                                        hour,minute,i->color?"blue":"red",i->id,1,j->color?"blue":"red",j->name.c_str(),j->id,i->place);
                                }
                            }
                        }
                    }
                }
                break;
            }
            case (40):{
                set<Warrior> tobeerased,tobeadded;
                for (auto i:samurai){
                    for (auto j:samurai){
                        if (i.color!=j.color&&i.place==j.place&&i.color){//i和j发生一起争斗 这里钦定i是蓝的
                            tobeerased.emplace(i);
                            tobeerased.emplace(j);
                            int place=i.place;
                            bool op;//现在轮到谁进攻
                            if (place&1)    op=0;
                            else    op=1;
                            i.sortweapon();
                            j.sortweapon();
                            int nowi=0,nowj=0;//i当前武器的编号，j当前武器的编号
                            int row=0;
                            while (row<=300){
                                //op进攻
                                row++;
                                if (op){//蓝打红
                                    int tmpptr=nowi;//当前武器
                                    int sz=i.weaponlist.size();
                                    if (sz==0){
                                        op^=1;
                                        continue;
                                    }
                                    while (tmpptr<=nowi+10&&i.weaponlist[tmpptr%sz].second==0){
                                        tmpptr++;
                                    }
                                    if (tmpptr==nowi+11){
                                        op^=1;
                                        continue;
                                    }
                                    //选定了武器了，现在i用武器打j
                                    if (i.weaponlist[tmpptr%sz].first==0){//这是一把剑
                                        j.strength-=i.attk/5;
                                    }
                                    if (i.weaponlist[tmpptr%sz].first==1){
                                        j.strength-=i.attk*2/5;
                                        if (i.name!="ninja")    i.strength-=i.attk*2/5/2;
                                        i.weaponlist[tmpptr%sz].second--;
                                    }
                                    if (i.weaponlist[tmpptr%sz].first==2){
                                        j.strength-=i.attk*3/10;
                                        i.weaponlist[tmpptr%sz].second--;
                                    }
                                    nowi=tmpptr+1;
                                }
                                else{
                                    int tmpptr=nowj;//当前武器
                                    int sz=j.weaponlist.size();
                                    if (sz==0){
                                        op^=1;
                                        continue;
                                    }
                                    while (tmpptr<=nowj+10&&j.weaponlist[tmpptr%sz].second==0){
                                        tmpptr++;
                                    }
                                    if (tmpptr==nowj+11){
                                        op^=1;
                                        continue;
                                    }
                                    //选定了武器了，现在j用武器打i
                                    if (j.weaponlist[tmpptr%sz].first==0){//这是一把剑
                                        i.strength-=j.attk/5;
                                    }
                                    if (j.weaponlist[tmpptr%sz].first==1){
                                        i.strength-=j.attk*2/5;
                                        if (j.name!="ninja")    j.strength-=j.attk*2/5/2;
                                        j.weaponlist[tmpptr%sz].second--;
                                    }
                                    if (j.weaponlist[tmpptr%sz].first==2){
                                        i.strength-=j.attk*3/10;
                                        j.weaponlist[tmpptr%sz].second--;
                                    }
                                    nowj=tmpptr+1;
                                }
                                op^=1;
                                //判断有没有东西似了
                                if (i.strength<=0&&j.strength>0){
                                    printf("%.3d:%.2d red %s %d killed blue %s %d in city %d remaining %d elements\n"
                                        ,hour,minute,j.name.c_str(),j.id,i.name.c_str(),i.id,i.place,j.strength);
                                    //舔尸体
                                    i.throwweaponsort();
                                    while (!i.weaponlist.empty()&&i.weaponlist.front().second==0)   i.weaponlist.pop_front();
                                    j.throwweaponsort();
                                    while (!j.weaponlist.empty()&&j.weaponlist.front().second==0)   j.weaponlist.pop_front();
                                    i.pickweaponsort();
                                    while (!i.weaponlist.empty()&&j.weaponlist.size()<=10){
                                        auto tmp=i.weaponlist.front();
                                        j.weaponlist.push_back(tmp);
                                        i.weaponlist.pop_front();
                                    }
                                    if (j.name=="dragon"){
                                        printf("%.3d:%.2d %s %s %d yelled in city %d\n",
                                            hour,minute,j.color?"blue":"red",j.name.c_str(),j.id,j.place);
                                    }
                                    tobeadded.emplace(j);
                                    break;
                                }
                                if (i.strength>0&&j.strength<=0){
                                    printf("%.3d:%.2d blue %s %d killed red %s %d in city %d remaining %d elements\n"
                                        ,hour,minute,i.name.c_str(),i.id,j.name.c_str(),j.id,j.place,i.strength);
                                    //舔尸体
                                    i.throwweaponsort();
                                    while (!i.weaponlist.empty()&&i.weaponlist.front().second==0)   i.weaponlist.pop_front();
                                    j.throwweaponsort();
                                    while (!j.weaponlist.empty()&&j.weaponlist.front().second==0)   j.weaponlist.pop_front();
                                    j.pickweaponsort();
                                    while (!j.weaponlist.empty()&&i.weaponlist.size()<=10){
                                        auto tmp=j.weaponlist.front();
                                        i.weaponlist.push_back(tmp);
                                        j.weaponlist.pop_front();
                                    }
                                    if (i.name=="dragon"){
                                        printf("%.3d:%.2d %s %s %d yelled in city %d\n",
                                            hour,minute,i.color?"blue":"red",i.name.c_str(),i.id,i.place);
                                    }
                                    tobeadded.emplace(i);
                                    break;
                                }
                                if (i.strength<=0&&j.strength<=0){
                                    printf("%.3d:%.2d both red %s %d and blue %s %d died in city %d\n"
                                        ,hour,minute,j.name.c_str(),j.id,i.name.c_str(),i.id,j.place);
                                    break;
                                }
                            }
                            if (row>=300){
                                printf("%.3d:%.2d both red %s %d and blue %s %d were alive in city %d\n"
                                    ,hour,minute,j.name.c_str(),j.id,i.name.c_str(),i.id,j.place);
                                i.throwweaponsort();
                                while (!i.weaponlist.empty()&&i.weaponlist.front().second==0)   i.weaponlist.pop_front();
                                j.throwweaponsort();
                                while (!j.weaponlist.empty()&&j.weaponlist.front().second==0)   j.weaponlist.pop_front();
                                if (j.name=="dragon"){
                                    printf("%.3d:%.2d %s %s %d yelled in city %d\n",
                                        hour,minute,j.color?"blue":"red",j.name.c_str(),j.id,j.place);
                                }
                                if (i.name=="dragon"){
                                    printf("%.3d:%.2d %s %s %d yelled in city %d\n",
                                        hour,minute,i.color?"blue":"red",i.name.c_str(),i.id,i.place);
                                }
                                tobeadded.emplace(i);
                                tobeadded.emplace(j);
                            }
                        }
                    }
                }
                for (auto i:tobeerased) samurai.erase(i);
                for (auto i:tobeadded){
                    samurai.emplace(i);
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
                    Warrior k=*i;
                    k.throwweaponsort();
                    int cc[3]={0,0,0};
                    for (auto j:k.weaponlist){
                        if (j.second)   cc[j.first]++;
                    }
                    printf("%.3d:%.2d %s %s %d has %d sword %d bomb %d arrow and %d elements\n"
                        ,hour,minute,k.color?"blue":"red",k.name.c_str(),k.id,cc[0],cc[1],cc[2],k.strength);
                }
                
                break;
            }
            default:{
                //do nothing
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