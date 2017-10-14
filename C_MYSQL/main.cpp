/*********************************************************************************
                       function:�򵥻ع�һ����ЩMYSQL��API
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
	p->InitMYSQLPool(106);//(������ʱ���з��ʳ�ͻ���е����ƣ�
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
			cout << "ִ��SQL���error" << endl;
			if (!strcmp(quit, temp->get_querybuffer()))
			{
				exit = 0;
			}
			break;
		}
		case -2:
		{
			cout << "ִ����û�з���ֵ��SQL���" << endl;
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