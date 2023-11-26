#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"


using namespace std;


void insert(int bcode,int pageno,int parano,Node* head);

struct freq_word{
    freq_word* right=nullptr;
    string word;
    int freq=0;
};

class para
{   
public:
    int b_code;
    int page_no;
    int para_no;
    // Dict* d=nullptr;
    // trie* t;
    vector<freq_word*> frequencies=vector<freq_word*>(311,nullptr);
    double score;
    para();
    ~para();
};

class QNA_tool {
private:
    // You are free to change the implementation of this function
    void insert_in_corpus(para* p,string& sentence);
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    void heapifyup(vector<para*> &v,int idx);
    void heapifydn(vector<para*> &v,int idx,int sz);
    void Sort(vector<para*> &corpus,int k);
    bool is_separator(char x);
    int hash(string& word);

    vector<para*> corpus=vector<para*>(380996);//////
    trie csv;
    Dict mkg;
    int cor_size;
    string separators = " .,-:!\"\'()?[];@";
    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    std::string get_paragraph(int book_code, int page, int paragraph);

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};