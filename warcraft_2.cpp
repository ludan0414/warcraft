#include <bits/stdc++.h>
#define rep(a,b,c)	for (int (a)=b;a<=c;a++)
#define per(a,b,c)	for (int (a)=b;a>=c;a--)
using namespace std;
map<string,int> cntr,cntb,hp;
int t,rm,bm;
int rid,bid;
string rord[5]={"iceman","lion","wolf","ninja","dragon"};
string bord[5]={"lion","dragon","ninja","iceman","wolf"};
void init(){
    cntr.clear();
    cntb.clear();
    hp.clear();
    t=0,rm=bm=0;
    rid=bid=0;
}
string weapon[3]={"sword","bomb","arrow"};
int main(){
    int tme;
    cin>>tme;
    for (int _=1;_<=tme;_++){
        init();
        printf("Case:%d\n",_);
        cin>>rm;
        bm=rm;
        int tmp;
        cin>>tmp;hp["dragon"]=tmp;
        cin>>tmp;hp["ninja"]=tmp;
        cin>>tmp;hp["iceman"]=tmp;
        cin>>tmp;hp["lion"]=tmp;
        cin>>tmp;hp["wolf"]=tmp;
        bool fb=1,fr=1;
        for (t=0;fb|fr;t++){
            if (fr){
                int mem=rid;
                while (rm<hp[rord[rid%5]]){
                    rid++;
                    if (rid==mem+5){
                        fr=0;
                        printf("%.3d red headquarter stops making warriors\n",t);
                        break;
                    }
                }
                if (fr){
                    int tot=0;
                    for (auto i:cntr)   tot+=i.second;
                    string name=rord[rid%5];
                    printf("%.3d red %s %d born with strength %d,%d %s in red headquarter\n",
                        t,rord[rid%5].c_str(),tot+1,hp[rord[rid%5]],cntr[rord[rid%5]]+1,rord[rid%5].c_str());
                    rm-=hp[rord[rid%5]];
                    cntr[rord[rid%5]]++;
                    rid=(rid+1)%5;
                    double morale=1.0*rm/hp[name];
                    if (name=="dragon") printf("It has a %s,and it's morale is %.2lf\n",weapon[(tot+1)%3].c_str(),morale);
                    if (name=="ninja")  printf("It has a %s and a %s\n",weapon[(tot+1)%3].c_str(),weapon[(tot+2)%3].c_str());
                    if (name=="iceman") printf("It has a %s\n",weapon[(tot+1)%3].c_str());
                    if (name=="lion")   printf("It's loyalty is %d\n",rm);
                }
            }
            if (fb){
                int mem=bid;
                while (bm<hp[bord[bid%5]]){
                    bid++;
                    if (bid==mem+5){
                        fb=0;
                        printf("%.3d blue headquarter stops making warriors\n",t);
                        break;
                    }
                }
                if (fb){
                    int tot=0;
                    for (auto i:cntb)   tot+=i.second;
                    string name=bord[bid%5];
                    printf("%.3d blue %s %d born with strength %d,%d %s in blue headquarter\n",
                        t,bord[bid%5].c_str(),tot+1,hp[bord[bid%5]],cntb[bord[bid%5]]+1,bord[bid%5].c_str());
                    bm-=hp[bord[bid%5]];
                    cntb[bord[bid%5]]++;
                    bid=(bid+1)%5;
                    double morale=1.0*bm/hp[name];
                    if (name=="dragon") printf("It has a %s,and it's morale is %.2lf\n",weapon[(tot+1)%3].c_str(),morale);
                    if (name=="ninja")  printf("It has a %s and a %s\n",weapon[(tot+1)%3].c_str(),weapon[(tot+2)%3].c_str());
                    if (name=="iceman") printf("It has a %s\n",weapon[(tot+1)%3].c_str());
                    if (name=="lion")   printf("It's loyalty is %d\n",bm);
                }
            }
        }
    }
    return 0;
}