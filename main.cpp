#include<iostream>
#include<string>

using namespace std;


typedef struct TrieNode{
    
    int val;
    char c;
    struct TrieNode *children[26];
    bool isEndOfWord;
    
}TrieNode;
    
TrieNode* root;

TrieNode* Last_Node(string key);
bool isEmpty(TrieNode* head);
int max_val_Node(TrieNode* node);

TrieNode* getNode(char ch)
{
    TrieNode* p = new TrieNode;
 
    if(p != NULL){
        p->val = 0;
        p->c = ch;
        p->isEndOfWord = false;
        for(int i = 0; i < 26; i++)
        {
            p->children[i] = NULL;
        }
    }
 
    return p;
}

void insert(string key)
{
    
    TrieNode *trav = root;
 
    for(int i=0; i<key.length(); i++)
    {
        char c_ati = key[i];
        if (trav->children[c_ati - 'a'] == NULL){
            trav->children[c_ati - 'a'] = getNode(c_ati);
        }
        
        trav = trav->children[c_ati - 'a'];
    }
 
    trav->isEndOfWord = true;
}


bool search(struct TrieNode *root, string key)
{
    struct TrieNode *trav = root;
    
    bool out = true,flag=true;
    for (int i = 0; i < key.length(); i++)
    {
        if (!trav->children[key[i] - 'a']){
            flag = false;
            break;
        }
        trav = trav->children[key[i] - 'a'];
    }
    out=(trav->isEndOfWord && flag);
    if(out){
        
        trav = root;
        for(int i=0; i<key.length(); i++)
        {
            trav = trav->children[key[i]-'a'];
            trav->val += 1;
        }
    }
    return out;
}


TrieNode* Last_Node(string key) //If returns the pointer pointing to the last letter of key if present else NULL
{
    TrieNode* curr = root;

    for(int i=0; i<key.length(); i++)
    {
        if(curr->children[key[i] - 'a'] == NULL){
            return NULL;
        }
        curr = curr->children[key[i] - 'a'];
    }
    return curr;
}

bool isEmpty(TrieNode* head) 
{
    for(int i = 0; i < 26; i++)
    {
        if(head->children[i] != NULL){
            return false;
        }
    }
    return true;
}


TrieNode* delete_word(TrieNode* root, string key, int depth)
{

    if (depth == key.size()) {

        if(root->isEndOfWord){
            root->isEndOfWord = false;
        }
 
        if(isEmpty(root)){
            delete(root);
            root = NULL;
        }
 
        return root;
    }

    root->children[key[depth] - 'a'] = delete_word(root->children[key[depth] - 'a'], key, depth + 1);
 
    if (isEmpty(root) && root->isEndOfWord == false) {
        delete(root);
        root = NULL;
    }
 
    return root;
}

void suggestionsRec(TrieNode* root, string currPrefix)
{
   
    if (root->isEndOfWord)
    {
        cout << "Suggestion: "<<currPrefix<<endl;
    }

    if (isEmpty(root)){
        return;
    }
    int i=0;
    while(i < 26)
    {
        if (root->children[i])
        {
            currPrefix.push_back(root->children[i]->c);
            suggestionsRec(root->children[i], currPrefix);
            currPrefix.pop_back();
            
        }
        i++;
    }
}
 

int Auto_Complete(TrieNode* root, const string key)
{
    TrieNode* trav = root;
    int i=0;
    while(i < key.length())
    {
        int index = key[i] - 'a';

        if (!trav->children[index]){
            return 0;
        }
        trav = trav->children[index];
        i++;
    }
 
    if((trav->isEndOfWord == true))
    {
        if(isEmpty(trav)){
            cout << "Suggestion: "<<key << endl;
            return -1;
        }
        
    }
 
    if (!isEmpty(trav))
    {
        string prefix = key;
        suggestionsRec(trav, prefix);
        return 1;
    }
}

void Autocorrect(string key)
{
    TrieNode* curr = root;
    
    int count = 0;
    for(int i=0; i<key.length(); i++)
    {
        if(curr->children[key[i] - 'a'] == NULL){
            break;
        }
        curr = curr->children[key[i] - 'a'];
        count++;
    }
    
    if(count == key.length() && curr->isEndOfWord == true){
        cout<< "Did you mean: " <<key<<" ?\n";
    }
    
    
    if(count == 0){
        curr = root;
        string out = "";
        while(!isEmpty(curr)){
            int index = max_val_Node(curr);
            out += curr->children[index]->c;
            curr = curr->children[index];
        }
        
        cout<<"Did you mean: "<<out<<"?\n";
        return;
    }
    
    if(0 < count <= key.length()){
        int i = count;
        while(i>0){
            curr = root;
            string out = key.substr(0,i);
            for(int j=0; j<i; j++)
            {
                curr = curr->children[key[j] - 'a'];
            }
            while(!isEmpty(curr)){
                int index = max_val_Node(curr);
                out += curr->children[index]->c;
                curr = curr->children[index];
            }
            cout<<"Did you mean: "<<out<<"?\n";
            i--;
        }
    }
}

int max_val_Node(TrieNode* node) //It returns the index of the child of node which has maximum value
{
    int maxi = 0;
    int out = 0;
    for(int i=0; i<26; i++)
    {
        if(node->children[i] != NULL){
            if(node->children[i]->val > maxi){
                //cout<<"\n"<<node->children[i]->val<<" "<<node->children[i]->c;
                maxi = node->children[i]->val;
                out = i;
            }
        }
    }
    return out;
}

int main(void) {
    
	root = getNode('\0'); // Dummy intilization of root
	
    cout <<"Enter 1 to Insert"<<endl;
    cout <<"Enter 2 to Delete"<<endl;
    cout <<"Enter 3 for searching"<<endl;
    cout <<"Enter 4 for Autocorrect"<<endl;
    cout <<"Enter 5 for Auto_Complete"<<endl;
    cout <<"Enter 6 to exit"<<endl<<endl;

    while(true){
        int i;
        cin>>i;
        if(i==1){
            string st;
            cout<<"Enter a word to be inserted: ";
            cin>>st;
            insert(st);
            search(root,st);
            cout<<"Inserted"<<endl;
        }
        if(i==2){
            string st;
            cout<<"Enter a word to be deleted: ";
            cin>>st;
            delete_word(root,st,0);
            cout<<"Deleted"<<endl;
        }
        if(i==3){
            string st;
            cout<<"Enter a word to be searched: ";
            cin>>st;
            if(search(root,st)){
                cout<<"Present"<<endl;
            }
            else{
                cout<<"Not present"<<endl;
            }
        }
        if(i==4){
            string st;
            cout<<"Enter a word for AutoCoreect: ";
            cin>>st;
            Autocorrect(st);
        }
        if(i==5){
            string s;
            cout<<"Enter a prefix for AutoComplete: ";
            cin>>s;
            int comp = Auto_Complete(root, s);
 
            if (comp == -1)
                cout << "No other strings found with this prefix\n";
 
            if (comp == 0)
                cout << "No string found with this prefix\n";
        }
        if(i==6){
            cout<<"Exited";
            break;
        }
    }
    //cout << max_val_Node(root->children['t'-'a']);
    return 0;
}
