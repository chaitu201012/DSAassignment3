#include<bits/stdc++.h>
#define FORN(i,n) for(ll i=0;i<n;i++)
#define minf numeric_limits<ll>::min()
#define inf numeric_limits<ll>::max()
#define len(a) a.size()
#define vec vector

#define ll long long 

using namespace std;

vector<ll> perm,clas;

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
    ll n; vec<T> t;
    T identity;
    T (*func)(T , T);
    segtree(vec<T> &a, T (*func)(T,T),T identity):n(len(a)),func(func),identity(identity){
        t.resize(len(a)*4+5, identity);
        build(a, 1, 0, len(a)-1);
    }
 
    void build(vec<T> &a, ll v, ll tl, ll tr){
        if(tl==tr) t[v]=a[tl];
        else {
            ll tm=(tl+tr)/2;
            build(a,2*v,tl,tm);
            build(a,2*v+1,tm+1,tr);
            t[v]=(*func)(t[2*v],t[2*v+1]);
        }
    }
    ll query(ll v, ll tl, ll tr, ll le ,ll ri){
        if(le>ri || tl>ri || tr<le) return identity;
        if(le<=tl && ri>=tr) return t[v];
        ll tm=(tl+tr)/2;
        return (*func)(query(v*2,tl,tm,le,ri),query(v*2+1,tm+1,tr,le,ri));
    }
 
    void update(ll v, ll tl , ll tr, ll pos, ll val){
        if(tl==tr) t[v]=val;
        else{
            ll tm=(tl+tr)/2;
            if(pos>=tl && pos<=tm) update(v*2,tl,tm,pos,val);
            else if(pos>=tm+1 && pos<=tr) update (v*2+1,tm+1,tr,pos,val);
            t[v]=(*func)(t[v*2],t[v*2+1]);
        }
    }
};

vector<ll> produce_shifts(vector<ll> &p,vector<ll> &c, vector<ll> &cnt,ll n,ll classes){
     vector<ll> pn(n), cn(n);
    for (ll h = 0; (1 << h) < n; ++h) {
        for (ll i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (ll i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (ll i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (ll i = n-1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (ll i = 1; i < n; i++) {
            pair<ll, ll> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            pair<ll, ll> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
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
vector<ll> sort_cyclic_shifts(string const& s) {
    ll n = s.size();
    const ll alphabet = 256;
    vector<ll> p(n), c(n), cnt(std::max(alphabet, n), 0);

    for (ll i = 0; i < n; i++){
        cnt[s[i]]++;
        //cout<<cnt[s[i]]<<" -> "<<s[i]<<"  ";
    }
    //cout<<"======================cnt array==================\n";
    for (ll i = 1; i < alphabet; i++){
        char c=cnt[i];
       // cout<<"Before==="<<cnt[i]<<" ->";
        cnt[i] += cnt[i-1];
       // cout<<"  char  ="<<i<<"   ";
       // cout<<"After==="<<cnt[i]<<"\n";

    }
    //cout<<"======================cnt updated array==================\n";

    for (ll i = 0; i < n; i++){
        //cout<<cnt[s[i]]<<"  "<<i<<"  \n";
        p[--cnt[s[i]]] = i;

    }
    c[p[0]] = 0;
    ll classes = 1;
    for (ll i = 1; i < n; i++) {
        //cout<<s[p[i-1]]<<"  "<<s[p[i]]<<"\n";
        if (s[p[i]] != s[p[i-1]]){
            classes++;
        //   cout<<classes<<"\n";
        }
        c[p[i]] = classes - 1;
    }
    return produce_shifts(p,c,cnt,n,classes);
   
}
vector<ll> suffix_array_construction(string s) {
    s += "$";
    vector<ll> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}

vector<ll> lcp_construction(string const& s, vector<ll> const& p) {
    ll n = s.size();
    vector<ll> rank(n, 0);
    for (ll i = 0; i < n; i++)
        rank[p[i]] = i;

    ll k = 0;
    vector<ll> lcp(n-1, 0);
    for (ll i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        ll j = p[rank[i] + 1];
        while (i + k < n && j + k < n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}
/*
vector<ll> kasai(string txt, vector<ll> suffixArr) { 
    ll n = suffixArr.size(); 
    vector<ll> lcp(n, 0); 
    vector<ll> invSuff(n, 0); 
    for (ll i=0; i < n; i++) 
        invSuff[suffixArr[i]] = i; 
    ll k = 0; 
    for (ll i=0; i<n; i++) 
    {     
        if (invSuff[i] == n-1) 
        { 
            k = 0; 
            continue; 
        } 
        ll j = suffixArr[invSuff[i]+1]; 
        while (i+k<n && j+k<n && txt[i+k]==txt[j+k]) 
            k++; 
        lcp[invSuff[i]] = k;  
        if (k>0) 
            k--; 
    } 
    return lcp; 
} 
*/
ll mini(ll a, ll b) {return min(a, b);}
int main(){

	ll k,N;
	
    vector<ll> shifts;
    string s;
    cin>>s;
    
    string s1=s+s;
    set<ll> st;
    //cout<<"Enter k\n";
    cin>>k;
    
    //cout<<s1<<endl;
    ll n=s.size();
    //cout<<"length original = "<<length_original<<endl;

    ll length_augmented=s1.size();
    //cout<<"length augmented = "<<length_augmented<<endl;
    ll index_needed=0;
    shifts=suffix_array_construction(s);
    //shifts.erase(shifts.begin());

    vector<ll> lcp=lcp_construction(s,shifts);
    lcp.push_back(0);
    //vector<ll> v=kasai(s,shifts);
    //for(auto i:lcp) cout<<i<<"  ";
    //cout<<"\n";
    //for(auto i:lcp) cout<<s.substr(shifts[i])<<":: ";
    //cout<<"\n";
    /*
    for(ll i=0;i<length_original-k;i++){
        string temp1=s.substr(shifts[i]);

        string temp2=s.substr(shifts[i+k-1]);


    }
    
    for(ll i = 0; i < k-1; i++) st.insert(lcp[i]);
    ll max_length = *st.begin();
    for(ll i = k-1; i <length_original; i++){
        st.erase(st.begin());
        st.insert(lcp[i]);
        max_length = std::max(max_length,*st.begin());
    }
    cout<<max_length<<"\n";
    
    */
   segtree<ll> t(lcp, mini, inf);
	ll ans=0;
	FORN(i, n+1-k){
		ans=max(ans, t.query(1,0, t.n-1, i, i+k-2));
	} 
	cout<<ans<<endl;
	/*
    if(k>1){
        if(ans>0) cout<<ans<<endl;
        else cout<<-1<<endl;
    }
    else cout<<-1<<endl;
	*/



    return 0;
    
}