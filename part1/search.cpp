// Do NOT add any other includes
#include "search.h"


bool is_separator(char x){
    string separators = " .,-:!\"\'()?[];@";
    for(auto i:separators){
        if(i==x) return true;
    }
    return false;
}
void insertAtHead(Node* head, Node* node, int &n_matches){
    if(head->right == NULL){
        head->right = node;
        node->right = NULL;
        node-> left = head;
    }else{
        Node* temp = head->right;
        head->right = node;
        temp->left = node;
        node->left = head;
        node->right = temp;
    }
    n_matches++;
}

SearchEngine::SearchEngine(){ 
}

SearchEngine::~SearchEngine(){  
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    sentence_info s;
    for (int i = 0; i < sentence.size(); ++i)
    {
        int asc=int(sentence[i]);
        if(asc>64 && asc<91) sentence[i]=char(asc+32);
    }
    s.sentence=sentence;
    s.metadata[0]=book_code;
    s.metadata[1]=page;
    s.metadata[2]=paragraph;
    s.metadata[3]=sentence_no;
    library.push_back(s);
    return;
}

void pattern_util(string& pattern,vector<int>& v){
    v[0]=-1;
    int i = 1;
    int cndt=0;
    while( i < pattern.size())
    {
        if(pattern[cndt]==pattern[i]){
            v[i]=v[cndt];
        }
        else{
            v[i]=cndt;
            while(cndt>=0 && pattern[cndt]!=pattern[i]){
                cndt=v[cndt];
            }
        }
        i++;
        cndt++;
    }
    v[i]=cndt;
}


Node* SearchEngine::search(string pattern, int& n_matches){
    
    
    std::vector<int> v(pattern.size()+1);
    pattern_util(pattern,v);
    int n = pattern.size();
    Node* head=new Node();
    head->left=NULL;
    head->right=NULL;
    for (int i = 0; i < pattern.size(); ++i)
    {
        int asc=int(pattern[i]);
        if(asc>64 && asc<91) pattern[i]=char(asc+32);
    }
    for(auto s:library){
        int i=0;//runs in sentence
        int j=0;//runs in pattern
        while(i<s.sentence.size()){
            if(s.sentence[i]==pattern[j]){
                i++;
                j++;
                if(j==n){
                    Node * N = new Node(s.metadata[0],s.metadata[1],s.metadata[2],s.metadata[3],i-j);
                    insertAtHead(head,N,n_matches);
                    j=v[j];
                }
            }
            else{
                j=v[j];
                if(j<0){
                    i++;
                    j++;
                }
            }    
        }
    }
    Node* ret=head->right;
    if(ret)
    {
        ret->left=NULL;
    }
    head->right=NULL;
    delete head;
    return ret;
}