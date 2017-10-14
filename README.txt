#这个project就是简单的调用一下MYSQL官网的那几个常用API,以回顾自己学习过的知识点。
#目前知道应该还有个Bug
###  有时候出现访问冲突，当连接数大于2时有时就出现。也是今天才发现。
###不过，在调试一小会又不见了。修改一下又不见了。也不知道有没有修改好。
###有点小纠结。


# 关于console输出出现乱码的处理： 
#1.在查询数据库之前进行该操作： mysql_query(&mysql,"set names gbk");用来设置客户端的字符集
###上面那句话相当于下面的三句话：
###mysql> SET character_set_client = gbk;
###mysql> SET character_set_results = gbk;
###mysql> SET character_set_connection =gbk;
