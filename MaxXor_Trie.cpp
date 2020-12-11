#include <bits/stdc++.h>
#define FORN(i,n) for(int i=0;i<n;i++)
#define minf numeric_limits<int>::min()
#define inf numeric_limits<int>::max()
#define len(a) a.size()
#define vec vector
using namespace std;

class Trie{
public:
    Trie *leftField;
    Trie *rightField;
    Trie()
    {
        leftField=NULL;
        rightField=NULL;
    }
};

void insert(Trie *root , int n){
    Trie *cur= root;
    for(int i=31;i>=0;i--){
        int b= (n>>i)&1;
        if(b==0){
            if(!cur->leftField)
            cur->leftField = new Trie();
            cur= cur->leftField;
        }
        else{
            if(!cur->rightField)
            cur->rightField= new Trie();
            cur= cur->rightField;
        }
    }
}

int getMaxXor(vec<int> &a, Trie * root,int c)
{
    int maxi=0, n= a.size();
    for(int i=0;i<n;i++){
        int cur_xor=0;
        Trie* cur=root;
        for(int j=31;j>=0;j--){
            //cout<<"a[i] = "<<a[i]<<" j= "<<j;
            int b= (a[i]>>j)&1;
            //cout<<" b= "<<b<<endl;
            if(b==0){
                if(cur->rightField)
                {
                    cur_xor= cur_xor+ (1<<j);
                    cur= cur->rightField;
                }
                else
                {
                    cur= cur->leftField;
                }
            }
            else{
                if(cur->leftField){
                    cur_xor= cur_xor+ (1<<j);
                    cur= cur->leftField;
                }
                else{
                    cur= cur->rightField;
                }
            }
        }

    maxi= max(maxi , cur_xor);
    }
    return maxi;

}

int max_xor(vec<int> &a,Trie *root,int c){
    int maxi=INT_MIN, n= a.size();
        int cur_xor=0;
        Trie* cur=root;
    for(int j=31;j>=0;j--){
        int b= (c>>j)&1;
        if(b==0){
            if(cur->rightField)
            {
                cur_xor+=pow(2,j);
                cur= cur->rightField;
            }
            else
            {
                cur= cur->leftField;
            }
        }
        else{
            if(cur->leftField){
                cur_xor+= (1<<j);
                cur= cur->leftField;
            }
            else{
                cur= cur->rightField;
            }
        }
    }

    maxi= max(maxi , cur_xor);
    return maxi;
}



// Complete the maxXor function below.
vector<int> maxXor(vector<int> arr, vector<int> queries) {
    int n=arr.size()  , i , j;
    Trie *root= new Trie();
    for(int i=0;i<n;i++)
    {
        insert(root,arr[i]);
    }
    i=0;
    vector<int> x;
    while(i<queries.size()){
        int c=queries[i];
        int max_xor_value=max_xor(arr,root,c);;
        x.push_back(max_xor_value);
        i++;
    }
    // solve here
    return x;
}

int main(){

    int N,q;
    cin>>N>>q;
    vec<int> numbers,queries;
    for(int i=0;i<N;i++){
        int data;
        cin>>data;
        numbers.push_back(data); 
    }
    //for(int i=0;i<N;i++) cout<<numbers[i]<<"  ";
    while(q>0){
        //cout<<"Entered here\n";
        int c;
        cin>>c;
        queries.push_back(c);
        q--;        
    }
    vector<int> result=maxXor(numbers,queries);
    for(auto i:result) cout<<i<<endl;
    return 0;
}