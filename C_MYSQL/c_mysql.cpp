
#include"c_mysql.h"

/**********************************************************************************************************************
                         Mutex互斥量实现
                         author：Demon
                         Time:2017/9/1
**********************************************************************************************************************/
mutex::mutex():Holder(0),HolderCounts(0)
{
	InitializeCriticalSection(&this->cs);
}
mutex::~mutex()
{
	DeleteCriticalSection(&this->cs);
}
bool mutex::Lock()
{
	if (::GetCurrentThreadId() == this->Holder)
	{
		this->HolderCounts++;
		return false;
	}
	else
	{
		
		EnterCriticalSection(&this->cs);
		MyAssert(this->Holder==0);
		this->Holder = ::GetCurrentThreadId();
		this->HolderCounts++;
		MyAssert(this->HolderCounts==1);
	}
	return true;
}
bool mutex::UnLock()
{
	if (::GetCurrentThreadId()!= this->Holder)
	{
		return false;
	}
	else
	{  
		MyAssert(this->HolderCounts>=1)
		this->HolderCounts--;
		if (this->HolderCounts == 0)
		{
			this->Holder = 0;
			LeaveCriticalSection(&this->cs);
		}
	}
	return true;
}
bool mutex::TryLock()
{
	if (::GetCurrentThreadId()== this->Holder)
	{
		this->HolderCounts++;
		return false;
	}
	else
	{
		if (::TryEnterCriticalSection(&this->cs))
		{
			MyAssert(this->Holder == 0);
			this->Holder = ::GetCurrentThreadId();
			this->Holder++;
			MyAssert(this->HolderCounts == 1);
		}
	}
	return true;
}


/****************************************************************************************

                C_MYSQL类的实现
                author:Demon
                Time:2017/8/19
*****************************************************************************************/

mutex*C_MYSQL::mysqlmutex = new mutex();

void C_MYSQL::WriteToLog(const char*text)
{
	FILE*fp = NULL;
	SYSTEMTIME sy ;
	char buffer[512] = { 0 };
	fp = fopen("./connsct_poo.log", "a+");
	if (fp == NULL)
	{
		cout << "open log file failed" << endl;
		return;
	}
	else
	{
		::GetLocalTime(&sy);
		 sprintf(buffer, "%d/%d/%d,%d:%d:%d %s\n",sy.wYear,sy.wMonth,sy.wDay,sy.wHour,sy.wMinute,sy.wSecond,text);
		 fputs(buffer, fp);
		 fflush(fp);
		 fclose(fp);
		 return;
	}
}

C_MYSQL::C_MYSQL(const char*host, const char*user, const char *passwd, const char*db)
{
	this->mysql = { 0 };
	this-> ptr_res = NULL;
	this->sqlrow = NULL;
	this->phost = new char[101];
	this->puser = new char[101];
	this->ppasswd = new char[101];
	this->pdb = new char[101];
	this->querybuffer = new char[101];
	strcpy_s(this->phost, 101,host);
	strcpy_s(this->puser, 101,user);
	strcpy_s(this->ppasswd, 101,passwd);
	strcpy_s(this->pdb, 101,db);


}
C_MYSQL::~C_MYSQL()
{
	my_delete(this->phost);
	my_delete(this->puser);
	my_delete(this->ppasswd);
	my_delete(this->pdb);
	my_delete(this->querybuffer);

}
char*C_MYSQL::get_phost()//获取
{
	return this->phost;
}
char*C_MYSQL::get_puser()
{
	return this->puser;
}
char*C_MYSQL::get_ppasswd()
{
	return this->ppasswd;
}
char*C_MYSQL::get_pdb()
{
	return this->pdb;
}
char*C_MYSQL::get_querybuffer()
{
	return this->querybuffer;
}
void C_MYSQL::set_host(const char*newhost)//设置
{
	strcpy_s(this->phost,100,newhost);
}
void C_MYSQL::set_user(const char*newuser)
{
	strcpy_s(this->puser, 100, newuser);

}
void C_MYSQL::set_passwd(const char*newpasswd)
{
	strcpy_s(this->ppasswd, 100, newpasswd);
}
void C_MYSQL::set_db(const char*newdb)
{
	strcpy_s(this->pdb, 100, newdb);
}
void C_MYSQL::ShowMYSQLError()//错误提示
{
	cout << "error:"<< mysql_errno(&this->mysql)<<","<<mysql_error(&this->mysql) << endl;
}

void C_MYSQL::InitConnectObject()
{
	mysql_init(&this->mysql);	
}

 
bool C_MYSQL::RealConnectDatabase()//连接database
{
	char*temp = new char(100);
	cout << "start connect databases" << endl;
	this->WriteToLog("start connect databases");
	if (!mysql_real_connect(&this->mysql, this->phost, this->puser,this->ppasswd,this->pdb, 0, NULL, 0))
	{
		cout << "connect databases failed" <<endl;
		sprintf(temp, "connect databases %s failed", this->pdb);
		this->WriteToLog(temp);
		this->ShowMYSQLError();
		return false;
	}
	else
	{
		cout << "Connect database " << this->pdb << " successed" << endl;
		sprintf(temp, "Connect database %s  successed", this->pdb);
		this->WriteToLog(temp);
		mysql_query(&this->mysql, "set names gbk");//设置character_set_client,character_set_results,character_set_connection为gbk
		return true;
	}
	
}
bool C_MYSQL::RealConnectDatabase(const char*host, const char*user, const char*passwd, const char*db)
{
	char*temp = new char(100);
	cout << "start connect databases" << endl;
	this->WriteToLog("start connect databases");
	if (!mysql_real_connect(&this->mysql, host, user, passwd, pdb, 0, NULL, 0))
	{
		cout << "connect databases failed" << endl;
		
		sprintf(temp, "connect databases  %s failed", db);
		this->WriteToLog(temp);
		this->ShowMYSQLError();
		return false;
	}
	else
	{
		mysql_query(&this->mysql, "set names gbk");//设置character_set_client,character_set_results,character_set_connection为gbk
		cout << "Connect database " << pdb << " successed" << endl;
		sprintf(temp, "connect databases  %s failed", db);
		this->WriteToLog(temp);
		strcpy_s(this->phost, 100, host);
		strcpy_s(this->puser, 100, user);
		strcpy_s(this->ppasswd, 100, passwd);
		strcpy_s(this->pdb, 100, db);
		return true;

	}
}
bool C_MYSQL::SetWhetherAutoCommit(int Flag)
{
	if (!mysql_autocommit(&this->mysql, Flag))
	return true;
	else
	return false;
}

bool C_MYSQL::Commit()
{
	if (!mysql_commit(&this->mysql))
	return true;
	else
	return false;
}

int C_MYSQL::QueryData()//查询信息
{
	mysqlmutex->Lock();

	int return_flags = mysql_query(&this->mysql,this->querybuffer);
	unsigned int field_count = mysql_field_count(&this->mysql);
	if (return_flags)
	{
		this->ShowMYSQLError();
		mysqlmutex->UnLock();
		return -1;
	}
	if (!field_count)
	{
		mysqlmutex->UnLock();
		return -2;
	}
	this->ptr_res = mysql_use_result(&this->mysql);
	mysqlmutex->UnLock();
	return 1;
}
int C_MYSQL::QueryData(const char*querysql)
{
	mysqlmutex->Lock();
	int return_flags = mysql_query(&this->mysql,querysql);
	unsigned int field_count = mysql_field_count(&this->mysql);
	if (return_flags)
	{
		this->ShowMYSQLError();
		mysqlmutex->UnLock();
		return -1;
	}
	if (!field_count)
	{
		mysqlmutex->UnLock();
		return -2;
	}
	this->ptr_res = mysql_use_result(&this->mysql);
	mysqlmutex->UnLock();
	return 1;
}

void C_MYSQL::FreeResult()
{
	mysql_free_result(this->ptr_res);
	cout << "Free MYSQL_RES* successed" << endl;
	this->WriteToLog("Free MYSQL_RES* successed");
}


void  C_MYSQL::CloseConnect()
{
	mysql_close(&this->mysql);
	cout << "Close Database connected successed" << endl;
	this->WriteToLog("Close Database connected successed");
}

void C_MYSQL::InputQueryBuffer()//输入查询SQL语句
{

    my_zero(this->querybuffer);
	cout << "Please input the query SQL:";
	cin.getline(this->querybuffer,100);

}
void C_MYSQL::InputQueryBuffer(const char*sql)
{
	strcpy_s(this->querybuffer, 100, sql);
}
void  C_MYSQL::TableHeaderInfo()//输出Table字段信息
{
	MYSQL_FIELD *pfield = NULL;
	while (pfield = mysql_fetch_field(this->ptr_res))
	{
		cout << "field name:" << pfield->name << endl;
		cout << "field type:";
		if (IS_NUM(pfield->type))
		{
			cout << "Numeric field" << endl;
		}
		else
		{
			switch (pfield->type)
			{
			case MYSQL_TYPE_VAR_STRING:
				cout << "VARCHAR or VARBINARY field" << endl;
				break;
			case MYSQL_TYPE_LONG:
				cout << "INTEGER field" << endl;
				break;
			default:
				cout << "UnKnown" << endl;
				break;
			}
		}
		cout << "field  size:" << pfield->length << endl;
		if (pfield->flags&AUTO_INCREMENT_FLAG)
			cout << "AUTO_INCREMENT" << endl;
	}
}

void C_MYSQL::OutputQueryInfo()//输出查询所有信息
{
	
	while (this->sqlrow = mysql_fetch_row(this->ptr_res))
	{
		unsigned int field_count = 0;
		//unsigned long *lengths = mysql_fetch_lengths(this->ptr_res);//获取field name大小
		while (field_count <mysql_field_count(&this->mysql) )
		{
			
			if (this->sqlrow[field_count] != NULL)
			cout << this->sqlrow[field_count] << endl;//cout <<lengths[field_count]<<":"<< this->sqlrow[field_count] << endl;
			else
			cout << "NULL" << endl;                   //cout <<lengths[field_count]<<":"<< "NULL" << endl;
			field_count++;
	
		}
		/*
		for (int field_count = 0;field_count < mysql_field_count(&this->mysql);field_count++)
		{
			if (this->sqlrow[field_count] ? this->sqlrow[field_count] : 0)
			{
				cout << this->sqlrow[field_count] << endl;
			}
			else
			{
				cout << "NULL" << endl;
			}
		}*/
		
	}
}

//查询Current的User
void C_MYSQL::GetUserInfo()
{
	cout << "Current user:" << this->get_puser() << endl;
	cout << "Current connection server host and connection type:" << mysql_get_host_info(&this->mysql) << endl;
}
//获取服务器Info
void C_MYSQL::GetServerInfo()
{
	cout << "Server version:" << mysql_get_server_info(&this->mysql)<<endl;
	 
}
//改变连接User
bool C_MYSQL::Change_User(const char*user, const char*password, const char*db)
{
	if (!mysql_change_user(&this->mysql, user, password, db))
	{
		strcpy_s(this->puser, 100, user);
		strcpy_s(this->ppasswd, 100, password);
		strcpy_s(this->pdb, 100, db);
		cout << "Change user successed,now current user is:" <<user<< endl;
		cout << "Connect databases is:" << db << endl;
		return true;
	}
	else
	{
		this->ShowMYSQLError();
		return false;
	}
	
}

/**********************************************************************************************************************

                    mysql_pool实现
                    author：Demon
                    Time:2017/9/1

**********************************************************************************************************************/

C_MYSQL_POOL* C_MYSQL_POOL::Instance = NULL;
mutex*C_MYSQL_POOL::ConnectionPoolMutex = NULL;

C_MYSQL_POOL::C_MYSQL_POOL()
{

	this->phost = new char[101];
	this->puser = new char[101];
	this->ppasswd = new char[101];
	this->pdb = new char[101];

	this->phost[100] = '\0';
	this->puser[100] = '\0';
	this->ppasswd[100] = '\0';
	this->pdb[100] = '\0';
};

C_MYSQL_POOL:: ~C_MYSQL_POOL()
{
	this->DestroyMYSQLPool();
}
C_MYSQL_POOL*C_MYSQL_POOL::GetInstance()
{

	if (!Instance)
	{
		Instance = new C_MYSQL_POOL();
		ConnectionPoolMutex = new mutex();
		return Instance;
	}
	else
	{
		return Instance;
	}
}

void C_MYSQL_POOL::SetMYSQLInfo(const char*host, const char*user, const char *passwd, const char*db)
{

	strcpy_s(this->phost, 100, host);
	strcpy_s(this->puser, 100, user);
	strcpy_s(this->ppasswd, 100, passwd);
	strcpy_s(this->pdb, 100, db);
}
int C_MYSQL_POOL::InitMYSQLPool(int NumConnection)
{
	ConnectionPoolMutex->Lock();
	for (int i = 0;i < NumConnection;i++)
	{
		C_MYSQL*temp = new C_MYSQL(this->phost, this->puser, this->ppasswd, this->pdb);
		temp->InitConnectObject();
		temp->RealConnectDatabase();
		ConnectionPool.push(temp);
	}
	ConnectionPoolMutex->UnLock();
	return NumConnection;
}



C_MYSQL*C_MYSQL_POOL::GetConnectionObject()
{
	ConnectionPoolMutex->Lock();
	C_MYSQL*temp = NULL;
	if (this->ConnectionPool.empty())
	{
		ConnectionPoolMutex->UnLock();
		return temp;
	}
	else
	{
		temp = this->ConnectionPool.front();
		this->ConnectionPool.pop();
		ConnectionPoolMutex->UnLock();
		return temp;
	}
}
void C_MYSQL_POOL::ReleaseConnectionObject(C_MYSQL*ReleaseObject)
{
	MyAssert(ReleaseObject != NULL);
	ConnectionPoolMutex->Lock();
	this->ConnectionPool.push(ReleaseObject);
	ConnectionPoolMutex->UnLock();
}

void C_MYSQL_POOL::DestroyMYSQLPool()
{
	my_delete(this->phost);
	my_delete(this->puser);
	my_delete(this->ppasswd);
	my_delete(this->pdb);
	while (!this->ConnectionPool.empty())
	{
		C_MYSQL*temp = (C_MYSQL*)(this->ConnectionPool.front());
		temp->FreeResult();
		temp->CloseConnect();
		my_delete(temp);
		this->ConnectionPool.pop();
	}
}