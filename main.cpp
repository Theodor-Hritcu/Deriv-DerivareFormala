#include <iostream>
#include <stack>
#include <cstring>
#include <string.h>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
using namespace std;

///STRUCTURA ARBORESCENTA
struct arbore
{
    string data;
    arbore* stanga;
    arbore* dreapta;
    arbore(string val) : data(val), stanga(NULL), dreapta(NULL) {}
};

arbore* arbore_expresie;
arbore* arbore_derivat;
arbore* arbore_derivat_2;

string ecuatie_de_derivat;
string notatia_Postfixata;
string notatia_inordine;
string notatia_inordine_2;

int latime_ecran=getmaxwidth();
int inaltime_ecran=getmaxheight();

char expr[150];
char data_nod[1000];

///EGALITATEA SUBARBORILOR
bool egalitate(arbore* a1,arbore* a2)
{
    if (a1 == NULL && a2 == NULL)
        return true;
    if (a1 != NULL && a2 != NULL)
        return ((a1->data == a2->data) && egalitate(a1->stanga, a2->stanga) && egalitate(a1->dreapta, a2->dreapta));
    return false;
}

///PRIORITATEA SEMNELOR
int prioritate(char c)
{
    if (((((c=='s')or c=='c') or c=='t') or c=='l') or c=='g')
        return 4;
    else if (c == '^')
        return 3;
    else if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

///POSTFIXAREA ECUATIEI
string Postfixare(string ecuatie)
{

    stack<char> stiva;
    string rezultat;
    int i=0;
    for ( i = 0; i < ecuatie.length(); i++)
    {
        char c = ecuatie[i];
        if ((c=='x'))
        {
            rezultat += c;
            rezultat+=",";
        }
        else if(c >= '0' && c <= '9')
        {
            while('0'<=ecuatie[i] and ecuatie[i]<='9')
            {
                rezultat+=ecuatie[i];
                i++;
            }
            rezultat+=",";
            i--;
        }
        else if(c=='-' and ecuatie[i+1]<='9' and ecuatie[i+1]>='0' and ecuatie[i-1]!='x' and (ecuatie[i-1]>'9' or ecuatie[i-1]<'0') and ecuatie[i-1]!='e' and ecuatie[i-1] != ')')
        {
            rezultat+="-";
            i++;
            while('0'<=ecuatie[i] and ecuatie[i]<='9')
            {
                rezultat+=ecuatie[i];
                i++;
            }
            rezultat+=",";
            i--;
        }
        else if(c=='.')
        {
            rezultat.pop_back();
            rezultat+=ecuatie[i];
            i++;
            while(ecuatie[i]<='9' and ecuatie[i]>='0')
            {
                rezultat+=ecuatie[i];
                i++;
            }
            rezultat+=",";
            i--;
        }
        else if(c=='e')
        {
            rezultat+=c;
            rezultat+=",";
        }
        else if (c == '(')
            stiva.push('(');
        else if (c == ')')
        {
            while (stiva.top() != '(')
            {
                if(stiva.top()=='s')
                {
                    rezultat+="sin";
                    rezultat+=",";
                }
                else if(stiva.top()=='c')
                {
                    rezultat+="cos";
                    rezultat+=",";
                }
                else if(stiva.top()=='t')
                {
                    rezultat+="tg";
                    rezultat+=",";
                }
                else if(stiva.top()=='g')
                {
                    rezultat+="ctg";
                    rezultat+=",";
                }
                else if(stiva.top()=='l')
                {
                    rezultat+="ln";
                    rezultat+=",";
                }
                else
                {
                    rezultat += stiva.top();
                    rezultat+=",";
                }
                stiva.pop();
            }
            stiva.pop();
        }
        else
        {
            while (!stiva.empty() && prioritate(ecuatie[i]) <= prioritate(stiva.top()))
            {
                if(stiva.top()=='s')
                {
                    rezultat+="sin";
                    rezultat+=",";
                }
                else if(stiva.top()=='c')
                {
                    rezultat+="cos";
                    rezultat+=",";
                }
                else if(stiva.top()=='t')
                {
                    rezultat+="tg";
                    rezultat+=",";
                }
                else if(stiva.top()=='g')
                {
                    rezultat+="ctg";
                    rezultat+=",";
                }
                else if(stiva.top()=='l')
                {
                    rezultat+="ln";
                    rezultat+=",";
                }
                else
                {
                    rezultat += stiva.top();
                    rezultat+=",";
                }
                stiva.pop();
            }
            if(c=='c' and ecuatie[i+1]=='t')
                stiva.push('g');///g-cotangenta
            else
                stiva.push(c);
            if(c=='s' or c=='c')
                i+=2;
            else if(c=='t' or c=='l')
                i+=1;
        }
    }
    while (!stiva.empty())
    {
        if(stiva.top()=='s')
        {
            rezultat+="sin";
            rezultat+=",";
        }
        else if(stiva.top()=='c')
        {
            rezultat+="cos";
            rezultat+=",";
        }
        else if(stiva.top()=='t')
        {
            rezultat+="tg";
            rezultat+=",";
        }
        else if(stiva.top()=='g')
        {
            rezultat+="ctg";
            rezultat+=",";
        }
        else if(stiva.top()=='l')
        {
            rezultat+="ln";
            rezultat+=",";
        }
        else
        {
            rezultat += stiva.top();
            rezultat+=",";
        }
        stiva.pop();
    }

    return rezultat;
}

///CONSTRUIREA ARBORELUI
arbore* construire(string s)
{
    stack<arbore*> stiva;
    char sir[s.size()];
    for(int i=0; i<s.size(); i++)
        sir[i]=s[i];
    sir[s.size()]=NULL;
    char *p=strtok(sir,",");
    while(p)
    {
        if(p[0]=='x' or ('0'<=p[0] and p[0]<='9') or(p[0]=='-' and '0'<=p[1] and p[1]<='9') or p[0]=='e')
            stiva.push(new arbore(p));
        else if(p[0]=='+' or p[0]=='-' or p[0]=='*' or p[0]=='/' or p[0]=='^')
        {
            arbore* dr = stiva.top();
            stiva.pop();
            arbore* st = stiva.top();
            stiva.pop();

            arbore* nou = new arbore(p);
            nou->stanga = st;
            nou->dreapta = dr;

            stiva.push(nou);
        }
        else
        {
            arbore* dr = stiva.top();
            stiva.pop();
            arbore* nou = new arbore(p);
            nou->dreapta = dr;
            stiva.push(nou);
        }
        p=strtok(NULL,",");
    }
    return stiva.top();
}

///SIMPLIFICAREA ARBORELUI
arbore* simplifica(arbore *derivata)
{
    if(derivata==NULL)
        return NULL;
    if(derivata->stanga==NULL && derivata->dreapta==NULL)
        return derivata;
    derivata->stanga=simplifica(derivata->stanga);
    derivata->dreapta=simplifica(derivata->dreapta);
    if(derivata->data=="+")
    {
        if(derivata->stanga->data=="0")
            return derivata->dreapta;
        else if(derivata->dreapta->data=="0")
            return derivata->stanga;
        else if(((derivata->stanga->data[0]<='9' and derivata->stanga->data[0]>='0') or (derivata->stanga->data[0]=='-' and derivata ->stanga->data[1]<='9' and derivata->stanga->data[1]>='0') or derivata->stanga->data=="e") and ((derivata->dreapta->data[0]<='9' and derivata->dreapta->data[0]>='0') or (derivata->dreapta->data[0]=='-' and derivata ->dreapta->data[1]<='9' and derivata->dreapta->data[1]>='0') or derivata->dreapta->data=="e" ))
        {
            float s,d,suma;
            if(derivata->stanga->data=="e")
                s=2.718281;
            else
                s=stod(derivata->stanga->data);
            if(derivata->dreapta->data=="e")
                d=2.718281;
            else
                d=stod(derivata->dreapta->data);
            suma=s+d;
            string str=to_string(suma);
            if(suma-(int)suma==0)
            {
                int n=int(suma);
                str=to_string(n);
            }
            return new arbore(str);
        }
    }
    else if(derivata->data=="*")
    {
        if(derivata->stanga->data=="0" || derivata->dreapta->data=="0")
        {
            return new arbore("0");
        }
        else if(derivata->stanga->data=="1")
            return derivata->dreapta;
        else if(derivata->dreapta->data=="1")
            return derivata->stanga;
        else if(((derivata->stanga->data[0]<='9' and derivata->stanga->data[0]>='0') or (derivata->stanga->data[0]=='-' and derivata ->stanga->data[1]<='9' and derivata->stanga->data[1]>='0') or derivata->stanga->data=="e") and ((derivata->dreapta->data[0]<='9' and derivata->dreapta->data[0]>='0') or (derivata->dreapta->data[0]=='-' and derivata ->dreapta->data[1]<='9' and derivata->dreapta->data[1]>='0') or derivata->dreapta->data=="e" ))
        {
            float s,d,inm;
            if(derivata->stanga->data=="e")
                s=2.718281;
            else
                s=stod(derivata->stanga->data);
            if(derivata->dreapta->data=="e")
                d=2.718281;
            else
                d=stod(derivata->dreapta->data);
            inm=s*d;
            string str=to_string(inm);
            if(inm-(int)inm==0)
            {
                int n=int(inm);
                str=to_string(n);
            }
            return new arbore(str);
        }
        else if(derivata->stanga->data=="^" and derivata->dreapta->data=="^")
        {
            if(egalitate(derivata->stanga->stanga,derivata->dreapta->stanga)==true )
            {
                if(((derivata->stanga->dreapta->data[0]<='9' and derivata->stanga->dreapta->data[0]>='0') or (derivata->stanga->dreapta->data[0]=='-' and derivata ->stanga->dreapta->data[1]<='9' and derivata->stanga->dreapta->data[1]>='0') or derivata->stanga->dreapta->data=="e") and ((derivata->dreapta->dreapta->data[0]<='9' and derivata->dreapta->dreapta->data[0]>='0') or (derivata->dreapta->dreapta->data[0]=='-' and derivata ->dreapta->dreapta->data[1]<='9' and derivata->dreapta->dreapta->data[1]>='0') or derivata->dreapta->dreapta->data=="e" ))
                {
                    arbore *f=derivata->stanga->stanga;
                    float p1,p2;
                    if(derivata->stanga->dreapta->data=="e")
                        p1=2.718281;
                    else
                        p1=stod(derivata->stanga->dreapta->data);
                    if(derivata->dreapta->dreapta->data=="e")
                        p2=2.718281;
                    else
                        p2=stod(derivata->dreapta->dreapta->data);
                    float putere=p1+p2;
                    string str=to_string(putere);
                    if(putere-int(putere)==0)
                    {
                        int n=int(putere);
                        str=to_string(n);
                    }
                    derivata=new arbore("^");
                    derivata->stanga=f;
                    derivata->dreapta=new arbore(str);
                    return derivata;
                }
            }
        }
        else if(derivata->dreapta->data=="*")
        {
            arbore *g=derivata->dreapta->stanga;
            arbore *h=derivata->dreapta->dreapta;
            arbore *f=derivata->stanga;
            derivata=new arbore("*");
            derivata->dreapta=h;
            derivata->stanga=new arbore("*");
            derivata->stanga->dreapta=g;
            derivata->stanga->stanga=f;
            return derivata;
        }
        else if(derivata->stanga->data=="^" and ((derivata->stanga->dreapta->data[0]<='9' and derivata->stanga->dreapta->data[0]>='0') or (derivata->stanga->dreapta->data[0]=='-' and derivata->stanga->dreapta->data[1]<='9' and derivata->stanga->dreapta->data[1]>='0') or derivata->stanga->dreapta->data=="e"))
        {
            if(egalitate(derivata->stanga->stanga,derivata->dreapta)==true)
            {
                float i;
                if(derivata->stanga->dreapta->data=="e")
                    i=2.718281;
                else
                    i=stod(derivata->stanga->dreapta->data);
                i++;
                string str=to_string(i);
                if(i-(int)i==0)
                {
                    int n=int(i);
                    str=to_string(n);
                }
                arbore *f=derivata->stanga->stanga;
                derivata=new arbore("^");
                derivata->stanga=f;
                derivata->dreapta=new arbore(str);
                return derivata;
            }
        }
        else if(egalitate(derivata->stanga,derivata->dreapta)==true)
        {
            arbore* f=derivata->stanga;
            derivata=new arbore("^");
            derivata->stanga=f;
            derivata->dreapta=new arbore("2");
            simplifica(derivata);
            return derivata;
        }
    }
    else if(derivata->data=="-")
    {
        if(derivata->dreapta->data=="0")
            return derivata->stanga;
        else if(derivata->stanga->data=="0")
        {
            derivata->stanga=new arbore("");
            return derivata;
        }
        else if(((derivata->stanga->data[0]<='9' and derivata->stanga->data[0]>='0') or (derivata->stanga->data[0]=='-' and derivata ->stanga->data[1]<='9' and derivata->stanga->data[1]>='0') or derivata->stanga->data=="e") and ((derivata->dreapta->data[0]<='9' and derivata->dreapta->data[0]>='0') or (derivata->dreapta->data[0]=='-' and derivata ->dreapta->data[1]<='9' and derivata->dreapta->data[1]>='0') or derivata->dreapta->data=="e" ))
        {
            float s,d,dif;
            if(derivata->stanga->data=="e")
                s=2.718281;
            else
                s=stod(derivata->stanga->data);
            if(derivata->dreapta->data=="e")
                d=2.718281;
            else
                d=stod(derivata->dreapta->data);
            dif=s-d;
            string str=to_string(dif);
            if(dif-(int)dif==0)
            {
                int n=int(dif);
                str=to_string(n);
            }
            return new arbore(str);
        }
        else if(egalitate(derivata->stanga,derivata->dreapta)==true)
            return new arbore("0");
    }
    else if(derivata->data=="^")
    {
        if(derivata->stanga->data=="1" or derivata->stanga->data=="0")
            return derivata->stanga;
        else if(derivata->dreapta->data=="1")
            return derivata->stanga;
        else if(derivata->dreapta->data=="0")
            return new arbore("1");
        else if(((derivata->stanga->data[0]<='9' and derivata->stanga->data[0]>='0') or (derivata->stanga->data[0]=='-' and derivata ->stanga->data[1]<='9' and derivata->stanga->data[1]>='0') or derivata->stanga->data=="e") and ((derivata->dreapta->data[0]<='9' and derivata->dreapta->data[0]>='0') or (derivata->dreapta->data[0]=='-' and derivata ->dreapta->data[1]<='9' and derivata->dreapta->data[1]>='0') or derivata->dreapta->data=="e" ))
        {
            float s,d,putere;
            if(derivata->stanga->data=="e")
                s=2.718281;
            else
                s=stod(derivata->stanga->data);
            if(derivata->dreapta->data=="e")
                d=2.718281;
            else
                d=stod(derivata->dreapta->data);
            putere=pow(s,d);
            string str=to_string(putere);
            if(putere-(int)putere==0)
            {
                int n=int(putere);
                str=to_string(n);
            }
            return new arbore(str);
        }
        else if(derivata->stanga->data=="^")
        {
            arbore *f=derivata->stanga->stanga;
            arbore *g=derivata->stanga->dreapta;
            arbore *h=derivata->dreapta;
            derivata=new arbore("^");
            derivata->stanga=f;
            derivata->dreapta=new arbore("*");
            derivata->dreapta->dreapta=h;
            derivata->dreapta->stanga=g;
            simplifica(derivata);
        }
    }
    else if(derivata->data=="/")
    {
        if(derivata->stanga->data=="0")
            return new arbore("0");
        else if(derivata->dreapta->data=="0")
        {
            derivata=new arbore("EROARE NU POTI IMPARTI LA 0!");
        }
        else if(egalitate(derivata->stanga,derivata->dreapta)==true)
            return new arbore("1");
        else if(((derivata->stanga->data[0]<='9' and derivata->stanga->data[0]>='0') or (derivata->stanga->data[0]=='-' and derivata ->stanga->data[1]<='9' and derivata->stanga->data[1]>='0') or derivata->stanga->data=="e") and ((derivata->dreapta->data[0]<='9' and derivata->dreapta->data[0]>='0') or (derivata->dreapta->data[0]=='-' and derivata ->dreapta->data[1]<='9' and derivata->dreapta->data[1]>='0') or derivata->dreapta->data=="e" ))
        {
            float s,d,div;
            if(derivata->stanga->data=="e")
                s=2.718281;
            else
                s=stod(derivata->stanga->data);
            if(derivata->dreapta->data=="e")
                d=2.718281;
            else
                d=stod(derivata->dreapta->data);
            div=s/d;
            string str=to_string(div);
            if(div-(int)div==0)
            {
                int n=int(div);
                str=to_string(n);
            }
            return new arbore(str);
        }
    }
    else if(derivata->data=="ln")
    {
        if(derivata->dreapta->data=="1")
            return new arbore("0");
        else if(derivata->dreapta->data=="e")
            return new arbore("1");
        else if(derivata->dreapta->data=="^")
        {
            arbore* st=derivata->dreapta->stanga;
            arbore* dr=derivata->dreapta->dreapta;
            derivata=new arbore("*");
            derivata->stanga=dr;
            derivata->dreapta=new arbore("ln");
            derivata->dreapta->dreapta=st;
            simplifica(derivata);
        }
    }
    else if(derivata->data=="sin")
    {
        if(derivata->dreapta->data=="0")
            return new arbore("0");
    }
    else if(derivata->data=="cos")
    {
        if(derivata->dreapta->data=="0")
            return new arbore("1");
    }
    return derivata;
}

///DERIVAREA ARBORELUI
arbore* derivare(arbore *expresie)
{
    arbore *derivata=new arbore(" ");
    if(expresie->data=="x")
        derivata->data="1";
    else if((expresie->data[0]>='0' and expresie->data[0]<='9') or(expresie->data[0]=='-' and expresie->data[1]>='0' and expresie->data[1]<='9') or expresie->data[0]=='e' )
        derivata->data="0";
    else if(expresie->data=="+")
    {
        derivata->data="+";
        derivata->stanga=derivare(expresie->stanga);
        derivata->dreapta=derivare(expresie->dreapta);
    }
    else if(expresie->data=="-")
    {
        derivata->data="-";
        derivata->stanga=derivare(expresie->stanga);
        derivata->dreapta=derivare(expresie->dreapta);
    }
    else if(expresie->data=="*")
    {
        derivata->data="+";

        arbore *st=new arbore("*");
        st->stanga=derivare(expresie->stanga);
        st->dreapta=expresie->dreapta;

        arbore *dr=new arbore("*");
        dr->stanga=expresie->stanga;
        dr->dreapta=derivare(expresie->dreapta);

        derivata->stanga=st;
        derivata->dreapta=dr;
    }
    else if(expresie->data=="/")
    {
        derivata->data="/";

        arbore *numarator=new arbore("-");
        arbore *numarator_stanga=new arbore("*");
        arbore *numarator_dreapta=new arbore("*");

        numarator_stanga->stanga=derivare(expresie->stanga);
        numarator_stanga->dreapta=expresie->dreapta;

        numarator_dreapta->stanga=expresie->stanga;
        numarator_dreapta->dreapta=derivare(expresie->dreapta);

        numarator->stanga=numarator_stanga;
        numarator->dreapta=numarator_dreapta;

        arbore *numitor=new arbore("^");
        numitor->stanga=expresie->dreapta;
        numitor->dreapta=new arbore("2");

        derivata->stanga=numarator;
        derivata->dreapta=numitor;
    }
    else if(expresie->data=="^")
    {
        if((expresie->stanga->data[0]<='9' and expresie->stanga->data[0]>='0') or (expresie->stanga->data[0]=='-' and expresie->stanga->data[1]<='9' and expresie->stanga->data[1]>='0') or expresie->stanga->data[0]=='e')
        {
            derivata->data="*";

            derivata->dreapta=derivare(expresie->dreapta);

            arbore *st=new arbore("*");
            st->dreapta=expresie;
            arbore *stst=new arbore("ln");
            stst->dreapta=expresie->stanga;
            st->stanga=stst;

            derivata->stanga=st;
        }
        else if((expresie->dreapta->data[0]<='9' and expresie->dreapta->data[0]>='0') or (expresie->dreapta->data[0]=='-' and expresie->dreapta->data[1]<='9' and expresie->dreapta->data[1]>='0' )or expresie->dreapta->data[0]=='e')
        {
            derivata->data="*";

            derivata->dreapta=derivare(expresie->stanga);

            arbore *st=new arbore("*");
            st->stanga=expresie->dreapta;
            arbore* stdr=new arbore("^");
            stdr->stanga=expresie->stanga;
            float i;
            string str;
            if(expresie->dreapta->data=="e")
                i=2.718281;
            else
                i=stod(expresie->dreapta->data);
            i-=1;
            str=to_string(i);
            if(i-(int)i==0)
            {
                int n=int(i);
                str=to_string(n);
            }
            stdr->dreapta=new arbore(str);
            st->dreapta=stdr;
            derivata->stanga=st;
        }
    }
    else if(expresie->data=="ln")
    {
        derivata->data="/";
        derivata->stanga=derivare(expresie->dreapta);
        derivata->dreapta=expresie->dreapta;
    }
    else if(expresie->data=="sin")
    {
        derivata->data="*";

        derivata->stanga=derivare(expresie->dreapta);

        arbore *dr=new arbore("cos");
        dr->dreapta=expresie->dreapta;

        derivata->dreapta=dr;
    }
    else if(expresie->data=="cos")
    {
        derivata->data="*";

        derivata->stanga=derivare(expresie->dreapta);

        arbore *dr=new arbore("-");
        dr->stanga=new arbore("0");
        arbore *drdr=new arbore("sin");
        drdr->dreapta=expresie->dreapta;
        dr->dreapta=drdr;
        derivata->dreapta=dr;
    }
    else if(expresie->data=="tg")
    {
        derivata->data="/";

        derivata->stanga=derivare(expresie->dreapta);

        arbore *dr=new arbore("^");
        arbore *drdr=new arbore("2");
        dr->dreapta=drdr;
        arbore *drst=new arbore("cos");
        drst->dreapta=expresie->dreapta;
        dr->stanga=drst;

        derivata->dreapta=dr;
    }
    else if(expresie->data=="ctg")
    {
        derivata->data="/";

        arbore *st=new arbore("-");
        st->stanga=new arbore("0");
        st->dreapta=derivare(expresie->dreapta);
        derivata->stanga=st;

        arbore *dr=new arbore("^");
        arbore *drdr=new arbore("2");
        dr->dreapta=drdr;
        arbore *drst=new arbore("sin");
        drst->dreapta=expresie->dreapta;
        dr->stanga=drst;

        derivata->dreapta=dr;
    }
    else if(expresie->data=="EROARE NU POTI IMPARTI LA 0!")
        derivata=new arbore("EROARE NU POTI IMPARTI LA 0!");
    return derivata;
}

///PARCURGEREA INORDINE
string inordine(arbore* expresie)
{
    if (expresie == NULL)
    {
        return "";
    }

    if (expresie->stanga == NULL && expresie->dreapta == NULL)
    {
        return expresie->data;
    }

    string rezultat;

    if (expresie->stanga != NULL)
    {
        bool adauga_paranteze = (expresie->stanga->data == "+" || expresie->stanga->data == "-" || expresie->stanga->data == "*" || expresie->stanga->data == "/" || expresie->stanga->data == "sin" || expresie->stanga->data == "cos" || expresie->stanga->data == "tg" || expresie->stanga->data == "ln" || expresie->stanga->data == "ctg" || expresie->stanga->data=="^" );

        if (adauga_paranteze)
            rezultat += "(" + inordine(expresie->stanga) + ")";
        else
            rezultat += inordine(expresie->stanga);
    }

    rezultat += expresie->data;

    if (expresie->dreapta != NULL)
    {
        bool adauga_paranteze = (expresie->dreapta->data == "+" || expresie->dreapta->data == "-" || expresie->dreapta->data == "*" || expresie->dreapta->data == "/" || expresie->dreapta->data == "sin" || expresie->dreapta->data == "cos" || expresie->dreapta->data == "tg" || expresie->dreapta->data == "ln" || expresie->dreapta->data == "ctg" || expresie->dreapta->data=="^" );

        if (adauga_paranteze)
            rezultat += "(" + inordine(expresie->dreapta) + ")";
        else
            rezultat += inordine(expresie->dreapta);
    }

    return rezultat;
}

void srd(arbore* r)
{
    if (r != NULL)
    {
        cout<<"(";
        srd(r->stanga);
        cout << r->data;
        srd(r->dreapta);
        cout<<")";
    }
}

int inaltime_arbore(arbore* arb)
{
    if(arb == NULL)
        return 0;
    else
    {
        int inaltime_stanga=inaltime_arbore(arb->stanga);
        int inaltime_dreapta=inaltime_arbore(arb->dreapta);
        return 1+max(inaltime_stanga,inaltime_dreapta);
    }
}

int latime_arbore(arbore* arb)
{
    if(arb == NULL)
        return 0;
    else
    {
        int latime_stanga=latime_arbore(arb->stanga);
        int latime_dreapta=latime_arbore(arb->dreapta);
        return 1+latime_stanga+latime_dreapta;
    }
}

void deseneaza_nod(char info_nod[],int x,int y)
{
    setlinestyle(0,0,3);
    ellipse(x,y,0,360,textwidth(info_nod)+5,textheight(info_nod)+5);
    settextstyle(BOLD_FONT,0,3);
    outtextxy(x-textwidth(info_nod)/2,y-textheight(info_nod)/2,info_nod);
}

void deseneaza_arbore(arbore* arb, int nivel, int stanga, int latime, int inaltime)
{
    if(arb != NULL)
    {
        int latime_partiala=latime_arbore(arb->stanga);
        int xc=stanga+latime*latime_partiala+latime/2;
        int yc=nivel*inaltime-inaltime/2;

        if (arb->stanga != NULL)
        {
            int xcs=stanga+latime*latime_arbore(arb->stanga->stanga)+latime/2;
            setlinestyle(0,0,3);
            line(xc, yc+textwidth(data_nod)+15, xcs, yc+inaltime-textheight(data_nod)-7);
        }

        if (arb->dreapta != NULL)
        {
            int xcd=stanga+latime*(latime_partiala+1)+latime*latime_arbore(arb->dreapta->stanga)+latime/2;
            setlinestyle(0,0,3);
            line(xc, yc+textwidth(data_nod)+15, xcd, yc+inaltime-textheight(data_nod)-7);
        }

        deseneaza_arbore(arb->stanga, nivel+1, stanga, latime, inaltime);
        deseneaza_arbore(arb->dreapta, nivel+1, stanga+latime*(latime_partiala+1), latime, inaltime);
        for(int i=0; i<1000; i++)
            data_nod[i]=NULL;
        for(int i=0; i<arb->data.size(); i++)
            data_nod[i]=arb->data[i];
        deseneaza_nod(data_nod, xc, yc);
    }
}

void denumire(int marime, char* s)
{
    int x,y;
    settextstyle(BOLD_FONT, 0, marime);
    setcolor(LIGHTCYAN);
    int latime_text=textwidth(s);
    int inaltime_text=textheight(s);
    x=(latime_ecran-latime_text)/2;
    y=(inaltime_ecran-inaltime_text)/32;
    outtextxy(x, y, s);
}

int navigare()
{
    int lat=50,inalt=30;
    int x_stanga_sus=latime_ecran*3/4;
    int y_stanga_sus=inaltime_ecran/5;
    int x_dreapta_jos=latime_ecran*15/16;
    int y_dreapta_jos=inaltime_ecran/4;
    setlinestyle(0, 0, 5);

    rectangle(x_stanga_sus, y_stanga_sus, x_dreapta_jos+lat, y_dreapta_jos+inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+110, y_stanga_sus+25, "Informatii");

    rectangle(x_stanga_sus, y_dreapta_jos+inalt, x_dreapta_jos+lat, y_dreapta_jos+4*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+60, y_stanga_sus+3*inalt, "Apasati aici pentru");
    outtextxy(x_stanga_sus+60, y_stanga_sus+4*inalt, "a introduce formula");

    rectangle(x_stanga_sus, y_dreapta_jos+4*inalt, x_dreapta_jos+lat, y_dreapta_jos+6*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+70, y_stanga_sus+6*inalt, "Notatia postfixata");

    rectangle(x_stanga_sus, y_dreapta_jos+6*inalt, x_dreapta_jos+lat, y_dreapta_jos+8*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+70, y_stanga_sus+8*inalt, "Arborele formulei");

    rectangle(x_stanga_sus, y_dreapta_jos+8*inalt, x_dreapta_jos+lat, y_dreapta_jos+10*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+125, y_stanga_sus+10*inalt, "Derivata");

    rectangle(x_stanga_sus, y_dreapta_jos+10*inalt, x_dreapta_jos+lat, y_dreapta_jos+12*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+55, y_stanga_sus+12*inalt, "Derivata de ordinul 2");

    rectangle(x_stanga_sus, y_dreapta_jos+12*inalt, x_dreapta_jos+lat, y_dreapta_jos+14*inalt);
    settextstyle(BOLD_FONT, 0, LIGHTCYAN);
    outtextxy(x_stanga_sus+135, y_stanga_sus+14*inalt, "Iesire");

    int x,y;

    while (ismouseclick(WM_LBUTTONDOWN) == 0)
    {
        x = mousex();
        y = mousey();
    }

    if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_stanga_sus && y < y_dreapta_jos+inalt)
        return 1;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+inalt && y < y_dreapta_jos+4*inalt)
        return 2;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+4*inalt && y < y_dreapta_jos+6*inalt)
        return 3;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+6*inalt && y < y_dreapta_jos+8*inalt)
        return 4;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+8*inalt && y < y_dreapta_jos+10*inalt)
        return 5;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+10*inalt && y < y_dreapta_jos+12*inalt)
        return 6;
    else if (x > x_stanga_sus && x < x_dreapta_jos+lat && y > y_dreapta_jos+12*inalt && y < y_dreapta_jos+14*inalt)
        return 0;
}

void alegere()
{
    clearmouseclick(WM_LBUTTONDOWN);
    switch (navigare())
    {

    case 0:
    {
        exit(0);
        break;
    }

    case 1:
    {
        clearviewport();
        denumire(8,"Derivare formala");
        navigare();
        outtextxy(latime_ecran/16,inaltime_ecran/6,"Informatii:");
        outtextxy(latime_ecran/16,inaltime_ecran/6+40,"1.Programul accepta ca necunoscuta doar litera 'x' mic.");
        outtextxy(latime_ecran/16,inaltime_ecran/6+65,"2.Programul accepta ca operanzi numere rationale sub forma 'a.b'.");
        outtextxy(latime_ecran/16,inaltime_ecran/6+90,"3.Operatiile acceptate sunt adunare, scadere, inmultire, impartire, ridicare la putere");
        outtextxy(latime_ecran/16+textwidth("si"),inaltime_ecran/6+115,"si radical(sub forma ridicarii la putere cu numere rationale ex:4^0.5=2).");
        outtextxy(latime_ecran/16,inaltime_ecran/6+140,"4.Functiile acceptate sunt: ln, sin, cos, tg, ctg.");
        alegere();
        break;
    }

    case 2:
    {
        clearviewport();
        denumire(8,"Derivare formala");
        navigare();
        outtextxy(latime_ecran/16,inaltime_ecran/6,"Preluati formula de la tastatura:");
        char c=getch();
        int poz=0;
        expr[poz]=NULL;
        notatia_Postfixata.clear();
        while(c != 13)
        {
            clearviewport();
            denumire(8,"Derivare formala");
            navigare();
            outtextxy(latime_ecran/16,inaltime_ecran/6,"Preluati formula de la tastatura:");
            if(c == 8)
                if(poz > 0)
                {
                    poz--;
                    expr[poz]=NULL;
                }
                else;
            else if(poz < 60)
            {
                expr[poz]=c;
                poz++;
                expr[poz]=NULL;
            }
            outtextxy(latime_ecran/16,inaltime_ecran/5,expr);
            c=getch();
        }
        if(expr[0])
        {
            ecuatie_de_derivat=expr;
            notatia_Postfixata=Postfixare(ecuatie_de_derivat);
            arbore_expresie=construire(notatia_Postfixata);
        }
        alegere();
        break;
    }

    case 3:
    {
        clearviewport();
        denumire(8,"Derivare formala");
        navigare();
        outtextxy(latime_ecran/16,inaltime_ecran/6,"Formula postfixata:");
        if(notatia_Postfixata[0])
        {
            int i;
            char v[notatia_Postfixata.size()];
            for(i=0; i<notatia_Postfixata.size(); i++)
                v[i]=notatia_Postfixata[i];
            v[i]=NULL;
            outtextxy(latime_ecran/16,inaltime_ecran/5,v);
        }
        else
            outtextxy(latime_ecran/16,inaltime_ecran/5,"Ai uitat sa introduci expresia!");
        alegere();
        break;
    }

    case 4:
    {
        arbore_expresie=simplifica(arbore_expresie);
        if(arbore_expresie)
        {
            int fereastra_arbore=initwindow(latime_ecran, inaltime_ecran, "Arbore expresie");
            setcurrentwindow(fereastra_arbore);
            settextstyle(BOLD_FONT,0,2);
            setcolor(LIGHTCYAN);
            int latime_text=textwidth("Iesire");
            int inaltime_text=textheight("Iesire");
            outtextxy((latime_ecran/2-latime_text/3),(inaltime_ecran-inaltime_text)/100,"Iesire");
            deseneaza_arbore(arbore_expresie,1,0,latime_ecran/latime_arbore(arbore_expresie),inaltime_ecran/inaltime_arbore(arbore_expresie));
            clearmouseclick(WM_LBUTTONDOWN);
            int x=0,y=0;
            while ((x == 0 || y == 0))
            {
                x=mousex();
                y=mousey();
                if(x > latime_ecran/2-latime_text/3 && x < latime_ecran/2-latime_text/3+latime_text && y > (inaltime_ecran-inaltime_text)/100 && y < (inaltime_ecran-inaltime_text)/100+inaltime_text && ismouseclick(WM_LBUTTONDOWN))
                    closegraph(CURRENT_WINDOW);
                else
                {
                    x=0;
                    y=0;
                    clearmouseclick(WM_LBUTTONDOWN);
                }
            }
        }
        setcurrentwindow(0);
        alegere();
        break;
    }

    case 5:
    {
        clearviewport();
        denumire(8,"Derivare formala");
        navigare();
        outtextxy(latime_ecran/16,inaltime_ecran/6,"Derivata:");

        arbore_expresie=simplifica(arbore_expresie);
        arbore_derivat=derivare(arbore_expresie);
        arbore_derivat=simplifica(simplifica(arbore_derivat));
        notatia_inordine=inordine(arbore_derivat);

        if(notatia_inordine[0])
        {
            int i;
            char v[notatia_inordine.size()];
            for(i=0; i<notatia_inordine.size(); i++)
                v[i]=notatia_inordine[i];
            v[i]=NULL;
            outtextxy(latime_ecran/16,inaltime_ecran/5,v);
        }
        else
            outtextxy(latime_ecran/16,inaltime_ecran/5,"Ai uitat sa introduci expresia!");

        alegere();
        break;
    }

    case 6:
    {
        clearviewport();
        denumire(8,"Derivare formala");
        navigare();
        outtextxy(latime_ecran/16,inaltime_ecran/6,"Derivata de oridnul 2:");

        arbore_derivat_2=derivare(arbore_derivat);
        arbore_derivat_2=simplifica(simplifica(arbore_derivat_2));
        notatia_inordine_2=inordine(arbore_derivat_2);

        if(notatia_inordine_2[0])
        {
            int i;
            char v[notatia_inordine_2.size()];
            for(i=0; i<notatia_inordine_2.size(); i++)
                v[i]=notatia_inordine_2[i];
            v[i]=NULL;
            outtextxy(latime_ecran/16,inaltime_ecran/5,v);
        }
        else
            outtextxy(latime_ecran/16,inaltime_ecran/5,"Ai uitat sa introduci expresia!");
        alegere();
        break;
    }

    default:
    {
        alegere();
        break;
    }

    }
}

int main()
{
    initwindow(latime_ecran, inaltime_ecran, "Derivare formala");
    denumire(8,"Derivare formala");
    alegere();
    return 0;
}
