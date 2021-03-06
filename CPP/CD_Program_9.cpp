/*
   Lab Name - Compiler Design
   Objective - Static Operator Precedence Parser
   Name - Anup Agrawal
   Roll No. - UE143014
   Date - 19/04/2017
*/

#include<bits/stdc++.h>
using namespace std;

stack<int> resultstk;
stack<char> stk;
stack<char> out;
queue<char> que;
char qtop;
char stop;
int operation = 0;
string infix = "";
string postfix;
string instr;
stack<string> poststk;
void display(string);
void reduce();
void shift();
char ch ='Y';
set<char> ter;
vector<char> terminal;
vector<string> rightp;
vector<string> v;
vector<char> leftp;
map<char, vector<string> > prod;
map<char , vector<char> > first ;
map<char , vector<char> > last ;
map<char,int> row;
map<char,int> index;
map<char,int> column;
string tab[20][20];

void displayStack(stack<char> a)
{
    stack <char> b;
    string s,temp;
    while(!a.empty())
   {
       temp=a.top();
       s.insert(0,temp);
       b.push(a.top());
       a.pop();
   }
   cout<<s;
   while(!b.empty())
   {
       a.push(b.top());
       b.pop();
   }
}

void displayQueue(queue<char> a)
{
    queue <char> b;
   string s,temp;

   while(!a.empty())
   {
       temp=a.front();
       s+=temp;
       b.push(a.front());
       a.pop();
   }
   cout<<s;
   while(!b.empty())
   {
       a.push(b.front());
       b.pop();
   }
}

bool Isoperator(char ch)
{
        if(ch == '+')
        {
            operation = 1;
            return true;
        }
        else if(ch == '-')
        {
            operation = 2;
            return true;
        }
        else if(ch == '*')
        {
            operation = 3;
            return true;
        }
        else if(ch == '/')
        {
            operation = 4;
            return true;
        }
    else{
        return false;
    }
}

bool Isoperand(char ch)
{
    if(ch =='1' || ch =='2' ||  ch =='3' ||  ch =='4')
    {
        return true;
    }
    else{
        return false;
    }
}

void firstFinder(char ch) {
	vector<string> production = prod[ch] ;
	set<char> ans ;
	for(int i=0 ; i<production.size() ; i++) {
		int j = 0 ;
		string p_str = production[i] ;
		while(j < p_str.length()) {
			int asc = (int)p_str[j] ;
			if(asc < 65 || asc > 90) {
				ans.insert(p_str[j]) ;
				ter.insert(p_str[j]);
				break ;
			} else {
				if(p_str[j] != ch) {
					firstFinder(p_str[j]) ;
					vector<char> temp = first[p_str[j]] ;
					for(int k=0 ; k<temp.size() ; k ++){
						ans.insert(temp[k]);
						ter.insert(temp[k]);
					}
				}
			}
			j ++ ;
		}
	}
	vector<char> temp ;
	for(set<char> :: iterator it = ans . begin() ; it != ans . end() ; it ++)
    {
		temp.push_back(*it) ;
    }
	first[ch] = temp ;
}

void allclear()
{
    v.clear();
    while(!stk.empty())
    {
        stk.pop();
    }
    while(!que.empty())
    {
        que.pop();
    }
    while(!out.empty())
    {
        out.pop();
    }
    while(!resultstk.empty())
    {
        out.pop();
    }
    while(!poststk.empty())
    {
        out.pop();
    }
    infix = "";
    postfix = "";
    operation =0;
}

void lastFinder(char ch) {
	vector<string> production = prod[ch] ;
	set<char> ans ;
	for(int i = 0 ; i < production.size() ; i ++) {
		string p_str = production[i] ;
		int j = p_str.length()-1 ;
		while(j >= 0) {
			int asc = (int)p_str[j] ;
			if(asc < 65 || asc > 90) {
				ans.insert(p_str[j]) ;
				ter.insert(p_str[j]) ;
				break ;
			} else {
				if(p_str[j] != ch) {
					lastFinder(p_str[j]) ;
					vector<char> temp = last[p_str[j]] ;
					for(int k = 0 ; k < temp . size() ; k ++){
						ans.insert(temp[k]) ;
						ter.insert(temp[k]) ;
				}
			}
			j -- ;
		}
	}
	vector<char> temp ;
	for(set<char> :: iterator it = ans . begin() ; it != ans . end() ; it ++)
    {
        temp.push_back(*it) ;
    }
        last[ch] = temp ;
}
}

void shift()
{
    if(que.front() != '$')
    {
        qtop = que.front();
        stk.push(qtop);
        que.pop();
        display("shift");
    }
    else if(stk.top() == '$' && que.front() == '$')
{
    cout<<"String is Successfully Parsed"<<endl;
}

    else{
     cout<<"String is NOT Parsed"<<endl;
    }
}

void reduce()
{
    if(stk.top() != '$')
{
    out.push(stk.top());
    stk.pop();
    display("Reduce");
}
else if(stk.top() == 'S')
{
    cout<<"String is Successfully Parsed"<<endl;
}

else{
     cout<<"String is NOT Parsed"<<endl;
}
}

void table()
{
    cout<<"*******Parse table is ********************"<<endl;
    for(int i=0;i<terminal.size();i++)
    {                                               // Give row indexes
        row[terminal[i]]=i+1;
    }
    for(int i=0;i<terminal.size();i++)
    {
        column[terminal[i]]=i+1;                     // Give column indexes
    }
    for(int i=1;i<=terminal.size();i++)
    {
        for(int j=1;j<=terminal.size();j++)           // All entries as -
        {
            tab[i][j] = "-";
        }
    }
    tab[0][0] = "#";
    string ha= "";

    for(int i=1;i<=terminal.size();i++)
    {                                              // Enter Terminal in Row
        ha = ha + terminal[i-1];
        tab[i][0] = ha;
        ha = "";
    }
    for(int i=1;i<=terminal.size();i++)
    {
        ha = ha + terminal[i-1];
        tab[0][i] = terminal[i-1];                // Enter Terminal in column
        ha = "";
    }

    for(int i=0;i<leftp.size();i++)
    {
        char tempc = leftp[i];
        string temps = rightp[i];
        int length = temps.length();
        for(int j=0;j<length - 1;j++)
        {
            int asc1 = (int)temps[j] ;
            int asc2 = (int)temps[j + 1] ;
            int asc3 = (int)temps[j + 2] ;
            if((asc1 < 65 || asc1 > 90) && (asc2 < 65 || asc2 > 90))
            {                                                            //Both terminal,   E->ab Then a = a
            int i1 = row[temps[j]] ;
            int i2 = column[temps[j + 1]] ;
            tab[i1][i2] = "=" ;
            }
            if((j < length - 2) && (asc1 < 65 || asc1 > 90) && (asc2 >= 65 && asc2 <= 90) && (asc3 < 65 || asc3 > 90)) {
            int i1 = row[temps[j]] ;                           // CASE 1 :->  E->aAb , a = b
            int i2 = column[temps[j + 2]] ;
            tab[i1][i2] = "=" ;
            }
            if((asc1 < 65 || asc1 > 90) && (asc2 >= 65 && asc2 <= 90))
            {                                                  // CASE 2:->   E->aB
                vector<char> firsts = first[temps[j + 1]] ;    // a<b   ,   First(B) > a
                for(int k = 0 ; k < firsts . size() ; k ++)
                    {
                    int i1 = row[temps[j]] ;
                    int i2 = column[firsts[k]] ;
                    tab[i1][i2] = "<" ;
                    }
            }
            if((asc1 >= 65 && asc1 <= 90) && (asc2 < 65 || asc2 > 90))
            {                                                  // CASE 3:-> E->Ab
                vector<char> lasts = last[temps[j]] ;          // a>b   , Last(A) > b
                for(int k = 0 ; k < lasts . size() ; k ++)
                 {
                    int i1 = row[lasts[k]] ;
                    int i2 = column[temps[j + 1]] ;
                    tab[i1][i2] = ">" ;
                }
            }
      }
      }
      vector<char> firsts = first[v[0][0]] ;
      for(int j = 0 ; j < firsts . size() ; j ++)
        {
            int i1 = row['$'] ;                                          // Fill Column Wise , Row fixed
            int i2 = column[firsts[j]] ;                                 // Row $ < all first of Start symbol in column
            tab[i1][i2] = "<" ;
        }
      vector<char> lasts = last[v[0][0]] ;
      for(int j = 0 ; j < lasts . size() ; j ++)
      {
        int i1 = row[lasts[j]] ;                                         // Fill Row wise , column fixed
        int i2 = column['$'] ;                                           // last of start symbol > column $
        tab[i1][i2] = ">" ;
      }
      cout<<endl<<endl;
      for(int i=0;i<=terminal.size();i++)
      {
          for(int j=0;j<=terminal.size();j++)
          {
              cout<<tab[i][j]<<"\t";
          }
          cout<<endl;
      }
}


vector<string> divideProduction(string str)
{
    vector<string> parts ;
    string temp = "" ;
    for(int i = 2;i<str.length();i ++) {
      if(str[i] == '|') {
          parts.push_back(temp) ;
          temp = "" ;
      } else {
           temp = temp + str[i] ;
      }
  }
    if(temp != "") {
     parts.push_back(temp) ; // For Last production E-E+T|T , Here for T
  }
    return parts ;
}

void display(string action)
{
            displayStack(stk);
            cout<<"\t\t\t";
            displayQueue(que);
            cout<<"\t\t\t";
            cout<<action;
            cout<<"\t\t\t";
            displayStack(out);
            cout<<endl;
}

void input()
{
    string str;
    int n;
    cout<<"*************** Enter Number of Variables ******************************"<<endl;
    cin>>n;
    cout<<"*************** Enter Productions **************************************"<<endl;
    for(int i=0;i<n;i++)
    {
        cin>>str;
        v.push_back(str);
    }
    cout<<"Enter a input string"<<endl;
    cin>>instr;
}

int findindicesrow(string d)
{
    int c=0;
    for(int i=0;i<=terminal.size();i++)
    {
        if(tab[0][i] == d)
        {
            c = i;
        }
    }
    return c;
}

int findindicescolumn(string d)
{
    int c=0;
    for(int i=0;i<=terminal.size();i++)
    {
        if(tab[i][0] == d)
        {
            c = i;
        }
    }
    return c;
}

void PostToIn()
{
    string temp;
    while(!out.empty())
    {
        temp = out.top();
        postfix.insert(0,temp);
        out.pop();
    }
    char ch;
    int temp1;
    int first;
    int second;
    int result;
    string op1;
    string op2;
    for(int i=0;i<postfix.length();i++)
    {
        ch = postfix[i];
        if(Isoperand(ch))                        // push into stack
        {
            temp = ch;
            stringstream convertch(temp);
            convertch>>temp1;
            poststk.push(temp);
            resultstk.push(temp1);
        }
        else if(Isoperator(ch))
        {
            op1 = poststk.top();
            first = resultstk.top();       // pop two elements from stack and use operator between them, op2 op op1
            poststk.pop();
            resultstk.pop();
            op2 = poststk.top();
            second = resultstk.top();
            poststk.pop();
            resultstk.pop();
            infix = op2 + ch + op1;
            poststk.push(infix);
            switch(operation)
            {
            case 1:
                resultstk.push(second + first);
                cout<<"\t\t\t\t\t\t\t\t\t"<<infix<<","<<second << "+" <<first <<"---->"<<resultstk.top()<<endl;
                break;
            case 2:
                resultstk.push(second - first);
                cout<<"\t\t\t\t\t\t\t\t\t"<<infix<<","<<second << "-" <<first <<"---->"<<resultstk.top()<<endl;
                break;
            case 3:
                resultstk.push(second * first);
                cout<<"\t\t\t\t\t\t\t\t\t"<<infix<<","<<second << "*" <<first <<"---->"<<resultstk.top()<<endl;
                break;
            case 4:
                resultstk.push(second / first);
                cout<<"\t\t\t\t\t\t\t\t\t"<<infix<<","<<second << "/" <<first <<"---->"<<resultstk.top()<<endl;
                break;
            }
        }
    }
    for(int i=0;i<infix.length();i++)
    {
        out.push(infix[i]);
    }
    display("None");
    cout<<"\t\t\t\t\t\t\t\t\t"<<resultstk.top()<<" --Answer"<<endl;
}

void traverse()
{
    string tempqtop;
    string tempstop;
    int row;
    int column;
    for(int i=0;i< 2* instr.length();i++)
    {
        qtop = que.front();
        if(qtop == '1' || qtop == '2' || qtop == '3' || qtop == '4')
        {
            qtop = 'i';
        }
        tempqtop = qtop;
        stop = stk.top();
        if(stop == '1' || stop == '2' || stop == '3' || stop == '4')
        {
            stop = 'i';
        }
        tempstop = stop;
        column = findindicesrow(tempqtop);
        row = findindicescolumn(tempstop);
        if(tab[row][column] == "<")
        {
            shift();
        }
        else if(tab[row][column] == ">")
        {
            reduce();
        }
        else{
            cout<<"************** ERROR *************"<<endl;
        }
    }
}

void intialize()
{
    cout<<"******************* BUP *******************"<<endl;
    input();
    int i=0;
    while(instr[i]!='\0')
        {
            que.push(instr[i]);
            i++;
        }
        que.push('$');
        stk.push('$');
}

int main()
{
    while(ch=='Y' || ch=='y')
    {
    intialize();
    for(int i=0;i<v.size();i++)
    {
        vector<string> temp = divideProduction(v[i]);
        for(int j=0 ;j<temp.size();j++) {
        rightp.push_back(temp[j]) ;
        leftp.push_back(v[i][0]) ;
        }
        prod[v[i][0]] = temp ;
    }
    for(int i = v.size() - 1 ; i >= 0 ; i --) {
  	if(first.find(v[i][0]) == first.end())
    {
  		firstFinder(v[i][0]) ;
    }
    }
    cout<<"Firsts Are :- "<<endl;

    for(map<char , vector<char> > :: iterator it = first.begin() ; it != first.end() ; it ++)
    {
        vector<char> temp = first[it -> first] ;
        cout << "First(" << it -> first << ") :-" ;
        for(int i = 0 ; i<temp.size() ; i++) {
  		cout << temp[i] ;
  		if(i != temp . size() - 1){
  			cout << " , " ;
  		}
  	}
  	cout <<endl;
    }
    for(int i = v.size() - 1 ; i >= 0 ; i --) {
  	if(last.find(v[i][0]) == last.end())
  		lastFinder(v[i][0]) ;
    }
    cout<<"Lasts Are :- "<<endl;

    for(map<char , vector<char> > :: iterator it = last.begin() ; it != last.end() ; it ++)
    {
        vector<char> temp = last[it -> first] ;
        cout << "Last(" << it -> first << ") :-" ;
        for(int i = 0 ; i<temp.size() ; i++) {
  		cout << temp[i] ;
  		if(i != temp . size() - 1){
  			cout << " , " ;
  		}
  	}
  	cout <<endl;
    }

    for(set<char> :: iterator it = ter . begin() ; it != ter . end() ; it ++)
    {
        terminal.push_back(*it) ;
    }
    terminal.push_back('$');
    cout<<"Terminal are  -->  ";
    for(int i=0;i<terminal.size();i++)
    {
        cout<<terminal[i]<<",";
    }
    cout<<endl;
    table();
    cout<<"********* Traversing is ... ***********"<<endl;
    cout<<"Stack"<<"\t\t\t"<<"Queue"<<"\t\t\t"<<"Action\t\t\t"<<"Output"<<endl<<endl;
    display("None");
    traverse();
    PostToIn();
    cout<<"Wnat to parse more strings, please enter Y/y"<<endl;
    cin>>ch;
    if(ch=='y' || ch=='Y')
    {
    system("cls");
    allclear();
    }
    else{
        exit(0);
    }
    }
return 0;
}
