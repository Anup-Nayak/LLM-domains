#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define ignorable 90000000

//preprocess the corporus: store the paragraphs in dll. :: done in constructor of QnA tool.
//for each paragraph keep a dict object/trieif cant import dict
//the dict will store the no. of times each word comes in the para. easily get freqs.

#define ull unsigned long long

// ull power(int x, int n){
//     ull ans = 1;
//     while (n > 0) {
//         if (n%2 == 1) 
//         {
//             ans = ans * x;
//         }
//         x *=x;
//         n/=2; 
//     }
//     return ans;
// }

// int hassh(string word){
//     int h=0;
//     for(int i=0;i<word.size();i++){
//         ull a=int(word[i]);
//         ull g=i;
//         ull t=i%5;
//         ull b= a*(power(3,g))+power(a,t);
//         h+= b;
//     }
//     return h%311;
// }

int QNA_tool::hash(string& word){
    unsigned long long h=0;
    for(int i=(word.size()-1);i>=0;i--){
        h = h*31 + int(word[i]);
        h%=311;
    }
    return h%311;
}

int get_inndx(char x){
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

void QNA_tool::insert_in_corpus(para* p, string& sentence){
    for (int i = 0; i < sentence.size(); ++i)
    {
        if(is_separator(sentence[i])) continue;
        int start=i;
        while(i<sentence.size() && !(is_separator(sentence[i]))){
            int asc=int(sentence[i]);
            if(asc>64 && asc<91) sentence[i]=char(asc+32);
            i++;
        }
        string word=sentence.substr(start,i-start);
        int h=hash(word);
        freq_word* n = p->frequencies[h];
        freq_word* nprev = p->frequencies[h];
        if(!n) {
            p->frequencies[h] = new freq_word;
            p->frequencies[h]->word = word;
            p->frequencies[h]->freq++;
        }
        else{
            while(n){
                nprev=n;
                if(n->word==word){
                    n->freq++;
                    break;
                }
                n=n->right;
            }
            if(!n){
                nprev->right=new freq_word;
                nprev->right->word=word;
                nprev->right->freq++;
            }
        }
    }
}


void insert(int bcode,int pageno,int parano,Node* head){
    Node* t = new Node(bcode,pageno,parano,0,0);
    if(head->right) head->right->left = t;
    t->right = head->right;
    head->right = t;
    t->left = head;
}
//saem ne samjhaya hai galat nahi hai
void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    mkg.insert_sentence(0,0,0,0,sentence);/*remove if mkg csv is accessable on GS :'{ */
    if(!cor_size){
        corpus[cor_size]->b_code=book_code;
        corpus[cor_size]->page_no=page;
        corpus[cor_size]->para_no=paragraph;
        // corpus[cor_size]->d->insert_sentence(0,0,0,0,sentence);
        insert_in_corpus(corpus[cor_size],sentence);
        cor_size++;
    }
    else if(corpus[cor_size-1]->para_no==paragraph && corpus[cor_size-1]->page_no==page && corpus[cor_size-1]->b_code==book_code){
        // corpus[cor_size-1]->d->insert_sentence(0,0,0,0,sentence); 
        insert_in_corpus(corpus[cor_size-1],sentence);
}
    else{
        corpus[cor_size]->b_code=book_code;
        corpus[cor_size]->page_no=page;
        corpus[cor_size]->para_no=paragraph;
        // corpus[cor_size]->d->insert_sentence(0,0,0,0,sentence);
        insert_in_corpus(corpus[cor_size],sentence);
        cor_size++;
    }
    return;

}

QNA_tool::QNA_tool(){
    cor_size=0;
    ifstream cv("unigram_freq.csv");
    string f;
    string word;
    cv.ignore(11);
    while(getline(cv,word,',') && getline(cv,f)){
        istringstream iss(f);

        long long freq;
        iss>>freq;
        csv.insert(word,freq);
    }
    cv.close();

    // ifstream cm("mkgandhi_csv.csv");
    // if (!cm.is_open()) {
    //         std::cout << "Error: Unable to open the file" << std::endl;
    //     }
    // while(getline(cm,word,',') && getline(cm,f)){
    //     istringstream iss(f);
    //     long long freq;
    //     iss>>freq;
    //     mkg.insert(word,freq);
    // }
    // cm.close();
    for (int i = 0; i < corpus.size(); ++i)
    {
        corpus[i] = new para();
    }
    // string file = "Paragraphs/para-";
    // for (int i = 0; i < corpus.size(); ++i)
    // {
    //     ifstream ifile(file+to_string(i+1)+".txt");
    //     string num;
    //     getline(ifile,num,',');
    //     istringstream iss1(num);
    //     iss1>>corpus[i]->b_code;
    //     getline(ifile,num,',');
    //     istringstream iss2(num);
    //     iss1>>corpus[i]->page_no;
    //     getline(ifile,num);
    //     istringstream iss3(num);
    //     iss1>>corpus[i]->para_no;
    //     while(getline(ifile,word,',') && getline(ifile,f)){
    //         istringstream iss(f);
    //         long long freq;
    //         iss>>freq;
    //     }
    //     ifile.close();

    // }
}

para::para(){
    // d = new Dict();
    // if(!d){cout<<"No Space in para"<<endl;}
    // t = new trie();
    // if(!t){cout<<"No Space in para"<<endl;}
    
}

para::~para(){
    // delete t;
    // delete d;
    for(auto i : frequencies){
        if(i) delete i;
    }
}

QNA_tool::~QNA_tool(){
    for(auto i:corpus){
        delete i;
    }
}
//HEAPUTIL BEGINS
int par(int idx){
    return (idx-1)/2;
}
int left(int idx){
    return idx*2+1;
}
int right(int idx){
    return idx*2+2;
}


void swap(para** p1,para** p2){
    para* p3=*p1;
    *p1=*p2;
    *p2=p3;
}

int swapmaxchild(vector<para*> &v,int idx){
    int l=left(idx);
    int r=right(idx);
    int max;
    if(v[l]->score<v[r]->score){
        max=r;
    }
    else{
        max=l;
    }
    if(v[idx]->score<v[max]->score){
        swap(&v[max],&v[idx]);
    }
    else{
        return 0;
    }
    return max;
}

//{0,1,2,3,4,5,6,7}
void QNA_tool::heapifyup(vector<para*> &v,int idx){
    if(!idx) return;
    int p=par(idx);
    if(v[p]->score<v[idx]->score){
        swap(&v[p],&v[idx]);
    }
    heapifyup(v,p);
}

void QNA_tool::heapifydn(vector<para*> &v,int idx,int size){
    int l=left(idx);
    int r=right(idx);
    if(l>=size){
        return;
    }
    if(r>=size){
        if(v[l]->score>v[idx]->score){
            swap(&v[l],&v[idx]);
        }
        return;
    }
    idx=swapmaxchild(v,idx);
    if(idx) heapifydn(v,idx,size);
    return;
}

//HEAPUTIL ENDS

//HEAPSORT
void QNA_tool::Sort(vector<para*> &corpus,int k){
    for (int i = 1; i < cor_size; ++i)
    {
        heapifyup(corpus,i);
    }
    int size=cor_size;
    for (int i = 0; i < k; ++i)
    {
        swap(&corpus[0],&corpus[size-1]);
        heapifydn(corpus,0,size-1);
        size--;
    }

}

//not getting used in current implementation
bool QNA_tool::is_separator(char x){
    for(auto i:separators){
        if(i==x) return true;
    }
    return false;
}


Node* QNA_tool::get_top_k_para(string question, int k) {
    Node* head=nullptr;
    //create dict to get count of occurances 
    Dict q;
    q.insert_sentence(1,0,0,0,question);
    //add the freq of word in csv to the metadata of word
    for (int i = 0; i < q.distinct_words.size(); ++i)
    {
        q.distinct_words[i].freq = csv.search(q.distinct_words[i].word);
    }
    for(int j = 0; j<corpus.size(); j++){
        corpus[j]->score=0;
    }
    /*w:: w.word - gives the word in lower case
    ,w.freq - frequency in csv file
    ,w.node - w.node->word_count gives the no. of times it is in "question"
    */
    for(auto w : q.distinct_words){
        string wrd = w.word;
        long long f_csv = w.freq;
        int occurances = w.node->word_count;

        //j is a para object
        //j->d gives us the dict for that paragraph that saem has made
        for(auto j : corpus){
            double countOfWord = 0;
            int h = hash(wrd);
            freq_word* n=j->frequencies[h];
            while(n){
                if(n->word == wrd){
                    countOfWord=n->freq;
                    break;
                }
                n=n->right;
            }
            // double countOfWord = j->t->search(wrd);
            long long score = mkg./*search*/get_word_count(wrd);
            // double score = sc;
            // double f_csv=fc;
            j->score += ((occurances)*(score+1) * (countOfWord))/(f_csv +1);
            // j->score += ((occurances)*(countOfWord+1))/(sore +1);
        }
    } 
    Sort(corpus,k);
    // if(k==4){
    //     for (auto j : corpus)
    //     {
    //         if(j->b_code==79 && j->page_no==457 && j->para_no==2){
    //             cout<<j->b_code<<' '<<j->score<<endl;
    //         }
    //         if(j->b_code==45 && j->page_no==386 && j->para_no==4){
    //             cout<<j->b_code<<' '<<j->score<<endl;

    //         }
    //     }
    // }
    head=new Node(corpus[cor_size-1]->b_code,corpus[cor_size-1]->page_no,corpus[cor_size-1]->para_no,0,0);
    head->left=nullptr;
    head->right=nullptr;
    for(int i=(cor_size-k);i<(cor_size-1);i++){
        insert(corpus[i]->b_code,corpus[i]->page_no,corpus[i]->para_no,head);
        // cout<<corpus[i]->score<<endl;
        
    }
    // cout<<corpus[cor_size-1]->score<<endl;
    return head;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "Corpora/MK Gandhi/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }
    inputFile.close();
    return res;
}


void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    Dict q;
    string imp_words;
    int count=0;
    q.insert_sentence(1,0,0,0,question);
    for(auto i : q.distinct_words){
        if(i.word == "mahatma" || i.word.substr(0,6) == "gandhi" || i.word == "india" || csv.search(i.word)>ignorable){
            
        }
        else if(i.word=="president" || i.word=="financial" || i.word.substr(0,3)=="tax" || i.word=="country" || i.word=="government" || i.word=="environment"){
            imp_words += i.word+' ';
            count++;
        }
        else {imp_words += (i.word+' '); count++;}
    }
    if(count<1){

    }
    //decide k based on the paragraph lengths obtained so far;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}


void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}

// int main(int argc, char const *argv[])
// {
//     vector<unsigned long long> v;
//     ifstream file("mkgandhi_csv.csv");
//     string word;
//     string f;
//     int ct=0;
//     for (int i = 0; i < 309; ++i)
//     {
//         getline(file,word,',');
//         getline(file,f);
//         int h = hassh(word);
//         v.push_back(h);
//         // if(v[h]) {cout<<word<<' '<<h<<' '<<v[h]<<endl; ct++;}
//         // v[h]++;
//     }
//     sort(v.begin(), v.end());
//     for(auto i:v){
//         cout<<i<<' ';
//     }
//     return 0;
// }