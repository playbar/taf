#include "Parser.h"
#include "servant/Application.h"

#ifdef HAVE_READLINE
#   include <readline/readline.h>
#   include <readline/history.h>
#endif

extern FILE* yyin;
extern int yydebug;

using namespace std;
//using namespace taf;


Parser* parser;

Parser::Parser(const AdminRegPrx& admin, const NodePrx & node) :
    _admin(admin),
    _node(node)
{
}

ParserPtr Parser::createParser(AdminRegPrx admin, NodePrx node)
{
    return new Parser(admin, node);
}

void Parser::usage()
{
    cout <<
        "##命令                                 \t\t 功能\n"
        "help                                   \t\t 打印帮助消息.\n"
        "exit, quit                             \t\t 退出.\n"
        "\n"
        "app list                               \t\t 应用列表.\n"
        "\n"
        "node list                              \t\t 节点列表.\n"
        "node ping NODE                         \t\t ping节点.\n"
        "node shutdown NODE                     \t\t 停止节点.\n"
        "node server start APP.SERVER           \t\t 通过node启动服务(不通知主控，仅供测试).\n"
        "node server stop  APP.SERVER           \t\t 通过node停止服务(不通知主控，仅供测试).\n"
        "\n"
        "server list   [more]                   \t\t server列表.\n"
        "server state  APP.SERVER_NODE          \t\t server状态.\n"
        "server start  APP.SERVER_NODE          \t\t 启动server.\n"
        "server stop   APP.SERVER_NODE          \t\t 停止server.\n"
        "server restart APP.SERVER_NODE         \t\t 重启server.\n"
        "server load   APP.SERVER_NODE          \t\t server重新加载配置.\n"
        "server notify APP[.SERVER][_NODE] MESSAGE  \t 通知server.\n"
        "server notify APP1.SERVER1_NODE1 ... MESSAGE  \t 通知多个server.\n"
        "server patch  APP.SERVER_NODE          \t\t 发布server.\n"
        "server patch  subborn [APP.SERVER_]NODE SRC DST\t 向node发布指定文件.\n"
        "server patch  percent APP.SERVER_NODE  \t\t 发布server进度.\n"      
        "\n"
        "profile get   PROFILE                  \t\t 获取模板.\n"
        "\n"
        ;
}

void Parser::getAllApplicationNames()
{
    try
    {
        string sResult;
        cout << "##应用分类列表" <<endl;
        vector<string> vApps =  _admin->getAllApplicationNames(sResult);
        if(vApps.size() == 0)
        {
            cout<<sResult<<endl;
        }
        for(unsigned i=0; i<vApps.size(); i++)
        {
            cout<<vApps[i]<<endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::getAllNodeNames()
{
    try
    {
        string sResult;
        cout << "##节点列表" <<endl;
        vector<string> vNodes = _admin->getAllNodeNames(sResult);

        if(vNodes.size() == 0)
        {
            cout<<sResult<<endl;
        }
        for(unsigned i=0; i<vNodes.size(); i++)
        {
            cout<< vNodes[i] <<endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::pingNode(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: node ping NODE");
        return;
    }

    try
    {
        string sResult;
        cout << "##节点状态" << endl;
        if(_admin->pingNode(args.front(), sResult))
        {
            cout << "node '"<< args.front() <<"' is up" << endl;
        }
        else
        {
            cout << "node '"<< args.front() <<"' is down. " << sResult << endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::shutdownNode(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: node shutdown NODE");
        return;
    }
    try
    {
        string sResult;
        cout << "##shutdown节点结果" << endl;
        cout<< (_admin->shutdownNode(args.front(), sResult) == 0 ? "成功" : "失败."+sResult ) <<endl;
    }
    catch(const TafException& ex)
    {
        cout<<"失败:" << ex.what()<<endl;
    }
}


void Parser::startServerByNode(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: node server start APP.SERVER");
        return;
    }
    try
    {
        string sResult;
        cout << "##stateServerByNode结果" << endl;

        vector<string> vServerArgs = serverIdParse(args[0] + "_");
        cout<< _node->startServer(vServerArgs[0], vServerArgs[1], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<"失败:" << ex.what()<<endl;
    }
}


void Parser::stopServerByNode(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: node server stop APP.SERVER");
        return;
    }
    try
    {
        string sResult;
        cout << "##stopServerByNode结果" << endl;

        vector<string> vServerArgs = serverIdParse(args[0] + "_");
        cout<< _node->stopServer(vServerArgs[0], vServerArgs[1], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<"失败:" << ex.what()<<endl;
    }
}


void Parser::getAllServerIds(const vector<string>& args)
{
    //默认不分段展示服务列表

    bool sliced     = false;

    //当启用分段展示时 默认0行开始 1次20行展示
    unsigned iLineNum    = 0;
    unsigned iCount      = 20;
    static unsigned giLastLineNum = 0;

    if(args.size() >= 1)
    {
        if(args[0] != "more")
        {
            invalidCommand("Usage: server list [more [ linenum [ count ] ]");
            return;
        }
        sliced = true;
        iLineNum = args.size() >=2?TC_Common::strto<int>(args[1]):giLastLineNum;
        if(args.size() >=3) iCount = TC_Common::strto<int>(args[2]);
    }

    try
    {
        cout.flags (ios::left);

        string sResult;
        _vServer = _admin->getAllServerIds(sResult);

        if(_vServer.size() == 0)
        {
            cout<<sResult<<endl;
            return;
        }
        sort(_vServer.begin(),_vServer.end());
        vector<vector<string> > vServer;

        if(sliced == false)
        {
            _vServer.swap(vServer);
        }
        else
        {
            if(iLineNum >= _vServer.size()-1)
            {
                iLineNum = 0;
            }
            for(unsigned i = iLineNum; i<_vServer.size(); i++)
            {
                vServer.push_back(_vServer[i]);
                giLastLineNum = i;
                if(vServer.size() >= iCount) break;
            }   
        }

        unsigned *length = new unsigned[vServer[0].size()];
        memset(length, 0, sizeof(unsigned) * vServer[0].size());
        for(unsigned i=0; i<vServer.size(); i++)
        {
            for(unsigned j=0; j<vServer[i].size(); j++)
            {
                if(length[j] < 16) length[i] = 16;
                length[j] = (vServer[i][j].length()+5 > length[j]) ? vServer[i][j].length()+5 : length[j];
            }
        }
        cout << setw(length[0]) << "##server名称"<< setw(length[1])
                                << "已设置状态" << setw(length[2]) 
                                << "目前状态(在主控db)"<< setw(length[3])
                                << "服务类型"<<endl;

        for(unsigned i=0; i<vServer.size(); i++)
        {
            for(unsigned j=0; j<vServer[i].size(); j++)
            {
                cout<<setw(length[j])<<vServer[i][j];
            }
            cout<<endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::stateServer(const vector<string>& args)
{
    cout.flags (ios::left);

    if(args.size() != 1)
    {
        invalidCommand("Usage: server state APP.SERVER_NODE");
        return;
    }

    try
    {
        string sResult;
        vector<string> vServerArgs = serverIdParse(args[0]);
        ServerStateDesc state;
        int iRet = _admin->getServerState(vServerArgs[0], vServerArgs[1], vServerArgs[2], state, sResult);
        if(iRet == 0)
        {
            cout << setw(24) <<"##在主控的设置状态";
            cout << setw(24) <<"在主控的当前状态";
            cout << setw(24) <<"在节点的当前状态";
            cout << setw(24) <<"进程id" << endl;
            cout << setw(24) << state.settingStateInReg;
            cout << setw(24) << state.presentStateInReg;
            cout << setw(24) << state.presentStateInNode;
            cout << setw(24) << state.processId << endl;
        }
        else
        {
            cout << sResult << endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::startServer(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: server start APP.SERVER_NODE");
        return;
    }

    try
    {
        string sResult;
        cout<< "##启动server结果(0=成功)" <<endl;
        vector<string> vServerArgs = serverIdParse(args[0]);
        cout<< _admin->startServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::stopServer(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: server stop APP.SERVER_NODE");
        return;
    }

    try
    {
        string sResult;
        cout<< "##停止server结果(0=成功) " <<endl;
        vector<string> vServerArgs = serverIdParse(args[0]);
        cout<< _admin->stopServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::restartServer(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: server restart APP.SERVER_NODE");
        return;
    }

    try
    {
        string sResult;
        cout<< "##重启server结果(0=成功) " <<endl;
        vector<string> vServerArgs = serverIdParse(args[0]);
        cout<< _admin->restartServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::notifyServer(const vector<string>& args)
{
    if(args.size() < 2)
    {
        invalidCommand("Usage: server notify APP[.SERVER][_NODE] MESSAGE");
        return;
    }
    try
    {
        string sResult;
        vector<string> vServerId;
        bool  bBatch = false;
        //若为批量通知 server notify APP msg 或 server notify APP.SERVER msg 格式
        if( args.size() == 2 && (args[0].find("_") == string::npos || args[0].find(".") == string::npos))
        {
            bBatch = true;
            string  s;
            vector<vector<string> > vServer = _admin->getAllServerIds(s);
            if(vServer.size() == 0)
            {
                cout<< s << endl;
                return;
            }
            for(unsigned i = 0; i< vServer.size();i++)
            {
                vector<string> v = serverIdParse(vServer[i].at(0));
                if(v[0] == args[0]  || v[0]+"."+v[1] == args[0])
                vServerId.push_back(vServer[i].at(0));
            }

            if(vServerId.size() == 0)
            {
               invalidCommand("Usage: server notify APP[.SERVER][_NODE] MESSAGE");                
               return;
            }
        }
        else
        {
            for(unsigned i = 0; i< args.size()-1;i++)
            {
                vServerId.push_back(args[i]);
            }
        }

        for(unsigned i=0; i< vServerId.size();i++)
        {
            vector<string> vServerArgs = serverIdParse(vServerId[i]);
            //node 指定
            if(bBatch )
            {              
                cout.fill('-'); 
                cout.width(60);
                cout<<endl<<"("+TC_Common::tostr(i+1)+"'th in "+TC_Common::tostr(vServerId.size())+")"<<endl;
             
            }
            cout<< "##通知"<<vServerId[i]<<"结果(0=成功) " <<endl;      
            cout<< _admin->notifyServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], args[args.size()-1], sResult)<<endl;               
            cout<< sResult << endl;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
    cout<<setfill(' '); 
}

void Parser::patchServer(const vector<string>& args)
{
    if(args.size() < 1)
    {
        invalidCommand("Usage: server patch APP.SERVER_NODE [VERSION]");
        return;
    }

    try
    {
        int iRet =  -1;
        string sResult;
        string sSevrerVersion = args.size()>1?args[1]:"";
        vector<string> vServerArgs = serverIdParse(args[0]);
        iRet = _admin->patchServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], false, sSevrerVersion, "", sResult);
        cout.setf(ios::left,ios::adjustfield);
        cout<<setw(24)<<"##发布server(0=成功) " ;
        cout<<setw(24)<< "结果描述"<<endl;
        cout<<setw(24)<<iRet;
        cout<<setw(24)<<sResult << endl<<endl;


        //如果1.5.0版本后node 启用进度查看
        string sNodeVersion = "";
        iRet = _admin->getNodeVesion(vServerArgs[2],sNodeVersion,sResult);
        if(iRet != 0)
        {
            return;
        }
        vector<int> vi = TC_Common::sepstr<int>(sNodeVersion,".");
        if(vi.size() < 3)
        {
            return;
        }
        //判断版本
        if(vi[0] < 1 || (vi[0] == 1 && vi[1] < 5) || (vi[0] == 1 && vi[1] == 5 && vi[2] == 0))
        {
            return;
        }   
        vector<string> vs;
        vs.push_back(args[0]);
        vs.push_back("-1");
        getPatchPercent(vs);
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::patchSubborn(const vector<string>& args)
{
    if(args.size() != 3)
    {
        invalidCommand("Usage: server patch subborn APP.SERVER_NODE|NODE SRC_FILE DST_FILE");
        return;
    }

    //获取服务
    string sApplication, sServerName, sNodeName;

    try
    {
        vector<string> vServerArgs = serverIdParse(args[0]);
        sApplication = vServerArgs[0];
        sServerName  = vServerArgs[1];
        sNodeName    = vServerArgs[2];
    }
    catch (TafException & ex)
    {
        sNodeName = args[0];
    }
    catch (...)
    {
        std::cout << "unkown exception:split APP.SERVER_NODE|NODE error" << std::endl;
        return ;
    }

    //发起请求
    try
    {
        string sResult;
        int iRet = _admin->patchSubborn(sApplication, sServerName, sNodeName, args[1], args[2], sResult);

        cout.setf(ios::left,ios::adjustfield);
        cout<<setw(24)<<"##发布文件(0=成功) " ;
        cout<<setw(24)<< "结果描述"<<endl;
        cout<<setw(24)<<iRet;
        cout<<setw(24)<<sResult << endl<<endl;

        if (!sApplication.empty() && !sServerName.empty())
        {
            vector<string> vs;
            vs.push_back(args[0]);
            vs.push_back("-1");
            getPatchPercent(vs);
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::getPatchPercent(const vector<string>& args)
{
    if(args.size() < 1)
    {
        invalidCommand("Usage: server patch percent APP.SERVER_NODE [COUNT]");
        return;
    }
    try
    {

        int iRet        = 0;
        PatchInfo tPatchInfo;
        int iCount      = args.size()>=2?TC_Common::strto<int>(args[1]):5;
        string sResult;
        vector<string> vServerArgs = serverIdParse(args[0]);
        cout.setf(ios::left,ios::adjustfield);
        cout<<setw(24)<< "##获取进度(0=成功)";
        cout<<setw(24)<< "进度百分比";
        cout<<setw(24)<< "最近修改时间";
        cout<<setw(24)<< "版本";
        cout<<setw(24)<< "结果描述"<<endl;
        

        while(1)
        {
            int i   = 0;
            iRet    = _admin->getPatchPercent(vServerArgs[0], vServerArgs[1], vServerArgs[2], tPatchInfo);
            cout<<setw(24)<<iRet;
            cout<<setw(24)<<tPatchInfo.iPercent;
            cout<<setw(24)<<TC_Common::tm2str(tPatchInfo.iModifyTime,"%Y-%m-%d %H:%M:%S"); 
            cout<<setw(24)<<tPatchInfo.sVersion;
            cout<<setw(24)<<tPatchInfo.sResult << endl;
            if(iRet == 0 && (iCount == -1 || i< iCount) && tPatchInfo.bPatching == true)
            {    
                sleep(1);
                i++;
                continue;
            }
            break;
        }
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::loadServer(const vector<string>& args)
{
    if(args.size() != 1)
    {
        invalidCommand("Usage: server load APP.SERVER_NODE");
        return;
    }

    try
    {
        string sResult;
        cout<< "##加载server结果(0=成功) " <<endl;
        vector<string> vServerArgs = serverIdParse(args[0]);
        cout<< _admin->loadServer(vServerArgs[0], vServerArgs[1], vServerArgs[2], sResult)<<endl;
        cout<< sResult << endl;
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::getProfileTemplate(const vector<string>& args)
{

    if(args.size() != 1)
    {
        invalidCommand("Usage: profile get   PROFILE");
        return;
    }

    string sResultDesc, sProfileTemplate;
    try
    {
        _admin->getProfileTemplate(args[0], sProfileTemplate, sResultDesc);
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }

    cout<<sResultDesc << endl << endl;
    cout<<sProfileTemplate << endl;
}

void Parser::shutdown()
{
    try
    {
        invalidCommand("'shutdown registry' not implement");
        //_admin->shutdown();
    }
    catch(const TafException& ex)
    {
        cout<<ex.what()<<endl;
    }
}

void Parser::showBanner()
{
    cout << "TAF " << TAF_VERSION << " by WSRD Tencent." << endl;
}

void Parser::getInput(char* buf, int& result, int maxSize)
{
    if(!_commands.empty())
    {
        if(_commands == ";")
        {
            result = 0;
        }
        else
        {
            result = min(maxSize, static_cast<int>(_commands.length()));
            strncpy(buf, _commands.c_str(), result);
            _commands.erase(0, result);
            if(_commands.empty())
            {
                _commands = ";";
            }
        }
    }
    else if(isatty(fileno(yyin)))
    {

#ifdef HAVE_READLINE

        const char* prompt = parser->getPrompt();
        char* line = readline(const_cast<char*>(prompt));
        if(!line)
        {
            result = 0;
        }
        else
        {
            if(*line)
            {
                add_history(line);
            }

            result = strlen(line) + 1;
            if(result > maxSize)
            {
                free(line);
                error("input line too long");
                result = 0;
            }
            else
            {
                strcpy(buf, line);
                strcat(buf, "\n");
                free(line);
            }
        }

#else

        cout << parser->getPrompt() << flush;

        string line;
        while(true)
        {
            char c = static_cast<char>(getc(yyin));
            if(c == EOF)
            {
                if(line.size())
                {
                    line += '\n';
                }
                break;
            }

            line += c;

            if(c == '\n')
            {
                break;
            }
        }

        result = (int) line.length();
        if(result > maxSize)
        {
            error("input line too long");
            buf[0] = EOF;
            result = 1;
        }
        else
        {
            strcpy(buf, line.c_str());
        }

#endif

    }
    else
    {
        if(((result = (int) fread(buf, 1, maxSize, yyin)) == 0) && ferror(yyin))
        {
            error("input in flex scanner failed");
            buf[0] = EOF;
            result = 1;
        }
    }
}

void Parser::nextLine()
{
    _currentLine++;
}

void Parser::continueLine()
{
    _continue = true;
}

const char* Parser::getPrompt()
{
    assert(_commands.empty() && isatty(fileno(yyin)));

    if(_continue)
    {
        _continue = false;
        return "(cont) ";
    }
    else
    {
        return "taf>> ";
    }
}

void Parser::scanPosition(const char* s)
{
    string line(s);
    string::size_type idx;

    idx = line.find("line");
    if(idx != string::npos)
    {
        line.erase(0, idx + 4);
    }

    idx = line.find_first_not_of(" \t\r#");
    if(idx != string::npos)
    {
        line.erase(0, idx);
    }

    _currentLine = atoi(line.c_str()) - 1;

    idx = line.find_first_of(" \t\r");
    if(idx != string::npos)
    {
        line.erase(0, idx);
    }

    idx = line.find_first_not_of(" \t\r\"");
    if(idx != string::npos)
    {
        line.erase(0, idx);

        idx = line.find_first_of(" \t\r\"");
        if(idx != string::npos)
        {
            _currentFile = line.substr(0, idx);
            line.erase(0, idx + 1);
        }
        else
        {
            _currentFile = line;
        }
    }
}

void Parser::invalidCommand(const char* s)
{
    error(string(s) + "\n(`help' for more info)");
}

void Parser::invalidCommand(const string& s)
{
    cout << "##命令错误" << endl;
    invalidCommand(s.c_str());
}

void Parser::error(const char* s)
{
    if(_commands.empty() && !isatty(fileno(yyin)))
    {
        cerr << _currentFile << ':' << _currentLine << ": " << s << endl;
    }
    else
    {
        cerr << "error: " << s << endl;
    }
    _errors++;
}

void Parser::error(const string& s)
{
    error(s.c_str());
}

void Parser::warning(const char* s)
{
    if(_commands.empty() && !isatty(fileno(yyin)))
    {
        cerr << _currentFile << ':' << _currentLine << ": warning: " << s << endl;
    }
    else
    {
        cerr << "warning: " << s << endl;
    }
}

void Parser::warning(const string& s)
{
    warning(s.c_str());
}

int Parser::parse(FILE* file, bool debug)
{
    yydebug = debug ? 1 : 0;

    assert(!parser);
    parser = this;

    _errors = 0;
    _commands.empty();
    yyin = file;
    assert(yyin);

    _currentFile = "";
    _currentLine = 0;
    _continue = false;
    nextLine();

    int status = yyparse();
    if(_errors)
    {
        status = EXIT_FAILURE;
    }

    parser = 0;
    return status;
}

vector<string> Parser::serverIdParse(const string & sServerId)
{
    //参数分解
    //[0] [1]    [2]
    //APP.SERVER_NODE

    string::size_type uMark1 = sServerId.find(".");
    string::size_type uMark2 = sServerId.find("_");

    if(uMark1 == string::npos || uMark2 == string::npos || uMark1 - uMark2 <= 1)
    {
        throw TafException("server id 格式有误，应为: APP.SERVER_NODE");
    }

    vector<string> args;
    args.push_back(sServerId.substr(0, uMark1));
    args.push_back(sServerId.substr(uMark1+1, uMark2-uMark1-1));
    args.push_back(sServerId.substr(uMark2+1));

    return args;
}




