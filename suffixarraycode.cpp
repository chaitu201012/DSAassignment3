#include <bits/stdc++.h>
 
#define readFile freopen("inn.txt","r",stdin)
#define writeFile freopen("out.txt","w",stdout)
#define fastio ios_base::sync_with_stdio(false)
 
using namespace std;
 
const int N = 1000010;
int suffArr[N], tmpSuffArr[N], rankk[N], tmpRank[N], 
        lcp[N], plcp[N], pos[N], freq[N], lens[N], n, k;
char s[N];
 
void countingSort(int k){
    int mx = max(300,n+1), acum = 0;
    memset(freq,0,sizeof(freq));
    for(int i = 0; i < n; i++) freq[i+k < n ? rankk[i+k] : 0]++;
    for(int i = 0; i < mx; i++){
        int t = freq[i];
        freq[i] = acum;
        acum += t;
    }
    for(int i = 0; i < n; i++) tmpSuffArr[freq[suffArr[i]+k < n ? rankk[suffArr[i]+k] : 0]++] = suffArr[i];
    for(int i = 0; i < n; i++) suffArr[i] = tmpSuffArr[i];
}
 
void buildSuffArr(){
    memset(suffArr,0,sizeof(suffArr)); memset(tmpSuffArr,0,sizeof(tmpSuffArr)); 
    memset(rankk,0,sizeof(rankk)); memset(tmpRank,0,sizeof(tmpRank));
    for(int i = 0; i < n; i++) rankk[i] = s[i], suffArr[i] = i;
    for(int k = 1; k <= n; k<<=1){
        countingSort(k);
        countingSort(0);
        int r = 1;
        tmpRank[suffArr[0]] = r;
        for(int i = 1; i < n; i++){
            if(rankk[suffArr[i]]==rankk[suffArr[i-1]] && rankk[suffArr[i]+k]==rankk[suffArr[i-1]+k]) tmpRank[suffArr[i]] = r;
            else tmpRank[suffArr[i]] = ++r;
        }
        for(int i = 0; i < n; i++) rankk[i] = tmpRank[i];
    }
}
 
void buildLcp(){
    memset(pos,0,sizeof(pos)); memset(plcp,0,sizeof(plcp));
    pos[suffArr[0]] = -1;
    for(int i = 1; i < n; i++) pos[suffArr[i]] = suffArr[i-1];
    int len = 0;
    for(int i = 0; i < n; i++){
        if(pos[i]==-1){
            plcp[i] = 0;
            continue;
        }
        while(s[i+len]==s[pos[i]+len]) len++;
        plcp[i] = len;
        len = max(len-1,0);
    }
    for(int i = 0; i < n; i++) lcp[i] = plcp[suffArr[i]];
}
set<int> st;
int main() {
#ifndef ONLINE_JUDGE
    readFile;
#endif
    fastio;
    scanf("%d%d",&k,&n);
    scanf("%s",s);
    if(k==1){
        printf("%d\n",n);
        return 0;
    }
    buildSuffArr();
    buildLcp();
    for(int i = 0; i < k-1; i++) st.insert(lcp[i]);
    int res = *st.begin();
    for(int i = k-1; i < n; i++){
        st.erase(st.begin());
        st.insert(lcp[i]);
        res = max(res,*st.begin());
    }
    printf("%d\n",res);
    return 0;
}