// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


class trie_node{
public:
    trie_node* par;
    long long word_count;
    vector<trie_node*> child=vector<trie_node*>(54,nullptr);
    trie_node();
    ~trie_node();
};

struct pair_util{
    trie_node* node;
    string word;
    long long freq;
};


class trie{
public:
    trie_node* root;
    trie();
    ~trie();
    long long search(string& word);
    void insert(string& word,long long& freq);
    
};

class Dict {
private:
    string separators = " .,-:!\"\'()?[];@";
    bool is_separator(char x);
    trie t;

public: 
    vector<pair_util> distinct_words;
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    void insert_word(string&word,long long& freq);
    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};