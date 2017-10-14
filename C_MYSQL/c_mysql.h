/*****************************************************************************************************

     该头文件主要回顾一下在MYSQL官网看的那些API
     author：Demon
     Time:2017/8/19

*********************************************************************************************************/
#pragma once
#include<iostream>
#include<mysql.h>
#include<queue>
#pragma comment(lib,"libmysql.lib")
using namespace std;



#define my_delete(p) {if(p){  delete p;p=NULL;} }
#define my_zero(p)   { memset(p,0,strlen(p));}
#define MyAssert(p)    { if(!p){ cout<<"Assert:"<<__FILE__<<":"<<__LINE__<<endl;} }

/**********************************************************************************************************************
                                 Mutex互斥量的类
                                 author：Demon
                                 Time:2017/9/1
**********************************************************************************************************************/

class mutex
{
public:
	mutex();
	virtual~mutex();
	bool Lock();
	bool UnLock();
	bool TryLock();
protected:

private:
	DWORD Holder;
	int HolderCounts;
	CRITICAL_SECTION cs;

};


/**********************************************************************************************************************
              
			                           mysql操作      
									   author：Demon
									   Time:2017/9/1
**********************************************************************************************************************/
class C_MYSQL
{
public:

	
	C_MYSQL(const char*host=0,const char*user=0,const char *passwd=0,const char*db=0);
	virtual~C_MYSQL();
	void ShowMYSQLError();
	

	//日记log
	void WriteToLog(const char*text);

	//初始化MYSQL结构体
	void InitConnectObject();
	
	//连接数据库
	bool RealConnectDatabase();
	bool RealConnectDatabase(const char*host, const char*user, const char*passwd, const char*db);


	//设置是否autocommit
	bool SetWhetherAutoCommit(int Flag=1);//Flag=1，自动提交。Flag=0,关闭自动提交。使用mysql_commit完成事务提交
	bool Commit();


	//填充查询SQL语句
	void InputQueryBuffer();
	void InputQueryBuffer(const char*sql);

	//查询信息
	int QueryData();
	int QueryData(const char*querysql);	

	//输出表结构
	void TableHeaderInfo();

	//查询信息遍历
	void OutputQueryInfo();

	//获取当前登录用户信息
	void GetUserInfo();
	void GetServerInfo();

	//改变连接User
	//my_bool mysql_change_user(MYSQL *mysql, const char *user, const char *password, const char *db)
    bool Change_User(const char*user, const char*password, const char*db);

	//释放记录集
	void FreeResult();

	//关闭连接
	void CloseConnect();
	
	//获取信息
	char*get_phost();
	char*get_puser();
	char*get_ppasswd();
	char*get_pdb();
	char*get_querybuffer();

	void set_host(const char*newhost);
	void set_user(const char*newuser);
	void set_passwd(const char*newpasswd);
	void set_db(const char*newdb);
	
private:
	static mutex*mysqlmutex;
	MYSQL mysql;
	MYSQL_RES *ptr_res;
	MYSQL_ROW sqlrow;
	char*phost;
	char*puser;
	char*ppasswd;
	char*pdb;
	char*querybuffer;

};

/**********************************************************************************************************************

                                            mysql_pool连接池类操作
											author：Demon
											Time:2017/9/1

**********************************************************************************************************************/

class C_MYSQL_POOL
{
 public:
	 ~C_MYSQL_POOL()noexcept;
    static C_MYSQL_POOL*GetInstance();
	int InitMYSQLPool(int NumConnection=1);
	void SetMYSQLInfo(const char*host , const char*user, const char *passwd , const char*db );

	C_MYSQL*GetConnectionObject();
	void ReleaseConnectionObject(C_MYSQL*ReleaseObject);
	void DestroyMYSQLPool();
protected:

private:
	C_MYSQL_POOL();
	static C_MYSQL_POOL*Instance;
	static mutex*ConnectionPoolMutex;
	queue<C_MYSQL*> ConnectionPool;
	char*phost;
	char*puser;
	char*ppasswd;
	char*pdb;
};




