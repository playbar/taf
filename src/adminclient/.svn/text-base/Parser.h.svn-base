#ifndef __OND_ADMIN_CLIENT_PARSE_H__ 
#define __OND_ADMIN_CLIENT_PARSE_H__ 

#include <iostream>
#include <vector>

#include "AdminReg.h"
#include "Node.h"

//
// Stuff for flex and bison
//

#define YYSTYPE std::vector<std::string>
#define YY_DECL int yylex(YYSTYPE* yylvalp)
YY_DECL;
int yyparse();

//
// I must set the initial stack depth to the maximum stack depth to
// disable bison stack resizing. The bison stack resizing routines use
// simple malloc/alloc/memcpy calls, which do not work for the
// YYSTYPE, since YYSTYPE is a C++ type, with constructor, destructor,
// assignment operator, etc.
//
#define YYMAXDEPTH  10000 // 10000 should suffice. Bison default is 10000 as maximum.
#define YYINITDEPTH YYMAXDEPTH // Initial depth is set to max depth, for the reasons described above.

//
// Newer bison versions allow to disable stack resizing by defining
// yyoverflow.
//
#define yyoverflow(a, b, c, d, e, f) yyerror(a)

using namespace taf;

class Parser;
typedef taf::TC_AutoPtr<Parser> ParserPtr;

class Parser : public TC_HandleBase
{
public:

    static ParserPtr createParser(AdminRegPrx, NodePrx);

    void usage();

    /******************
     * application 命令
     */
    //application 列表
    void getAllApplicationNames();


    /******************
     * node 命令
     */

    //node列表
    void getAllNodeNames();

    //ping 特定node
    void pingNode(const vector<string>& args);

    //停止特定node
    void shutdownNode(const vector<string>& args);

    //通过node启动server
    void startServerByNode(const vector<string>& args);

    //通过node停止server
    void stopServerByNode(const vector<string>& args);

    /******************
     * server 命令
     */
    //server 列表
    void getAllServerIds(const vector<string>& args);;

    //查询特定server状态
    void stateServer(const vector<string>& args);

    //启动特定server
    void startServer(const vector<string>& args);

    //停止特定server
    void stopServer(const vector<string>& args);

    //重启特定server
    void restartServer(const vector<string>& args);

    /**
     * 通知服务
     * @param args
     */
    void notifyServer(const vector<string>& args);

    //发布特定server
    void patchServer(const vector<string>& args);

	//发布特定文件
	void patchSubborn(const vector<string>& args);

    //发布进度
    void getPatchPercent(const vector<string>& args);

    //加载特定server
    void loadServer(const vector<string>& args);

    /******************
     * registry 命令
     */
    //停止registry
    void shutdown();
    //版本信息
    void showBanner();

    //获取模板
    void getProfileTemplate(const vector<string>& args);


    /******************
     * 内部辅助函数
     */

    void getInput(char*, int&, int);
    void nextLine();
    void continueLine();
    const char* getPrompt();
    void scanPosition(const char*);

    void invalidCommand(const char*);
    void invalidCommand(const std::string&);
    void error(const char*);
    void error(const std::string&);

    void warning(const char*);
    void warning(const std::string&);

    int parse(FILE*, bool);
    int parse(const std::string&, bool);
    vector<string> serverIdParse(const string & sServerId);

private:

    Parser(const taf::AdminRegPrx &, const taf::NodePrx &);

    std::string _commands;
    AdminRegPrx _admin;
    NodePrx _node;
    bool _continue;
    int _errors;
    int _currentLine;
    std::string _currentFile;

    vector<vector<string> > _vServer; 
};

extern Parser* parser; // The current parser for bison/flex


#endif
