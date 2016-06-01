#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
using namespace std;

class Card
{
    char number;
    char suit;
    public:
        Card(char n, char s) : number(n), suit(s) {}
        Card(){}
        char getnumber()    {   return number;  }
        char getsuit()      {   return suit;    }
};

vector<stack<Card> > v(52);
vector<stack<Card> >::iterator it;

void start(string& s1, string& s2)
{
    int i=0, j=0;
    Card cards[52];
    char value, suit;
    for(; j<52; i+=2, j++)
    {
        i = (j != 26 ? i : 0);
        value = ( j<26 ? s1[i++] : s2[i++]);
        suit    = (  j<26 ?  s1[i] : s2[i]);
        cards[j] = { value, suit};
        v.at(j).push(cards[j]);
    }
}

void accordian_patience()
{
        if(it == v.end())
                return;
        if( it > (v.begin()+2) && (it->top().getnumber() == (it-3)->top().getnumber() || it->top().getsuit() == (it-3)->top().getsuit()))
        {
                (it-3)->push(it->top());
                it->pop();
                if(it->empty() == true)
                        v.erase(it);
                it = v.begin()+1;
                return accordian_patience();
        }

        if(it->top().getnumber() == (it-1)->top().getnumber() || it->top().getsuit() == (it-1)->top().getsuit())
        {
                (it-1)->push(it->top());
                it->pop();
                if(it->empty() == true)
                        v.erase(it);
                it = v.begin()+1;
                return accordian_patience();
        }
        ++it;
        return accordian_patience();
}

int main()
{
        string pt1, pt2;
        while(getline(cin,pt1) && pt1[0] != '#')
        {
                getline(cin, pt2);
                start(pt1, pt2);
                it = v.begin() + 1;
                accordian_patience();

                cout << v.size() << (v.size() > 1 ?  " piles" : " pile") << " remaining:  ";
                 for(it = v.begin(); it != v.end(); ++it)
                         cout << it->size() << ((it+1) != v.end() ? " " : "\n") ;

                pt1.clear();
                pt2.clear();
                v.clear();
                v.resize(52);
        }
        return 0;
}
