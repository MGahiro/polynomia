﻿// Polynomia.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<iostream>
#include<fstream>
using namespace std;
class  node
{
public:
	double coef;//系数  
	int exp;//指数  
	node *next;
	//构造函数
	node(double a, int b, node* c = NULL)
	{
		coef = a;
		exp = b;
		next = c;
	}
	node(node* c = NULL)
	{
		next = c;
	}
	//析构函数
	~node() {};
};
//多项式类
class polynomia
{
public:
	node * head;
	node* curr;
	//构造函数
	polynomia()
	{
		head = curr = new node;
		head->coef = NULL;
		head->exp = -1;
		head->next = NULL;
		curr = head;
	}	//插入元素
	void insert(node *a)
	{
		node *h = new node();
		h->coef = a->coef;
		h->exp = a->exp;
		h->next = curr->next;
		curr->next = h;
		curr = h;
	}
	void clear()
	{
		node *ctem;
		while (head->next != NULL)
		{
			ctem = head->next;
			head->next = ctem->next;
			delete ctem;
		}
	}
	//析构函数
	~polynomia() {}
}; 
//加法实现
void ADD(polynomia &a, polynomia &b)
{
	node* p, *q, *u, *pre;
	p = a.head->next;
	q = b.head->next;
	pre = a.head;
	while ((p != NULL) && (q != NULL))
	{
		if (p->exp > q->exp)
		{
			pre = p;
			p = p->next;
		}
		else if (p->exp == q->exp)
		{
			double x;
			x = p->coef + q->coef;
			if (x != 0)
			{
				p->coef = x;
				pre = p;
			}
			else
			{
				pre->next = p->next;
				//delete(p);
			}
			p = pre->next;
			u = q;
			q = q->next;
			//delete(u);
		}
		else if (p->exp <q->exp)
		{
			u = q->next;
			q->next = p;
			pre->next = q;
			pre = q;
			q = u;
		}
	}
	if (q != NULL)
	{
		pre->next = q;
	}
}
//减法实现
void SUB(polynomia &a, polynomia &b)
{
	node *p, *q, *u, *pre;
	p = a.head->next;
	q = b.head->next;
	pre = a.head;
	while ((p != NULL) && (q != NULL))
	{
		if (p->exp > q->exp)
		{
			pre = p;
			p = p->next;
		}
		else if (p->exp == q->exp)
		{
			double x;
			x = p->coef - q->coef;
			if (x != 0)
			{
				p->coef = x;
				pre = p;
			}
			else
			{
				pre->next = p->next;
				//delete(p);
			}
			p = pre->next;
			u = q;
			q = q->next;
			//delete(u);
		}
		else if (p->exp < q->exp)
		{
			q->coef = -q->coef;
			u = q->next;
			q->next = p;
			pre->next = q;
			pre = q;
			q = u;
		}
	}
	if (q != NULL)
	{
		pre->next = q;
		pre->next->coef = -(q->coef);
	}
}
//乘法实现
void MUL(polynomia &a, polynomia &b)
{
	polynomia ans;
	node *p, *q, *u;
	p = a.head->next;
	int flag = 0;//定义一个标志变量
	while (p != NULL)
	{
		polynomia tem;
		q = b.head->next;
		while (q != NULL)
		{
			u = new node();
			u->coef = p->coef*q->coef;
			u->exp = p->exp + q->exp;
			tem.insert(u);
			q = q->next;
			//delete u;
		}//第一个多项式第一项乘第二个多项式每一项存入tem
		if (flag == 0)
		{
			ans = tem;
			flag = 1;
		}//标志变量为0表示为首项
		else
		{
			ADD(ans, tem);
		}
		p = p->next;
	}
	a = ans;
}
//除法实现
void DIV(polynomia &a, polynomia &b, polynomia &k)
{
	node *p, *q, *dtem = new node(), *dtemp = new node();
	polynomia ans;
	p = a.head->next;
	q = b.head->next;
	while (p != NULL && q != NULL)
	{
		polynomia tem;
		if (p->exp < q->exp)
		{
			k = ans;
			break;
		}
		else
		{
			dtem->coef = p->coef / q->coef;
			dtem->exp = p->exp - q->exp;
			ans.insert(dtem);
		}
		while (q != NULL)
		{
			dtemp->coef = dtem->coef*q->coef;
			dtemp->exp = dtem->exp + q->exp;
			tem.insert(dtemp);
			q = q->next;
		}//商式第一项乘以除式
		q = a.head->next;
		SUB(a,tem);//两式相减存入 tem为新的除式
		p = b.head->next;
	}
}
double horner(polynomia &a,double x) //秦九韶算法计算多项式的值
{
	double result=0;
	node *p = a.head->next;
	while (p!= NULL) 
	{
		result = result * x + p->coef;
		p = p->next;
	}
	return result;
}
int main()
{
	double x;//系数
	int y;//指数
	double x0;
	char *oper = new char(10);//定义一个表示代数运算的字符串指针并分配空间
	ifstream infile1("input.txt");
	infile1 >>x  >> y;
	polynomia s, t;
	while (x != 0)
	{
		node *li = new node(x, y);
		s.insert(li);
		delete li;
		infile1 >>x>>y ;
	}
	infile1 >> x >>  y ;
	while (x != 0)
	{
		node *li = new node(x, y);
		t.insert(li);
		delete li;
		infile1 >>  x >> y ;
	}
	infile1 >> oper;
	oper[strlen(oper)] = '\0';
	infile1 >> x0;
	infile1.close();
	if (!strcmp(oper, "AND(x,y)"))
	{
		ADD(s, t);
		ofstream out("output.txt");
		char x = 'x';
		node* tem;
		tem = s.head;
		while (tem->next != NULL)
		{
			if (tem->next->exp != 0)
			{
				out << tem->next->coef << "x^" << tem->next->exp;
			}
			else
			{
				out << tem->next->coef;
			}
			while (tem->next->next != NULL)
			{
				if (tem->next->next->coef > 0)
					out << " +";
				break;
			}
			tem = tem->next;
		};
		out << endl;
		double r = horner(s, x0);
		out << "x=" << x0 << "时,多项式值为：" << r;
		out.close();
		return 0;
	}
	if (!strcmp(oper, "SUB(x,y)"))
	{
		SUB(s, t);
		ofstream out("output.txt");
		char x = 'x';
		node* tem;
		tem = s.head;
		while (tem->next != NULL)
		{
			if (tem->next->exp != 0)
			{
				out << tem->next->coef << "x^" << tem->next->exp;
			}
			else
			{
				out << tem->next->coef;
			}
			while (tem->next->next != NULL)
			{
				if (tem->next->next->coef > 0)
					out << " +";
				break;
			}
			tem = tem->next;
		};		
		out << endl;
		double r = horner(s, x0);
		out << "x=" << x0 << "时,多项式值为：" << r;
		out.close();
		return 0;
	}
	if (!strcmp(oper, "MUL(x,y)"))
	{
		MUL(s, t);
		ofstream out("output.txt");
		node* tem;
		tem = s.head;
		while (tem->next != NULL)
		{
			if (tem->next->exp != 0 && tem->next->coef != 0)
			{
				out << tem->next->coef << "x^" << tem->next->exp;
			}
			else if (tem->next->coef != 0)
			{
				out << tem->next->coef;
			}
			while (tem->next->next != NULL)
			{
				if (tem->next->next->coef > 0)
					out << " +";
				break;
			}
			tem = tem->next;
		}
		out << endl;
		double r = horner(s, x0);
		out << "x=" << x0 << "时,多项式值为：" << r;
		out.close();
		return 0;
	}

	if (!strcmp(oper, "DIV(x,y)"))
	{
		polynomia cur;
		DIV(s, t, cur);
		ofstream out("output.txt");
		out << "结果是：";
		node* tem;
		tem = cur.head;
		while (tem->next != NULL)
		{
			if (tem->next->exp != 0 && tem->next->coef != 0)
			{
				out << tem->next->coef << "x^" << tem->next->exp;
			}
			else if (tem->next->coef != 0)
			{
				out << tem->next->coef;
			}
			while (tem->next->next != NULL)
			{
				if (tem->next->next->coef > 0)
					out << " +";
				break;
			}
			tem = tem->next;
		}
		out << endl << "余项是：";
		tem = s.head;
		while (tem->next != NULL)
		{
			if (tem->next->exp != 0 && tem->next->coef != 0)
			{
				out << tem->next->coef << "x^" << tem->next->exp;
			}
			else if (tem->next->coef != 0)
			{
				out << tem->next->coef;
			}
			while (tem->next->next != NULL)
			{
				if (tem->next->next->coef > 0)
					out << " +";
				break;
			}
			tem = tem->next;
		}
		out << endl;
		double r = horner(s, x0);
		out << "x=" << x0 << "时,多项式值为：" << r;
		out.close();
		return 0;
	}
	ofstream fout("output.txt");
	fout << "输入有误,请重新输入";
	fout.close();
	return 0;
	system("pause");
}
