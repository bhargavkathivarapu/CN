#include<iostream>
#include<cmath>
#include<bitset>
using namespace std;
int find(int k)
{
    int i=0;
    while((k=k>>1)>0)
    {
        //cout<<k<<" ";
        i++;

    }
    return i;

}
int main()
{
    int p,m;
    cout<<"Enter polynomial in decimal\n";
    cin>>p;
    cout<<"Enter Message:";
    cin>>m;
    cout<<"Input Message:"<<std::bitset<16>(m)<<endl;
    int dp=find(p);
    cout<<dp<<endl;
    int d=(m<<dp);

    int dm=find(d);
    cout<<dm<<endl;
    int dr=p<<(dm-dp);
  cout<< std::bitset<16>(dr)<<endl<<std::bitset<16>(d)<<endl;
    int i=0;
    while(i<=(dm-dp))
    {
        if((d>>(dm-dp-i))>=pow(2,dp))
        {
            //cout<<"hi";
            d=(d^dr);
        }
        else
        {
            d=(d^0);
        }

        i++;
        dr=dr>>1;
          cout<<std::bitset<16>(d)<<endl;
            //cout<<std::bitset<16>(dr)<<endl;
            //cout<<"------------------------------------"<<endl;
    }

    int fm=(m<<dp)^d;
     cout<<"Final Message:"<<std::bitset<16>(fm)<<endl;


return 0;
}
