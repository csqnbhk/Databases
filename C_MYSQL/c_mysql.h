/*****************************************************************************************************

     ��ͷ�ļ���Ҫ�ع�һ����MYSQL����������ЩAPI
     author��Demon
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
                                 Mutex����������
                                 author��Demon
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
              
			                           mysql����      
									   author��Demon
									   Time:2017/9/1
**********************************************************************************************************************/
class C_MYSQL
{
public:

	
	C_MYSQL(const char*host=0,const char*user=0,const char *passwd=0,const char*db=0);
	virtual~C_MYSQL();
	void ShowMYSQLError();
	

	//�ռ�log
	void WriteToLog(const char*text);

	//��ʼ��MYSQL�ṹ��
	void InitConnectObject();
	
	//�������ݿ�
	bool RealConnectDatabase();
	bool RealConnectDatabase(const char*host, const char*user, const char*passwd, const char*db);


	//�����Ƿ�autocommit
	bool SetWhetherAutoCommit(int Flag=1);//Flag=1���Զ��ύ��Flag=0,�ر��Զ��ύ��ʹ��mysql_commit��������ύ
	bool Commit();


	//����ѯSQL���
	void InputQueryBuffer();
	void InputQueryBuffer(const char*sql);

	//��ѯ��Ϣ
	int QueryData();
	int QueryData(const char*querysql);	

	//�����ṹ
	void TableHeaderInfo();

	//��ѯ��Ϣ����
	void OutputQueryInfo();

	//��ȡ��ǰ��¼�û���Ϣ
	void GetUserInfo();
	void GetServerInfo();

	//�ı�����User
	//my_bool mysql_change_user(MYSQL *mysql, const char *user, const char *password, const char *db)
    bool Change_User(const char*user, const char*password, const char*db);

	//�ͷż�¼��
	void FreeResult();

	//�ر�����
	void CloseConnect();
	
	//��ȡ��Ϣ
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

                                            mysql_pool���ӳ������
											author��Demon
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




