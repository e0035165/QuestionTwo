//
//  main.cpp
//  QuestionTwo
//
//  Created by Sart Way on 17/1/24.
//

#include <iostream>
#include <iostream>
#include <string>
#include<algorithm>
#include<functional>
#include<fstream>
#include<vector>
#include <set>
#include <queue>
#include<map>
#include <chrono>
#include <thread>


using namespace std;
string ltrim(const string &str);
vector<string> split(const string& str);
string rtrim(const string &str);


struct Node{
    char S;
    int children_index[26];
    vector<Node*>children;
    Node* failure=NULL;
    Node* parent=NULL;
    bool endofword = false;
    int id = -1;
    int number;
    Node(){}
    
    Node(char S)
    {
        this->S = S;
        for(int i=0;i<26;++i)
        {
            children_index[i] = -1;
        }
    }
    
    void addChild(char S, bool endofword) {
        int index = (int)children.size();
        Node* no = new Node(S);
        no->endofword=endofword;
        children.push_back(new Node(S));
        children_index[S-'a']=index;
    }
    
    int getIndexValue(char S)
    {
        return this->children_index[S-'a'];
    }
    
    Node* getCharNode(char S)
    {
        if(this->getIndexValue(S)!=-1)
        {
            return this->children[this->getIndexValue(S)];
        } else {
            return NULL;
        }
    }
    
    void setNumber(int number)
    {
        this->endofword=true;
        this->number=number;
    }
    
    ~Node()
    {}
    
};
int getForwardNumber(Node* startnode, string value);


int getForwardNumber(Node* startnode, string value)
{
    
    Node* temp = startnode;
    for(int i=0;i<value.length();++i)
    {
        //cout << "Current value: " << value[i] << endl;
        if(int(value[i])>=49 && int(value[i])<=57)
        {
            return int(value[i]) - int('0');
        } else {
            if(temp->getCharNode(value[i])!=NULL)
            {
                temp = temp->getCharNode(value[i]);
                //cout << "If internal checking: " << temp->S << endl;
            } else {
                Node* temp_failure = temp->failure;
                while(temp_failure->getCharNode(value[i])==NULL && temp_failure!=startnode)
                {
                    temp_failure = temp_failure->failure;
                }
                if(temp_failure==startnode)
                {
                    //cout << "Inside startnode: " << temp_failure->S << endl;
                    if(temp_failure->getCharNode(value[i])!=NULL)
                    {
                        //cout << "On" << endl;
                        temp_failure = temp_failure->getCharNode(value[i]);
                        //cout << temp_failure->S << endl;
                    }
                    temp = temp_failure;
                } else {
                    temp=temp_failure;
                }
                //cout << "else checking : " << temp->S << endl;
            }
            if(temp->endofword==true)
            {
                //cout << "Tester: " << temp->number << endl;
                return temp->number;
            }
        }
    }
    return -1;
}

Node* createForwardTrie(string* allwords, int n)
{
    Node* startnode = new Node('!');
    startnode->parent = startnode;
    startnode->failure=startnode;
    Node* temp = startnode;
    vector<queue<Node*>>qr(10);
    int idr = 0;
    startnode->id=idr;
    for(int i=0;i<n;++i)
    {
        temp = startnode;
        string tem = allwords[i];
        for(int j=0;j<=tem.length()-1;++j)
        {
            char Y = tem[j];
            if(temp->getIndexValue(Y) == -1)
            {
                idr++;
                if(j==tem.length()-1) {
                    temp->addChild(Y, false);
                } else {
                    temp->addChild(Y, false);
                }
                Node* child = temp->getCharNode(Y);
                child->id = idr;
                child->parent = temp;
                temp = child;
                qr[j].push(child);
            } else {
                temp = temp->getCharNode(Y);
            }
            if(j==tem.length()-1)
            {
                temp->setNumber(i+1);
            }
        }
    }
    
    queue<Node*>q;
    
    for (int i = 0; i < qr.size(); ++i)
    {
        while (qr[i].size())
        {
//            qr[i].front()->id = idr++;
//            cout << i << " letter: " << qr[i].front()->S << " id: " << qr[i].front()->id << endl << " parent letter: " << qr[i].front()->parent->S << " id: " << qr[i].front()->parent->id << endl;
            q.push(qr[i].front());
            qr[i].pop();
        }
    }
    temp=startnode;
    while(!q.empty())
    {
        Node* tester = q.front();
        q.pop();
        Node* parent = tester->parent;
        Node* parent_failure = tester->parent->failure;
//        cout << tester->S << " " << tester->id << endl;
//        cout << parent->S << " " << parent->id << endl;
        
        
//        cout << "Parent " << parent->S << "Parent id: " << parent->id << " Child: " << tester->S << " Child id: " << tester->id << endl;
//        cout << "Parent failure: " << parent_failure->S << endl;
        if(parent_failure == parent)
        {
            tester->failure=parent;
        } else {
            while(parent_failure->getCharNode(tester->S)==NULL && parent_failure!=startnode)
            {
                parent_failure = parent_failure->failure;
            }
            if(parent_failure==startnode)
            {
                if(parent_failure->getCharNode(tester->S)!=NULL)
                {
                    tester->failure = startnode->getCharNode(tester->S);
                } else {
                    tester->failure = startnode;
                }
            } else {
                tester->failure = parent_failure->getCharNode(tester->S);
            }
        }
//        cout << "Child failure: " << tester->failure->S << " failure id: " << tester->failure->id << endl;
//        cout << "Check parent again: " << parent->S << " its id: " << parent->id << endl;
//        if(tester->endofword==true)
//        {
//            cout << "End of word: " << tester->number << " " << tester->S << endl;
//        }
    }
    
    
    return startnode;
}

int main(int argc, const char * argv[]) {
    fstream finout("File.txt", ios::in | ios::out);
    string numbers;
    getline(finout,numbers);
    //cout << numbers;
    int n = stoi(numbers);
    vector<string>values(n);
    for(int i=0;i<n;++i)
    {
        getline(finout,values[i]);
    }
    
//    for(int i=0;i<n;++i)
//    {
//        cout << values[i] << endl;
//    }
    string allwords[] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };
    
    string allwords_reverse[] = {
        "eno",
        "owt",
        "eerht",
        "ruof",
        "evif",
        "xis",
        "neves",
        "thgie",
        "enin"
    };
    
    Node* startnode = createForwardTrie(&allwords[0], 9);
//    cout << endl;
//    cout << endl;
    //cout << "Value to test: " << values[220] << endl;
    cout << endl;
    Node* endnode = createForwardTrie(&allwords_reverse[0], 9);
    long ans = 0l;
    for(int i=0;i<n;++i)
    {
        int num = getForwardNumber(startnode, values[i]);
        reverse(values[i].begin(),values[i].end());
        //cout << endl;
        int revnum = getForwardNumber(endnode, values[i]);
        
        int temp = num*10+revnum;
        reverse(values[i].begin(),values[i].end());
        cout << values[i] << endl;
        cout << num << endl;
        cout << revnum << endl;
        cout << temp << endl;
        ans+=temp;
    }
    cout << "Answer: " << endl;
    cout << ans << endl;
    //std::cout << "Hello, World!\n";
    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
    return s;

    return s;
}



vector<string> split(const string& str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
