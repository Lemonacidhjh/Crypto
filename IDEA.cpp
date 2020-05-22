#include <iostream>
#include <string>
#include <math.h>
using namespace std;
const unsigned int N=256;    //mod(pow(2,8))=256
const unsigned int MUL=65537;  //16 比特的整数做 mod(pow(2,16)+1)=65537 的乘法运算
const unsigned int ADD=65536;  //16 比特的整数做 mod(pow(2,16))=65536 的加法运算
string key_change(unsigned int n,int k)// 十进制数倒序变换为 k 位二进制
{
 string result;
 for(int i=0;i<k;i++)
 {  
   if((i!=0)&&!(i%4))  result+=",";
   if(n%2)    result+="1";
   else    result+="0";
   n/=2;
 }
 return result;
}
string change(unsigned int n,int k)// 十进制数顺序变换为 k 位二进制
{
 string result;
 for(int i=0;i<k;i++)
 {
   if((i!=0)&&!(i%4))  result=","+result;
   if(n%2)    result="1"+result;
   else    result="0"+result;
   n/=2;
 }
 return result;
}
void set_key(unsigned int key[],unsigned int z[9][6])// 生成密钥模块
{
 int i,j,k,flag=0,t=0;
 unsigned int sum,temp[9][6][16];
 for(i=0;i<9;i++)
 {
   for(j=0;j<6;j++)
   {
     for(k=0;k<16;k++)
       temp[i][j][k]=key[(flag+t++)%128];
     if(!(i==j==0)&&((6*i+j)%8)==7)  flag+=25;
   }
 }
 for(i=0;i<9;i++)
 {
   for(j=0;j<6;j++)
   {
     sum=0;
     for(k=0;k<16;k++)
       if(temp[i][j][k])  sum+=pow(2,k);
       z[i][j]=sum;
   }
 }
}
void set_m(unsigned int write[],unsigned int x[])// 生成明文模块  
{
 int i,j;
 unsigned int sum;
 for(i=0;i<64;i+=16)
 {
   sum=0;
   for(j=0;j<16;j++)
     if(write[i+j])  sum+=pow(2,j);
   x[i/16]=sum;
 }
}
void string_bb(string str,unsigned int result[])// 字符序列转换为二进制
{
 int i,j;
 unsigned int temp;
 for(i=0;i<str.length();i++)
 {
   temp=str[i];
   for(j=0;j<8;j++)
   {
     if(temp%2)    result[8*i+j]=1;
     else  result[8*i+j]=0;
     temp=temp/2;
   }
 }  
}
void encrypt(unsigned int x[],unsigned int z[9][6])
{
 int t,j;
 unsigned int result[20];
 for(t=0;t<8;t++)
 {
   result[0]=(x[0]*z[t][0])%MUL;    //X1 和第 1 个子密钥块作乘法运算
   result[1]=(x[1]+z[t][1])%ADD;    //X2 和第 2 个子密钥块作加法运算
   result[2]=(x[2]+z[t][2])%ADD;    //X3 和第 3 个子密钥块作加法运算
   result[3]=(x[3]*z[t][3])%MUL;    //X4 和第 4 个子密钥块作乘法运算
   result[4]=result[0]^result[2];    //(1) 和 (3) 结果作异或运算
   result[5]=result[1]^result[3];    //(2) 和 (4) 结果作异或运算
   result[6]=(result[4]*z[t][4])%MUL;  //(5) 的结果与第 5 个子密钥块作乘法运算
   result[7]=(result[5]+result[6])%ADD;//(6) 和 (7) 结果作加法运算
   result[8]=(result[7]*z[t][5])%MUL;  //(8) 的结果与第 6 个子密钥块作乘法运算　
   result[9]=(result[6]+result[8])%ADD;//(7) 和 (9)  结果作加法运算
   result[10]=result[0]^result[8];    //(1) 和 (9) 结果作异或运算
   result[11]=result[2]^result[8];    //(3) 和 (9)  结果作异或运算
   result[12]=result[1]^result[9];    //(2) 和 (10)  结果作异或运算
 result[13]=result[3]^result[9];    //(4) 和 (10)  结果作异或运算
   cout<<"\n  第 "<<t+1<<" 趟  "<<endl<<endl;//(4) 和 (10)  结果作异或运算
   for(j=0;j<14;j++)
     cout<<"\tStep"<<j+1<<"\t"<<" \t\t"<<change(result[j],16)<<endl;
   cout<<"\nresult[10]="<<result[10]<<" "<<change(result[10],16)<<"\t";
   cout<<"result[12]="<<result[12]<<" "<<change(result[12],16)<<endl;
   cout<<"result[11]="<<result[11]<<" "<<change(result[11],16)<<"\t";
   cout<<"result[13]="<<result[13]<<" "<<change(result[13],16)<<endl;
   cout<<"-------------------------------------------------------------------------------"<<endl;
   x[0]=result[10];x[1]=result[12];x[2]=result[11];x[3]=result[13];// 除最后一轮（第 9 轮）外，第 2 和第 3 块交换
 }
 x[0]=(x[0]*z[t][0])%MUL;//X1 和第 1 个子密钥块作乘法运算
 x[1]=(x[1]+z[t][1])%ADD;//X2 和第 2 个子   密钥块作加法运算
 x[2]=(x[2]+z[t][2])%ADD;//X3 和第 3 个子密钥块作加法运算
 x[3]=(x[3]*z[t][3])%MUL;//X4 和第 4 个子密钥块作乘法运算  
 cout<<" 加密结果是 :"<<endl;
 cout<<"\t"<<change(x[0],16)<<"\t"<<change(x[1],16)<<endl;
 cout<<"\t"<<change(x[2],16)<<"\t"<<change(x[3],16)<<endl;
}
int main()
{
 int i,j,t;
 unsigned int sum,temp,x[4],z[9][6],key[128],write[64];
 string m,k,str;
 cout<<"\t\t*****************************\n";
 cout<<"\t\t\tIDEA 密码算法 ";
 cout<<"\n\t\t*****************************\n";
 cout<<"\n 请输入明文字符串 :";cin>>m;
 cout<<"\n 请输入密钥 :";cin>>k;
 cout<<"\n\t\t 密钥为 k="<<k<<endl<<endl;
 string_bb(k,key);// 将密钥字符序列转换为二进制，存放于数组 key[128]
 cout<<"k="<<endl;
 for(i=0;i<128;i++)
 {
   cout<<key[i];
   if((i!=0)&&(i%8)==7)  cout<<" ";
   if((i!=0)&&(i%32)==31)  cout<<endl;  
 }
 string_bb(m,write);// 将明文字符序列转换为二进制，存放于数组 write[64]
 cout<<"\n\t\t\t 明文是 m="<<m<<endl<<endl;
 cout<<"m="<<endl;
 for(i=0;i<64;i++)
 {
   cout<<write[i];
   if((i!=0)&&(i%8)==7)  cout<<" ";
   if((i!=0)&&(i%32)==31)  cout<<endl;  
 }
 set_key(key,z);// 生成密钥模块 z[9][6]
 cout<<"\n=====> 密钥模块 :\n"<<endl;
 for(i=0;i<9;i++)
 {
   for(j=0;j<6;j++)
   {
     cout<<"z["<<i+1<<"]["<<j+1<<"] "<<key_change(z[i][j],16)<<"\t";
     if(j%2)  cout<<endl;
   }
   cout<<endl;
 }
 for(i=0,j=0;i<4;i++,j+=2)    x[i]=m[j+1]*pow(2,8)+m[j];
 set_m(write,x);// 生成明文模块 x[4]
 cout<<"\n=====> 明文模块 :\n"<<endl;
 for(i=0;i<4;i+=2)  cout<<"x["<<i<<"]="<<x[i]<<"\t"<<key_change(x[i],16)<<"\t"<<"x["<<i+1<<"]="<<x[i+1]<<"\t"<<key_change(x[i+1],16)<<endl;
 cout<<"*******************************************************************************\n"<<endl;
 encrypt(x,z);
 return 0;  
}
