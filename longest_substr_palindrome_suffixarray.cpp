#include<bits/stdc++.h>
#define FORN(i,n) for(int i=0;i<n;i++)
#define minf numeric_limits<int>::min()
#define inf numeric_limits<int>::max()
#define len(a) a.size()
#define vec vector
using namespace std;

vector<int> perm,clas;

template<typename K> istream & operator>>(istream& in, vec<K> &a){
    FORN(i,a.size()) in>>a[i];
    return in;
}   
template<typename K> ostream & operator<<(ostream& out, vec<K> &a){
    FORN(i,a.size()) out<<a[i]<<" ";
    out<<endl;
    return out;
}  
template<typename K> K Max(vec<K> a){
    K ans=numeric_limits<K>::min();
    FORN(i, a.size()) ans=max(ans, a[i]);
    return ans;
}
template<typename K> K Min(vec<K> a){
    K ans=numeric_limits<K>::max();
    FORN(i, a.size()) ans=min(ans, a[i]);
    return ans;
}


template<typename T>
struct segtree{
    int n; vec<T> t;
    T identity;
    T (*func)(T , T);
    segtree(vec<T> &a, T (*func)(T,T),T identity):n(len(a)),func(func),identity(identity){
        t.resize(len(a)*4+5, identity);
        build(a, 1, 0, len(a)-1);
    }
 
    void build(vec<T> &a, int v, int tl, int tr){
        if(tl==tr) t[v]=a[tl];
        else {
            int tm=(tl+tr)/2;
            build(a,2*v,tl,tm);
            build(a,2*v+1,tm+1,tr);
            t[v]=(*func)(t[2*v],t[2*v+1]);
        }
    }
    int query(int v, int tl, int tr, int le ,int ri){
        if(le>ri || tl>ri || tr<le) return identity;
        if(le<=tl && ri>=tr) return t[v];
        int tm=(tl+tr)/2;
        return (*func)(query(v*2,tl,tm,le,ri),query(v*2+1,tm+1,tr,le,ri));
    }
 
    void update(int v, int tl , int tr, int pos, int val){
        if(tl==tr) t[v]=val;
        else{
            int tm=(tl+tr)/2;
            if(pos>=tl && pos<=tm) update(v*2,tl,tm,pos,val);
            else if(pos>=tm+1 && pos<=tr) update (v*2+1,tm+1,tr,pos,val);
            t[v]=(*func)(t[v*2],t[v*2+1]);
        }
    }
};

vector<int> produce_shifts(vector<int> &p,vector<int> &c, vector<int> &cnt,int n,int classes){
     vector<int> pn(n), cn(n);
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    perm=p;
    clas=cn;
    return p;
}
vector<int> sort_cyclic_shifts(string const& s) {
    int n = s.size();
    const int alphabet = 256;
    vector<int> p(n), c(n), cnt(std::max(alphabet, n), 0);

    for (int i = 0; i < n; i++){
        cnt[s[i]]++;
        //cout<<cnt[s[i]]<<" -> "<<s[i]<<"  ";
    }
    //cout<<"======================cnt array==================\n";
    for (int i = 1; i < alphabet; i++){
        char c=cnt[i];
       // cout<<"Before==="<<cnt[i]<<" ->";
        cnt[i] += cnt[i-1];
       // cout<<"  char  ="<<i<<"   ";
       // cout<<"After==="<<cnt[i]<<"\n";

    }
    //cout<<"======================cnt updated array==================\n";

    for (int i = 0; i < n; i++){
        //cout<<cnt[s[i]]<<"  "<<i<<"  \n";
        p[--cnt[s[i]]] = i;

    }
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        //cout<<s[p[i-1]]<<"  "<<s[p[i]]<<"\n";
        if (s[p[i]] != s[p[i-1]]){
            classes++;
        //   cout<<classes<<"\n";
        }
        c[p[i]] = classes - 1;
    }
    return produce_shifts(p,c,cnt,n,classes);
   
}
vector<int> suffix_array_construction(string s) {
    s += "$";
    vector<int> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}

vector<int> lcp_construction(string const& s, vector<int> const& p) {
    int n = s.size();
    vector<int> rank(n, 0);
    for (int i = 0; i < n; i++)
        rank[p[i]] = i;

    int k = 0;
    vector<int> lcp(n-1, 0);
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = p[rank[i] + 1];
        while (i + k < n && j + k < n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}
/*
vector<int> kasai(string txt, vector<int> suffixArr) { 
    int n = suffixArr.size(); 
    vector<int> lcp(n, 0); 
    vector<int> invSuff(n, 0); 
    for (int i=0; i < n; i++) 
        invSuff[suffixArr[i]] = i; 
    int k = 0; 
    for (int i=0; i<n; i++) 
    {     
        if (invSuff[i] == n-1) 
        { 
            k = 0; 
            continue; 
        } 
        int j = suffixArr[invSuff[i]+1]; 
        while (i+k<n && j+k<n && txt[i+k]==txt[j+k]) 
            k++; 
        lcp[invSuff[i]] = k;  
        if (k>0) 
            k--; 
    } 
    return lcp; 
} 
*/
int mini(int a, int b) {return min(a, b);}
int main(){

    vector<int> shifts;
    string s;
    cin>>s;
    string s2=s;
    reverse(s2.begin(),s2.end());
    //cout<<"Reverse "<<s2<<endl;
    string str=s+"#"+s2;
    //cout<<str<<endl;
    //cout<<s1<<endl;
    int n=s.size();

    int actualLen=len(s);
    //cout<<"length augmented = "<<length_augmented<<endl;
    int index_needed=0;
    shifts=suffix_array_construction(str);
    //shifts.erase(shifts.begin());

    vector<int> lcp=lcp_construction(str,shifts);
    lcp.insert(lcp.begin(),0);
    //sort(lcp.begin(),lcp.end());
    //vector<int> v=kasai(s,shifts);
    //for(auto i:shifts) cout<<i<<"  ";
    //cout<<"\n";
    //for(auto i:lcp) cout<<i<<" ";
    //cout<<"\n";
    /*
    for(int i=0;i<length_original-k;i++){
        string temp1=s.substr(shifts[i]);

        string temp2=s.substr(shifts[i+k-1]);


    }
    
    for(int i = 0; i < k-1; i++) st.insert(lcp[i]);
    int max_length = *st.begin();
    for(int i = k-1; i <length_original; i++){
        st.erase(st.begin());
        st.insert(lcp[i]);
        max_length = std::max(max_length,*st.begin());
    }
    cout<<max_length<<"\n";
    
    */
   int position=0,longestLength=0;
   int Len=len(str);
    for(int i=1;i<Len;i++){
        if(lcp[i]>longestLength){
            if((shifts[i-1]<actualLen && shifts[i]>actualLen) || (shifts[i]<actualLen && shifts[i-1]>actualLen)){
                longestLength=lcp[i];
                position=shifts[i];
            } 

        }
    }
    //cout<<"length of longest palindrome = "<<longestLength<<endl;
    cout<<str.substr(position,longestLength)<<endl;
    return 0;
    
}