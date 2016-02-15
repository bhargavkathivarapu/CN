#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<bitset>
#include<cstring>
using namespace std;
//char H[16]=('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
int  errorat(char m[],int n,int nr)
{
    char t='0';
    int p=0;
    for(int j=0;j<nr;j++)
    {
        for(int i=0;i<n;i++)
        {
            if(((i+1)>>j)&1)
            {
                if(t==m[i])
                {
                    t='0';
                }
                else
                {
                    t='1';
                }
            }
        }
       // cout<<j<<" "<<t<<" "<<m[(1<<j)-1]<<endl;
        if(t!='0')
        {
            p=p+(1<<j);
        }
        t='0';
    }
    return p;
}
int findnr(int n)
{
    int r=0;
    while((1<<r)<(n+r+1))
    {
        r++;
    }
    return r;
}


void hextobin(char h[],char b[],int n)
{
    int sum=0;
   for(int i=n-1;i>=0;i--)
   {
       if(h[i]>='0' && h[i]<='9')
       {
           sum=sum+(h[i]-48)*pow(16,n-i-1);
       }
       else
        {
            sum=sum+(h[i]-55)*pow(16,n-i-1);
        }
   }

    itoa(sum,b,2);
}
char* bintohex(char b[],int n)
{
   int sum=0;
   for(int i=n-1;i>=0;i--)
   {
       if(b[i]=='1')
       {
           sum=sum+pow(2,n-i-1);
       }
   }
   char buffer[30];
   cout<<itoa(sum,buffer,16);
}
int main()
{
    char m[32];
    char a[32];int ch;
    int n;
    memset(m,'\0',32);
    memset(a,'\0',32);
    cout<<"Choose an input method:\n";
    cout<<"1.Hexadecimal\n2.Binary\n";
    cin>>ch;

    switch(ch)
    {

    case 1:
        char h[32];
        cout<<"Enter message(hexadecimal):";
        cin>>h;
        n=strlen(h);
        hextobin(h,a,n);
        //cout<<a;
        n=strlen(a);
        break;
    case 2:
        cout<<"Enter message(binary):";
        cin>>a;
        n=strlen(a);

        break;
    }


    int nr=findnr(n);


   int nm=n+nr;

    for(int i=0,j=0,k=0;i<nm;i++)
    {
        if(i+1==pow(2,j))
        {
            m[i]='0';
            j++;
        }
        else
        {
            m[i]=a[k++];
        }
    }
    cout<<"Input Message:"<<a<<endl;
    cout<<"Formatted Message:"<<m<<endl;

    for(int i=0;i<nm;i++)
    {
        for(int j=0;j<nr;j++)
        {
            if(((i+1)>>j)&1)
            {
                if(m[(1<<j)-1]==m[i])
                {
                    m[(1<<j)-1]='0';
                }
                else
                {
                    m[(1<<j)-1]='1';
                }

            }
        }
    }

    cout<<"Message after addition of Hamming codes: "<<m<<endl;
    cout<<"Message after addition of Hamming codes(hexadecimal):";
    bintohex(m,strlen(m));
    cout<<endl;
    m[8]='1';
    cout<<"After Error:"<<m<<endl;
    int ep=errorat(m,nm,nr);
    if(ep==0)
    {
        cout<<"No Error\n";
    }
    else
    {
        cout<<"Error at pos:"<<ep<<endl;
    }




return 0;
}
