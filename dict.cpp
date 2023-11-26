// Do NOT add any other includes
#include "dict.h"


trie_node::trie_node(){
    word_count=0;
}

trie_node::~trie_node(){
    word_count=0;

    return;
}

trie::trie(){
    root=new trie_node();
}

int get_idx(char x){
    int asc=int(x);
    if(asc>47 && asc<58) return (asc-22);
    if(asc>64 && asc<91) return (asc-65);
    if(asc>96 && asc<123) return (asc-97);
    if(x=='/') return 36;
    if(x=='_') return 37;
    if(asc==35) return 38;
    if(asc==36) return 39;
    if(asc==37) return 40;
    if(asc==38) return 41;
    if(asc==42) return 42;
    if(asc==43) return 43;
    if(asc==60) return 44;
    if(asc==62) return 45;
    if(asc==61) return 46;
    if(asc==94) return 47;
    if(asc==96) return 48;
    if(asc==92) return 49;
    if(asc==123) return 50;
    if(asc==124) return 51;
    if(asc==125) return 52;
    if(asc==126) return 53;
    //add other possible symbols
    return -1;
}

long long trie::search(string& word){
    trie_node* n=root;
    for(auto i:word){
        int asc=get_idx(i);
        if(asc<0) return 0;
        if(!n->child[asc]) return 0;
        n=n->child[asc];
    }
    return n->word_count;
}

void trie::insert(string& word,long long& freq){
    trie_node* n=root;
    for(auto i : word){
        int asc = get_idx(i);
        if(n->child[asc]){
            n = n->child[asc];
        }
        else{
            n->child[asc] = new trie_node();
            n = n->child[asc];
        }
    }
    n->word_count=freq;
}

void rec_del(trie_node* n){
    for (auto i:n->child)
    {
        if(i) rec_del(i);
    }
    delete n;
}

trie::~trie(){
    rec_del(root);
}

Dict::Dict(){
}

Dict::~Dict(){
    return;    
}

bool Dict::is_separator(char x){
    for(auto i:separators){
        if(i==x) return true;
    }
    return false;
}

void Dict::insert_word(string&word,long long& freq){
    t.insert(word,freq);
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    for (int i = 0; i < sentence.size(); ++i)
    {
        if(is_separator(sentence[i])) continue;
        int start=i;
        trie_node* n=t.root;
        while(i<sentence.size() && !(is_separator(sentence[i]))){
            int asc=get_idx(sentence[i]);
            if(asc<0) {i++; continue;}
            if(n->child[asc]) n=n->child[asc];
            else{
                n->child[asc]=new trie_node();
                n=n->child[asc];
            }
            i++;
        }
        n->word_count++;
        if(book_code){
            if(n->word_count==1){
                string w=sentence.substr(start,i-start);
                for(int x=0;x<w.size();x++){
                    int asc=int(w[x]);
                    if(asc>64 && asc<91) w[x]=char(asc+32);
                }
                distinct_words.push_back({n,w,0});
            }
        }
        
    }
}

long long Dict::get_word_count(string word){
    int res = t.search(word);
    if(res<0) return 0;
    return res;
}

void Dict::dump_dictionary(string filename){
    ofstream fout(filename);
    for(auto pair:distinct_words){
        fout<<pair.word<<","<<pair.node->word_count<<endl;
    }
    fout.close();
    return;
}

// int main(int argc, char const *argv[])
// {
//     Dict d;
//     d.insert_sentence(1,2,3,4,"My name is Saem");
//     // d.insert_sentence(1,2,3,4,"your game gis aaem");
// }