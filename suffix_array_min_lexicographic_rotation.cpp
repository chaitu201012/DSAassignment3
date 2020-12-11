#include<bits/stdc++.h>

using namespace std;

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
    return p;
}
vector<int> sort_cyclic_shifts(string const& s) {
    int n = s.size();
    const int alphabet = 256;
    vector<int> p(n), c(n), cnt(max(alphabet, n), 0);

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


int main(){

    vector<int> shifts;
    string s;
    cin>>s;
    string s1=s+s;
    //cout<<s1<<endl;
    int length_original=s.size();
    //cout<<"length original = "<<length_original<<endl;

    int length_augmented=s1.size();
    //cout<<"length augmented = "<<length_augmented<<endl;
    int index_needed=0;
    shifts=suffix_array_construction(s1);
    for(int i=0;i<length_augmented;i++){ 
        if(0<=shifts[i] && shifts[i]<=length_original-1){
            index_needed=shifts[i];
            //cout<<"index_needed = "<<index_needed<<endl;
            break;
        }
        //cout<<shifts[i]<<" ->"<<s1.substr(shifts[i]) <<endl;
        

    }
    cout<<s1.substr(index_needed,length_original)<<endl;
    //cout<<"Minimum lexi "<<s.substr(s.find(s[shifts[0]]))+s.substr(0,s.find(s[shifts[0]]))<<endl;
    return 0;
}