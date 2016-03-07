%{

#include "Parser.h"
#include "Grammar.hpp"


using namespace std;
using namespace taf;

#define YY_INPUT(buf, result, maxSize) parser->getInput(buf, result, maxSize)

%}

WS    [ \t\v\f\r]
NL    [\n]

%option noyywrap
%option always-interactive

%%

^"#"[[:blank:]]*[[:digit:]]+[[:blank:]]*$ {
    parser->scanPosition(yytext);
}

^"#"[[:blank:]]*[[:digit:]]+[[:blank:]]+"\""[^\"]*"\"".*$ {
    parser->scanPosition(yytext);
}

^"#"[[:blank:]]*"line"[[:blank:]]+[[:digit:]]+[[:blank:]]*$ {
    parser->scanPosition(yytext);
}

^"#"[[:blank:]]*"line"[[:blank:]]+[[:digit:]]+[[:blank:]]+"\""[^\"]*"\"".*$ {
    parser->scanPosition(yytext);
}

"//" {
    // C++-style comment
    int c;
    do
    {
    c = yyinput();
    if(c == '\n')
    {
        parser->nextLine();
    }
    }
    while(c != '\n' && c != EOF);
}

"/*" {
    // C-style comment
    while(true)
    {
    int c = yyinput();
    if(c == '\n')
    {
        parser->nextLine();
    }
    else if(c == '*')
    {
        int next = yyinput();
        if(next == '/')
        {
        break;
        }
        else
        {
        unput(next);
        }
    }
    else if(c == EOF)
    {
        parser->warning("EOF in comment");
        break;
    }
    }
}

"help" {
    return TAF_HELP;
}

"quit"|"exit" {
    return TAF_EXIT;
}

"app" {
    return TAF_APPLICATION;
}

"server" {
    return TAF_SERVER;
}

"list" {
    return TAF_LIST;
}

"shutdown" {
    return TAF_SHUTDOWN;
}

"state" {
    return TAF_STATE;
}

"pid" {
    return TAF_PID;
}

"start" {
    return TAF_START;
}

"patch" {
    return TAF_PATCH;
}

"stop" {
    return TAF_STOP;
}

"restart" {
    return TAF_RESTART;
}

"node" {
    return TAF_NODE;
}

"ping" {
    return TAF_PING;
}

"notify" {
    return TAF_NOTIFY;
}

"copying" {
    return TAF_COPYING;
}

"load" {
    return TAF_LOAD;
}

"profile" {
    return TAF_PROFILE;
}

"get" {
    return TAF_GET;
}

"percent" {
    return TAF_PERCENT;
}

"subborn" {
    return TAF_SUBBORN;
}

{WS}*(\\{WS}*{NL})? {
    size_t len = strlen(yytext);
    for(size_t i = 0; i < len; ++i)
    {
    if(yytext[i] == '\\')
    {
        parser->continueLine();
    }
    else if(yytext[i] == '\n')
    {
        parser->nextLine();
    }
    }
}

{NL}|; {
    size_t len = strlen(yytext);
    for(size_t i = 0; i < len; ++i)
    {
    if(yytext[i] == '\n')
    {
        parser->nextLine();
    }
    }
    return ';';
}

\" {
    // "..."-type strings
    string s;
    while(true)
    {
    char c = static_cast<char>(yyinput());
    if(c == '"')
    {
        break;
    }
    else if(c == EOF)
    {
        parser->warning("EOF in string");
        break;
    }
    else if(c == '\n')
    {
        s += c;
        parser->nextLine();
    }
    else if(c == '\\')
    {
        char next = static_cast<char>(yyinput());
        switch(next)
        {
        case '\\':
        case '"':
        {
            s += next;
            break;
        }
        
        case 'n':
        {
            s += '\n';
            break;
        }
        
        case 'r':
        {
            s += '\r';
            break;
        }

        case 't':
        {
            s += '\t';
            break;
        }
        
        case 'v':
        {
            s += '\v';
            break;
        }
        
        case 'f':
        {
            s += '\f';
            break;
        }
        
        default:
        {
            s += c;
            unput(next);
        }
        }
    }
    else
    {
        s += c;
    }
    }
    yylvalp->clear();
    yylvalp->push_back(s);
    return TAF_STRING;
}

\' {
    // '...'-type strings
    string s;
    while(true)
    {
    char c = static_cast<char>(yyinput());
    if(c == '\'')
    {
        break;
    }
    else if(c == EOF)
    {
        parser->warning("EOF in string");
        break;
    }
    else if(c == '\n')
    {
        s += c;
        parser->nextLine();
    }
    else
    {
        s += c;
    }
    }
    yylvalp->clear();
    yylvalp->push_back(s);
    return TAF_STRING;
}

. {
    // Simple strings
    string s;
    s += yytext[0];
    while(true)
    {
    char c = static_cast<char>(yyinput());
    if(c == EOF)
    {
        break;
    }
    else if(isspace(c) || c == ';')
    {
        unput(c);
        break;
    }
    
    s += c;
    }
    yylvalp->clear();
    yylvalp->push_back(s);
    return TAF_STRING;
}

%%
