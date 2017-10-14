/ 关于console输出出现乱码的处理：
  

 1.在查询数据库之前进行该操作： mysql_query(&mysql,"set names gbk");用来设置客户端的字符集

上面那句话相当于下面的三句话：
mysql> SET character_set_client = gbk;
mysql> SET character_set_results = gbk;
mysql> SET character_set_connection =gbk;