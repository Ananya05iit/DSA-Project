#include<bits/stdc++.h>
#include <fstream>
using namespace std;
int main()
{  int choice;
   cin>>choice;
   ifstream newfile;
   if(choice==2){
    newfile.open("sherlock2.txt");
   }
   else{
   newfile.open("sherlock.txt");
   }
    string s1;
   if (newfile.is_open()){ 
      string tp;
      while(getline(newfile, tp)){ 
        s1+=tp;
        s1+=' ';
        if(v1.size()==0){
            v1.push_back(tp.size());
        }
        else{
            v1.push_back(v1.back()+tp.size()+1);
        }
      }
      newfile.close(); 
   }
   else{
    cout<<"File not opened"<<endl;
   }
    cout<<s1;
    string s2;
    getline(cin, s2);
    if(choice==1){
        s1+="$";
        int z = s1.size();
        string Text;
        Suffix_Tree a;
        a.buildSuffixTree(s1);
        string x;
        x = s2;
        int y = 0;
        z = x.length();
        string Temp2;
        for(int i=0;i<z;i++) Temp2+=x[i];
        Temp2[z] = '\0';
        a.checkForSubString(Temp2, a.root, s1);
    }
    if(choice==2){
        SuffixTrie S(s1);
        string s2;
        getline(cin, s2);
        S.search(s2);
        cout<<"Number of occurence "<<count_trie;
    }
    if(choice==3){
        KMPSearch(s2, s1);
        cout<<"Number of Occurences: "<<count_kmp;
    }
    if(choice==4){
        search(s2,s1);
        cout<<"Number of Occurences: "<<count_fa;
    }
    
    return 0;
}