#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sstream>
#include <queue>
using namespace std;
int length=0; 
int num;
int numb=0;
int tempv;
string varname;
string constname;
FILE *in,*out;
string letter[100000];
char op[20000];
struct ident {
	string name;
	string name2;
	int value;
	int type; //1表示变量，0表示常量, 2表示变量的值（1是指针） 
};
ident idents[100]; 
ident shuzi[200];
int top1=-1,top2=-1,top3=-1;
string temp;
int tempvalue;
int operate(char c)
{
	string num1="",num2="";
	char ch[10];
	if(shuzi[top1].type==1)//是变量 
	{
		fprintf(out,"          \%%%d = load i32, i32* %s\n",++numb,shuzi[top1].name2.c_str());
		sprintf(ch,"\%%%d",numb);
		shuzi[top1].name2=ch;
		shuzi[top1].type=2; 
	}
	if(shuzi[top1-1].type==1)//是变量
	{
		fprintf(out,"          \%%%d = load i32, i32* %s\n",++numb,shuzi[top1-1].name2.c_str());
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
	} 
	if(shuzi[top1].type==2)
	{
		num1=shuzi[top1].name2;
	}
	else
	{
		sprintf(ch,"%d",shuzi[top1].value);
		num1=ch;
	}
	if(shuzi[top1-1].type==2)
	{
		num2=shuzi[top1-1].name2;
	}
	else
	{
		sprintf(ch,"%d",shuzi[top1-1].value);
		num2=ch;
	}
	
	if(c=='+')
	{
		fprintf(out,"          \%%%d = add i32 %s,%s\n",++numb,num2.c_str(),num1.c_str());
		shuzi[top1-1].value=shuzi[top1].value+shuzi[top1-1].value;
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
		shuzi[top1-1].name="";//叫啥都无所谓了 
	}
	if(c=='-')
	{	
		fprintf(out,"          \%%%d = sub i32 %s,%s\n",++numb,num2.c_str(),num1.c_str());
		shuzi[top1-1].value=shuzi[top1-1].value-shuzi[top1].value;
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
		shuzi[top1-1].name="";//叫啥都无所谓了 
	}
	if(c=='*')
	{
		fprintf(out,"          \%%%d = mul i32 %s,%s\n",++numb,num2.c_str(),num1.c_str());
		shuzi[top1-1].value=shuzi[top1-1].value*shuzi[top1].value;
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
		shuzi[top1-1].name="";//叫啥都无所谓了 
	}
	if(c=='/')
	{
		fprintf(out,"          \%%%d = sdiv i32 %s,%s\n",++numb,num2.c_str(),num1.c_str());
		shuzi[top1-1].value=shuzi[top1-1].value/shuzi[top1].value;
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
		shuzi[top1-1].name="";//叫啥都无所谓了 
	}
	if(c=='%')
	{
		fprintf(out,"          \%%%d = srem i32 %s,%s\n",++numb,num2.c_str(),num1.c_str());
		shuzi[top1-1].value=shuzi[top1-1].value%shuzi[top1].value;
		sprintf(ch,"\%%%d",numb);
		shuzi[top1-1].name2=ch;
		shuzi[top1-1].type=2;
		shuzi[top1-1].name="";//叫啥都无所谓了 
	}
	top1--;
	return shuzi[top1-1].value; 
}
int q[100];
int top=0;
int result= -1;
int MulExp();
int PrimaryExp();
int AddExp();
int UnaryExp();
int ConstDef();
int inputstack[100];
int opstack[100];
int ConstDecl();
int VarDecl();
int Exp();
int Vardef();
int VarDecl();
int Stmt(); 
int Decl();
int Blockitem();
int FuncRParams();
int symbol(string s)
{
	if(s=="(")
	{
		fprintf(out,"(");
		num++;
		return 3;
	}
	else if(s==")")
	{	
		fprintf(out,")");
		num++;
		return 4;
	}	
	else if(s=="{")
	{
		fprintf(out,"{\n");
		num++;
		return 5;
	}
	else if(s=="}")
	{
		fprintf(out,"}");
		num++;
		return 9;
	}
	else if(s==";")
	{
		num++;
		return 8;
	}
	else
		return 0;
} 
 int judgeword(string s,int n)
 {
 	int j=n+1;
 	while(1)
	{
		if((letter[j]>="a"&&letter[j]<="z"||letter[j]>="0"&&letter[j]<="9"||letter[j]>="A"&&letter[j]<="Z"||letter[j]=="_")&&letter[j]!="block")
		{
   			s=(s+letter[j]).c_str();
   			j++;
   		}
   		else
   		{
   			break;
		}
  	}
  	num=j;
 	if(s=="int")
 	{
 		return 1;
 	}
 	else if(s=="main")
 	{
 		fprintf(out,"@main");
 		return 2;
 	}
 	else if(s=="return")
 	{
	 	
 		return 6;
 	}
 	else if(s=="const")
 		return 7;
 	else if(s=="getint")
 		return 10;
 	else if(s=="getch")
 		return 11;
 	else if(s=="getarray")
 		return 12;
 	else if(s=="putint")
 		return 13;
 	else if(s=="putch")
 		return 14;
 	else if(s=="putarray")
 		return 15;
 	else
 	{
 		temp=s;
 		return 3;
 	}
 }
 bool isnonezero(string s)
 {
 	if(s>="1"&&s<="9")
 		return true;
	return false; 
 }
 bool isoctaldigit(string s)
 {
 	if(s>="0"&&s<="7")
 		return true;
 	else
 		return false;
 }
 bool ishexdigit(string s)
 {
 	if((s>="0"&&s<="9"||s>="a"&&s<="f"||s>="A"&&s<="F")&&s!="block")
 		return true;
 	else
 		return false;
 }
int Blockitem()
{
	while(letter[num]=="block")
	{
		num++;
	}
	int j = num;
	if(Decl()>0)
	{
		return 1;	
	}	
	else
	{
		num = j;
		if(Stmt()>0)
			return 2;
		else
		{
			num = j;
			return 0;
		}
	}
}
int Decl()
{
	if(letter[num]=="c")
	{
		if(ConstDecl()>0)
			return 1;
		else
			return 0;
	}
	else if(letter[num]=="i")
	{
		if(VarDecl()>0)
			return 2;
		else
			return 0;
	}
	else
		return 0;
}
int ConstDecl()
{
	if((letter[num]>="a"&&letter[num]<="z")||(letter[num]>="A"&&letter[num]<="Z")||letter[num]=="_")
	{
		int a = judgeword(letter[num],num);
		if(a==7)
		{
			while(letter[num]=="block")
			{
				num++;
			}
			int b = judgeword(letter[num],num);
			while(letter[num]=="block")
			{
				num++;
			}
			if(b==1)
			{			
				if(ConstDef()>0)
				{
					for(int i=0;i<=top3;i++)
					{
						if(idents[i].name==constname)
							return 0;
					}
					idents[++top3].name=constname;
					idents[top3].type=0;
					idents[top3].value=shuzi[0].value;
					idents[top3].name2="";
					while(letter[num]==",")
					{
						num++;
						if(ConstDef()<=0)
						{
							return 0;
						}
						for(int i=0;i<=top3;i++)
						{
							if(idents[i].name==constname)
								return 0;
						}
						idents[++top3].name=constname;
						idents[top3].type=1;
						idents[top3].value=shuzi[0].value;
						idents[top3].name2="";
					}
					
				}
				else
				{
					return 0;
				}
				while(letter[num]=="block")
				{
					num++;
				}
				if(letter[num]==";")
				{
					num++;
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int ConstDef()
{
	top1=-1;
	top2=-1;
	while(letter[num]=="block")
	{
		num++;
	}
	if((letter[num]>="a"&&letter[num]<="z")||(letter[num]>="A"&&letter[num]<="Z")||letter[num]=="_")
	{
		int a = judgeword(letter[num],num);
		for(int i=0;i<=top3;i++)
		{
			if(idents[i].name==temp)
			{
				return 0;
			}
		}
		while(letter[num]=="block")
		{
			num++;
		}
		if(a==3)
		{
			constname=temp;
			if(letter[num]=="=")
			{
				num++;
				while(letter[num]=="block")
				{
					num++;
				}
				if(Exp()>0)
				{
					while(top2!=-1)
					{
						operate(op[top2]);
						top2--;
					}
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int VarDecl()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if((letter[num]>="a"&&letter[num]<="z")||(letter[num]>="A"&&letter[num]<="Z")||letter[num]=="_")
	{
		int a = judgeword(letter[num],num);
		if(a==1)
		{
			numb++;
			fprintf(out,"          \%%%d = alloca i32\n",numb);
			idents[++top3].type=1;
			string name2;
			char ch[50];
			sprintf(ch,"\%%%d",numb);
			name2=ch;
			idents[top3].name2=name2;
			int b =Vardef();
			if(b>0)
			{	
				for(int i=0;i<=top3;i++)//防止重复定义某一变量 
				{
					if(idents[i].name==varname)
						return 0;
				}
				idents[top3].name=varname;
				idents[top3].type=1;
				idents[top3].value=0;
				if(b==2)
				{
					if(shuzi[0].type==0)
					{
						fprintf(out,"          store i32 %d, i32 *%s\n",shuzi[0].value,idents[top3].name2.c_str());
					}
					else if(shuzi[0].type==2)
					{
						fprintf(out,"          store i32 %s, i32 *%s\n",shuzi[0].name2.c_str(),idents[top3].name2.c_str());
					}
					else if(shuzi[0].type==1)
					{
						fprintf(out,"          store i32 *%s, i32 *%s\n",shuzi[0].name2.c_str(),idents[top3].name2.c_str());
					}
					idents[top3].value=shuzi[0].value;//这里可能要修改 
				}
				while(letter[num]=="block")
				{
					num++;
				}
				while(letter[num]==",")
				{
					num++;
					numb++;
					fprintf(out,"          \%%%d = alloca i32\n",numb);
					idents[++top3].type=1;
					sprintf(ch,"\%%%d",numb);
					name2=ch;
					idents[top3].name2=name2;
					int c = Vardef();
					if(c<=0)
					{
						return 0;
					}
					for(int i=0;i<=top3;i++)
					{
						if(idents[i].name==varname)
							return 0;
					}
					idents[top3].name=varname;
					idents[top3].value=0;					
					if(c==2)
					{
						if(shuzi[0].type==0)
						{
							fprintf(out,"          store i32 %d, i32 %d\n",numb,shuzi[0].value);
						}
						else if(shuzi[0].type==2)
						{
							fprintf(out,"          store i32 %d, i32 %s\n",numb,shuzi[0].name2.c_str());
						}	
						else if(shuzi[0].type==1)
						{
							fprintf(out,"          store i32 %d, i32 *%s\n",numb,shuzi[0].name2.c_str());
						}
						idents[top3].value=shuzi[0].value;//这里可能要修改 
					}
				}
				while(letter[num]=="block")
				{
					num++;
				}
				if(letter[num]==";")
				{
					num++;
					return 1;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int Vardef()
{
	top1=-1;
	top2=-1;
	while(letter[num]=="block")
	{
		num++;
	}
	if((letter[num]>="a"&&letter[num]<="z")||(letter[num]>="A"&&letter[num]<="Z")||letter[num]=="_")
	{
		int a = judgeword(letter[num],num);
		int flag=0;
		if(a==3)
		{
			varname=temp;
			while(letter[num]=="block")
			{
				num++;
			}
			if(letter[num]=="=")
			{
				num++;
				while(letter[num]=="block")
				{
					num++;
				}
				if(Exp()>0)
				{
					while(top2!=-1)
					{
						operate(op[top2]);
						top2--;
					}
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int Stmt()
{
	top1=-1;
	top2=-1;
	while(letter[num]=="block")
	{
		num++;
	}
	int j = num ;
	if(letter[num]==";")
	{
		num++;
		return 2;
	}
	if(Exp()>0)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		while(top2!=-1)
		{
			operate(op[top2]);
			top2--;
		}
		if(letter[num]==";")
		{
			num++;
			return 2;
		}
		else
			return 0;
	}
	num=j;
	top1=-1;
	top2=-1;
	if((letter[num]>="a"&&letter[num]<="z")||(letter[num]>="A"&&letter[num]<="Z")||letter[num]=="_")
	{
		num=j;
		int a = judgeword(letter[num],num);
		if(a==6)
		{
			
			if(Exp()>0)
			{
				while(letter[num]=="block")
				{
					num++;
				}
				while(top2!=-1)
				{
					operate(op[top2]);
					top2--;
				}
				if(letter[num]==";")
				{
					num++;
					fprintf(out,"          ret i32 ");
					if(shuzi[0].type==0)
						fprintf(out,"%d",shuzi[0].value);
					else if(shuzi[0].type==2)
						fprintf(out,"%s",shuzi[0].name2.c_str());
					else if(shuzi[0].type==1)
						fprintf(out,"*%s",shuzi[0].name2.c_str());
					return 3;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				num = j;
				return 0;
			}
		}
		else if(a==3)
		{
			int flag=0;
			int biao=0;
			for(int i=0;i<=top3;i++)
			{
				if(idents[i].name==temp)
				{
					if(idents[i].type==0)
						return 0;
					else
					{
						flag=1;
						biao=i;
						break;
					}
				}
			}
			if(flag==0)
			{
				return 0;
			}
			while(letter[num]=="block")
			{
				num++;
			}
			if(letter[num]=="=")
			{
				num++;
				while(letter[num]=="block")
				{
					num++;
				}
				if(Exp()>0)
				{
					while(letter[num]=="block")
					{
						num++;
					}
					while(top2!=-1)
					{
						operate(op[top2]);
						top2--;
					}
					if(letter[num]==";")
					{
						num++;
						if(shuzi[0].type==0)
						{
							fprintf(out,"          store i32 %d, i32 *%s\n",shuzi[0].value,idents[biao].name2.c_str());
						}
						else if(shuzi[0].type==2)
						{
							fprintf(out,"          store i32 %s, i32 *%s\n",shuzi[0].name2.c_str(),idents[biao].name2.c_str());
						}
						else if(shuzi[0].type==1)
						{
							fprintf(out,"          store i32 *%s, i32 *%s\n",shuzi[0].name2.c_str(),idents[biao].name2.c_str());
						}
						idents[biao].value=shuzi[0].value;
						return 1;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					top1=-1;
					top2=-1;
					num = j;
					return 0;
				}
			}
			else
			{
				num = j;
				return 0;
			}
		}
		else
		{
			num=j;
			return 0;
		}
	}
	num=j;
	return 0;
}
int Number(string s,int n)
{
	stringstream ss1;
	int j=n+1;
	int flag=1;
	int ret;
	if((s=="0"&&letter[j]=="x"||s=="0"&&letter[j]=="X")&&letter[j]!="block")//十六进制 
	{
		s=(s+letter[j]).c_str();
 		j++;
 		while(1)
 		{
 			if(ishexdigit(letter[j]))
 			{
 				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>hex>>ret;
				return ret;
			}
		}
	}
	else if(s=="0"&&letter[j]!="x"&&letter[j]!="X")//八进制 
	{
		while(1)
		{
			if(isoctaldigit(letter[j]))
			{
				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>oct>>ret;
				return ret;
			}
		}
	}
	else if(isnonezero(s))
	{
		while(1)
		{
			if(letter[j]>="0"&&letter[j]<="9")
			{
				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>ret;
				return ret;
			}
		}
	}
 num=j;
 return -1;
}
int Exp()
{
	
	while(letter[num]=="block")
	{
		num++;
	}
	if(AddExp()>0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
 } 
int AddExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(MulExp()>0)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		while(letter[num]=="+"||letter[num]=="-")
		{
			while(op[top2]!='('&&top2!=-1)
			{
				operate(op[top2]);
				top2--;
			}
			if(letter[num]=="+") 
				op[++top2]='+';
			else if(letter[num]=="-")
				op[++top2]='-';
			num++;
			while(letter[num]=="block")
			{
				num++;
			}
			if(MulExp()==0)
				return 0;
		}
		return 1;
	}
	else
		return 0;
}
int MulExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(UnaryExp()>0)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		if(letter[num]=="=")
			return 0;
		while(letter[num]=="*"||letter[num]=="/"||letter[num]=="%")
		{
			
			if(op[top2]=='+'||op[top2]=='-'||op[top2]=='('||top2==-1)
			{
				if(letter[num]=="*")
					op[++top2]='*';
				else if(letter[num]=="/")
					op[++top2]='/';
				else if(letter[num]=="%")
					op[++top2]='%';				
			}
			else if(op[top2]=='*'||op[top2]=='%'||op[top2]=='/')
			{
				while(op[top2]=='*'||op[top2]=='%'||op[top2]=='/')
				{
					operate(op[top2]);
					top2--;
				}
				if(letter[num]=="*")
					op[++top2]='*';
				else if(letter[num]=="/")
					op[++top2]='/';
				else if(letter[num]=="%")
					op[++top2]='%';
			}
			num++;
			while(letter[num]=="block")
			{
				num++;
			}
			if(UnaryExp()==0)
				return 0;
			while(letter[num]=="block")
			{
				num++;
			}
		}
		return 1;
	}
	else
		return 0;
 } 
int PrimaryExp(int opt)
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(letter[num]=="(")
	{
		op[++top2]='(';
		num++;
		while(letter[num]=="block")
		{
			num++;
		}
		if(Exp()>0)
		{
			while(letter[num]=="block")
			{
				num++;
			}
			if(letter[num]==")")
			{
				while(op[top2]!='(')
				{
					operate(op[top2]);
					top2--;
				}
				top2--;
				if(opt==-1)
				{
				
					op[++top2]='(';
					ident newident=shuzi[top1];
					shuzi[++top1]=newident;
					shuzi[top1-1].type=0;
					shuzi[top1-1].value=0;
					op[++top2]='-';
					while(op[top2]!='(')
					{
						operate(op[top2]);
						top2--;
					}
					top2--;
				}
				num++;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		
	}
	else if(letter[num]>="a"&&letter[num]<="z"||letter[num]>="A"&&letter[num]<="Z"||letter[num]=="_")//把对应的数字入栈 
	{
		string s = letter[num];
		if(judgeword(s,num)==3)
		{
			int flag=0;
			for(int i=0;i<=top3;i++)
			{
				if(idents[i].name==temp)
				{
					if(opt==-1)
					{
						op[++top2]='(';
						ident newident;
						newident.type=0;
						newident.value=0;
						newident.name="";
						newident.name2="";
						shuzi[++top1]=newident;
						op[++top2]='-';
						shuzi[++top1]=idents[i];
						while(op[top2]!='(')
						{
							operate(op[top2]);
							top2--;
						}
						top2--;
					}
					else if(opt==1)
					{
						shuzi[++top1]=idents[i];
					}
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(letter[num]>="0"&&letter[num]<="9")//number
	{
		int j=num;
		string s=letter[j];
		result=Number(s,j);
		ident newident;
		newident.value=opt*result;
		newident.name="";
		newident.name2="";
		newident.type=0;
		shuzi[++top1]=newident;
		if(result!=-1)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int UnaryExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	int j=num;
	if(letter[num]>="a"&&letter[num]<="z"||letter[num]>="A"&&letter[num]<="Z"||letter[num]=="_")
	{
		int a = judgeword(letter[num],num);
		if(a>=10&&a<=15)
		{
			while(letter[num]=="block")
			{
				num++;
			}
			if(letter[num]=="(")
			{
				num++;
				while(letter[num]=="block")
				{
					num++;
				}
				if(a==10&&letter[num]==")")
				{
					printf("          \%%%d = call i32 @getint()\n",++numb);
					char ch[50];
					sprintf(ch,"\%%%d",numb);
					ident newident;
					newident.value=0;
					newident.name="";
					newident.name2=ch;
					newident.type=2;
					shuzi[++top1]=newident;
					num++;
					return 2;
				}
				else if(a==11&&letter[num]==")")
				{
					printf("          \%%%d = call i32 @getch()\n",++numb);
					char ch[50];
					sprintf(ch,"\%%%d",numb);
					ident newident;
					newident.value=0;
					newident.name="";
					newident.name2=ch;
					newident.type=2;
					shuzi[++top1]=newident;
					num++;
					return 2;
				}
				else if(a==13)
				{
					if(FuncRParams()>0)
					{
						if(shuzi[0].type==1)
							fprintf(out,"          call void @putint(i32 *%s)\n",shuzi[0].name2.c_str());
						else if(shuzi[0].type==0)
							fprintf(out,"          call void @putint(i32 %d)\n",shuzi[0].value);
						else if(shuzi[0].type==2)
							fprintf(out,"          call void @putint(i32 %s)\n",shuzi[0].name2.c_str());
						num++;
						return 2;
					}
					else
					{
						return 0;
					}
					
				}
				else if(a==14)
				{
					if(FuncRParams()>0)
					{
						if(shuzi[0].type==1)
							fprintf(out,"          call void @putch(i32 *%s)\n",shuzi[0].name2.c_str());
						else if(shuzi[0].type==0)
							fprintf(out,"          call void @putch(i32 %d)\n",shuzi[0].value);
						else if(shuzi[0].type==2)
							fprintf(out,"          call void @putch(i32 %s)\n",shuzi[0].name2.c_str());
						num++;
						return 2;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			num=j;
		}
	}
	else
	{
		num=j;
	}
	int opt=1;
	while(letter[num]=="+"||letter[num]=="-")
	{
		if(letter[num]=="-")
		{
			opt = -opt;
		}
		num++;
		while(letter[num]=="block")
		{
			num++;
		}
	}
	if(PrimaryExp(opt)>0)
		return 1;
	else
		return 0;
}
int FuncRParams()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(Exp()>0)
	{
		while(top2!=-1)
		{
			operate(op[top2]);
		}
		while(letter[num]=="block")
		{
			num++;
		}
		while(letter[num]==",")
		{
			num++;
			if(Exp()>0)
			{
				while(letter[num]=="block")
				{
					num++;
				}
			}
			else
			{
				return 0;
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
int FuncDef()
{
	while(letter[num]=="block")
	{
		num++;
	}
	string s=letter[num];
	if(judgeword(s,num)==1)
	{
		fprintf(out,"define dso_local i32 ");
		while(letter[num]=="block")
		{
			num++;
		}
		if(judgeword(letter[num],num)==2)
		{
			while(letter[num]=="block")
				num++;
			if(symbol(letter[num])==3)
			{
				while(letter[num]=="block")
					num++;
				if(symbol(letter[num])==4)
				{
					while(letter[num]=="block")
						num++;
					if(symbol(letter[num])==5)
					{
						while(letter[num]=="block")// {
							num++;
						while(Blockitem()>0)//return
						{
							while(letter[num]=="block")
								num++;	
						}
						if(symbol(letter[num])==9)
						{
							return 1;
						}
					}
				}
			}	
		}
	}
	return 0;
}
int main(int argc,char **argv){
	char w;
 	int i,j;

	in = fopen(argv[1],"r");
	out = fopen(argv[2],"w");
 	int flag=0;
	char line[105];
	while(fgets(line,100,in)!=NULL)
 	{
 		int len=strlen(line);
	 	for(int k=0;line[k]>0;k++)
		{
			if(line[k]==47&&line[k+1]==42)//是/*形注释 
			{
				flag=1;
			} 
			if(flag==1)
			{
				if(k>0)
				{
					if(line[k-1]==42&&line[k]==47)
					{
						flag=0;
					}
				}
				continue;
			}
			if(k>=len)
			{
				break;
			 } 
			if(line[k]==47&&line[k+1]==47)//是//形注释 
			{
				break;
			}
			w=line[k];
  			if(!isblank(w)&&w!='\n'&&w!='\r')
			{
   				letter[length]=w;
   				length++;
  			} 
  			else if(isblank(w)||w=='\n'&&w!='\r')
  			{
  				letter[length]="block";
  				length++;
  				if(w=='\n')
  				{
  					break;
				}
  			}
  			else
  			{
  				break;
  			}
  		}
	}
	int fd=FuncDef();
	if(fd>0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}