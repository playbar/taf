#include "util/tc_mysql.h"
#include <iostream>

using namespace taf;

TC_Mysql mysql;

void test()
{
//    cout << mysql.getVariables("character_set_client") << endl;

    TC_Mysql::MysqlData data;
    data = mysql.queryRecord("select * from t_app_users");
    for(size_t i = 0; i < data.size(); i++)
    {
        cout << data[i]["ID"] << endl;
    }
}

void testInsert()
{
    map<string, pair<TC_Mysql::FT, string> > m;
    m["ID"]     = make_pair(TC_Mysql::DB_INT, "2334");
    m["USERID"] = make_pair(TC_Mysql::DB_STR, "abcttt");
    m["APP"]    = make_pair(TC_Mysql::DB_STR, "abcapbbp");
    mysql.insertRecord("t_user_logs", m);
    mysql.replaceRecord("t_user_logs", m);

    mysql.updateRecord("t_user_logs", m, "where ID=2234");
}

int main(int argc, char *argv[])
{
    try
    {
        try{
        mysql.init("172.25.38.21", "pc", "pc@sn", "db_dmqq_system");
        mysql.connect();

        }catch(exception &ex)
        {
            cout << ex.what() << endl;
        }
        mysql.execute("select * from t_app_users");
        test();

//        sleep(10);
//        test();
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


