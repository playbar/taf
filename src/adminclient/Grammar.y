%{

#include "Parser.h"

using namespace std;
using namespace taf;

void
yyerror(const char* s)
{
    parser->invalidCommand(s);
}

%}

%pure_parser

%token TAF_HELP
%token TAF_EXIT
%token TAF_APPLICATION
%token TAF_NODE
%token TAF_SERVER
%token TAF_PING
%token TAF_LIST
%token TAF_SHUTDOWN
%token TAF_STRING
%token TAF_START
%token TAF_NOTIFY
%token TAF_STOP
%token TAF_RESTART
%token TAF_PATCH
%token TAF_STATE
%token TAF_PID
%token TAF_SHOW
%token TAF_COPYING
%token TAF_LOAD
%token TAF_PROFILE
%token TAF_GET
%token TAF_PERCENT
%token TAF_SUBBORN

%%

// ----------------------------------------------------------------------
start
// ----------------------------------------------------------------------
: commands
{
}
|
{
}
;

// ----------------------------------------------------------------------
commands
// ----------------------------------------------------------------------
: commands command
{
}
| command
{
}
;

// ----------------------------------------------------------------------
command
// ----------------------------------------------------------------------
: TAF_HELP ';'
{
    parser->usage();
}
| TAF_EXIT ';'
{
    return 0;
}
| TAF_APPLICATION TAF_LIST ';'
{
    parser->getAllApplicationNames();
}
| TAF_NODE TAF_LIST ';'
{
    parser->getAllNodeNames();
}
| TAF_NODE TAF_PING optional_strings ';'
{
    parser->pingNode($3);
}
| TAF_NODE TAF_SHUTDOWN optional_strings ';'
{
    parser->shutdownNode($3);
}
| TAF_NODE TAF_SERVER TAF_START optional_strings ';'
{
    parser->startServerByNode($4);
}
| TAF_NODE TAF_SERVER TAF_STOP optional_strings ';'
{
    parser->stopServerByNode($4);
}
| TAF_SERVER TAF_LIST optional_strings ';'
{
    parser->getAllServerIds($3);
}
| TAF_SERVER TAF_STATE optional_strings ';'
{
    parser->stateServer($3);
}
| TAF_SERVER TAF_START optional_strings ';'
{
    parser->startServer($3);
}
| TAF_SERVER TAF_STOP optional_strings ';'
{
    parser->stopServer($3);
}
| TAF_SERVER TAF_RESTART optional_strings ';'
{
    parser->restartServer($3);
}
| TAF_SERVER TAF_NOTIFY optional_strings ';'
{
    parser->notifyServer($3);
}
| TAF_SERVER TAF_PATCH TAF_PERCENT optional_strings ';'
{
    parser->getPatchPercent($4);
}
| TAF_SERVER TAF_PATCH TAF_SUBBORN optional_strings ';'
{
    parser->patchSubborn($4);
}
| TAF_SERVER TAF_PATCH optional_strings ';'
{
    parser->patchServer($3);
}
| TAF_SERVER TAF_LOAD optional_strings ';'
{
    parser->loadServer($3);
}
| TAF_PROFILE TAF_GET optional_strings ';'
{
    parser->getProfileTemplate($3);
}
| TAF_SHUTDOWN ';'
{
    parser->shutdown();
}
| TAF_APPLICATION optional_strings ';'
{
    if($2.empty())
    {
    parser->invalidCommand("invalid command `application'");
    }
    else
    {
    parser->invalidCommand("invalid command: `application " + $2.front() + "'");
    }
}
| TAF_SERVER optional_strings ';'
{
    if($2.empty())
    {
    parser->invalidCommand("invalid command `server'");
    }
    else
    {
    parser->invalidCommand("invalid command: `server " + $2.front() + "'");
    }
}
| TAF_NODE optional_strings ';'
{
    if($2.empty())
    {
    parser->invalidCommand("invalid command `node'");
    }
    else
    {
    parser->invalidCommand("invalid command: `node " + $2.front() + "'");
    }
}
| strings ';'
{
    parser->invalidCommand("invalid command `" + $1.front() + "'");
}
| error ';'
{
    yyerrok;
}
| ';'
{
}
;

// ----------------------------------------------------------------------
strings
// ----------------------------------------------------------------------
: TAF_STRING strings
{
    $$ = $2;
//    $$.push_front($1.front());
    $$.insert($$.begin(), $1.front());
}
| TAF_STRING
{
    $$ = $1;
}
;

// ----------------------------------------------------------------------
optional_strings
// ----------------------------------------------------------------------
: TAF_STRING optional_strings
{
    $$ = $2;
//    $$.push_front($1.front());
    $$.insert($$.begin(), $1.front());
}
|
{
    $$ = YYSTYPE()
}
;

%%
