#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

struct State
{
    int len;
    int link;
    int pos;
    map<char,int> transitions;
    State(int len = 0, int link = -1,int pos = 0, const map<char,int>& tr = map<char,int>()):len(len),pos(pos),link(link),transitions(tr)
    {
    }
};

bool isSquare(const string& w)
{
    int s = w.size();
    for(int i = 0; i < s/2; i++)
    {
        if(w[i] != w[s/2 + i])
        {
            return false;
        }
    }

    return true;
}

int cntSquaredRepresentatives(const vector<State>& st, const string& w, int n)
{
    int cnt = 1; ///for epsilon
    for(int i = 1; i < n; i++)
    {
        if(st[i].len % 2 == 1)
        {
            continue;
        }
        string temp;
        int j = st[i].pos - st[i].len + 1;
        int l = 0;
        while(l < st[i].len)
        {
            temp.push_back(w[j + l]);
            l++;
        }
        if(isSquare(temp))
        {
            cnt++;
        }
    }

    return cnt;
}

int main(int argc, char* argv[])
{

    vector<State> st(1);
    vector<bool> isFinal;
    int deltaSize = 0;
    int numFinalStates = 0;
    int n = 1;
    int last = 0;
    st[0] = State();
    string fileName = argv[1];
    ifstream iFile;
    iFile.open(fileName);
    if(!iFile)
    {
        cerr << "File was not opened successfully \n";
        return 1;
    }
    string w;
    iFile >> w;
    iFile.close();
    st.resize(2 * w.size() + 1);
    for(int i = 0; i < w.size(); i++)
    {
        char c = w[i];
        int curr = n++;
        st[curr].len = st[last].len + 1;
        st[curr].pos = i;
        int p = last;
        while(p > -1 && !st[p].transitions.count(c))
        {
            st[p].transitions[c] = curr;
            deltaSize++;
            p = st[p].link;
        }
        if(p == -1)
        {
            st[curr].link = 0;
        }
        else
        {
            int q = st[p].transitions[c];
            if(st[p].len + 1 == st[q].len)
            {
                st[curr].link = q;
            }
            else
            {
                int clone = n++;
                st[clone].len = st[p].len + 1;
                st[clone].pos = i;
                st[clone].transitions = st[q].transitions;
                deltaSize += st[q].transitions.size();
                st[clone].link = st[q].link;
                while(p != -1 && st[p].transitions[c] == q)
                {
                    st[p].transitions[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[curr].link = clone;
            }
        }
        last = curr;
    }
    isFinal.resize(n);
    while(last > -1)
    {
        numFinalStates++;
        isFinal[last] = 1;
        last = st[last].link;
    }
    int cntSq = cntSquaredRepresentatives(st,w, n);

    cout << "NumStates = " << n << endl;
    cout << "DeltaSize = " << deltaSize << endl;
    cout << "numFinalStates = " << numFinalStates << endl;
    cout << "numSquaredWords = " << cntSq << endl;

    return 0;
}
