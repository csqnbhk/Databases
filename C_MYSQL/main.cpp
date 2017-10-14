/*********************************************************************************
                       function:简单回顾一下那些MYSQL的API
                       author:Demon
                       Time:2017/8/19
**********************************************************************************/
//#include<windows.h>
#include"c_mysql.h"

int main()
{ 
	
	
	int exit = 1;
	const char*quit = "exit";
	C_MYSQL_POOL*p = C_MYSQL_POOL::GetInstance();
	p->SetMYSQLInfo("localhost", "root", "123456", "students");
	p->InitMYSQLPool(106);//(这里有时候有访问冲突，有点纳闷）
	Sleep(100);
	C_MYSQL*temp = p->GetConnectionObject();
	
	temp->GetServerInfo();

	
	while (exit)
	{
		temp->InputQueryBuffer();
		int return_query = temp->QueryData();
		switch (return_query)
		{
		case 1:
		{
			temp->TableHeaderInfo();
			temp->OutputQueryInfo();
			break;
		}
		case -1:
		{
			cout << "执行SQL语句error" << endl;
			if (!strcmp(quit, temp->get_querybuffer()))
			{
				exit = 0;
			}
			break;
		}
		case -2:
		{
			cout << "执行了没有返回值的SQL语句" << endl;
			break;
		}
		default:
		{
			cout << "UnKnown return value" << endl;
			break;
		}

		}
	}
	
	
	delete p;

	return 0;
}