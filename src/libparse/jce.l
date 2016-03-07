
%option outfile="lex.yy.cpp"
%option   yylineno
%{
#include <map>
#include <string>
#include <sstream>
#include <cassert>
#include <errno.h>
#include <math.h>
#include "parse/parse.h"
#include "jce.tab.hpp"

using namespace std;

bool stringToInt64(const string& s, long long& result)
{
    const char* start = s.c_str();
    char* end = 0;
    errno = 0;
    result = strtoll(start, &end, 0);
    return (errno == 0 && start != end);
}

extern "C"
{
    int yywrap()
    {
        return 1;
    }
}

struct include_state
{
	YY_BUFFER_STATE state;
	string file;
};

#define MAX_INCLUDE_DEPTH 100
include_state include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

%}

identifier          [[:alpha:]_][[:alnum:]_]*
anychar             .
integer_constant	(\+|-)?((0[0-7]+)|(0x[[:xdigit:]]+)|([[:digit:]]+))
fractional_constant	(\+|-)?(([[:digit:]]*\.[[:digit:]]+)|([[:digit:]]+\.))
exponent_part		(e|E)(\+|-)?[[:digit:]]+
floating_literal	(({fractional_constant}{exponent_part}?)|((\+|-)?[[:digit:]]+{exponent_part}))[fF]?

%x INCL

%%

"#include"[ \t]*   { BEGIN(INCL); }

<INCL>"\"".*"\""   {
    if ( include_stack_ptr >= MAX_INCLUDE_DEPTH )
    {
        g_parse->error("Includes nested too deeply" );
    }

    string file;
    bool b = g_parse->getFilePath( yytext, file);
    g_parse->currentContextPtr()->addInclude(file);

    //该文件已经解析过
    if(!b)
    {
        --include_stack_ptr;
        if ( include_stack_ptr < 0 )
        {
            include_stack_ptr = 0;
            //yyterminate();
        }
        else
        {
            yy_delete_buffer( YY_CURRENT_BUFFER );
            fclose(yyin);
            yy_switch_to_buffer(include_stack[include_stack_ptr].state );
            g_parse->popFile();
        }
    }
    else
    {
        include_stack[include_stack_ptr].state = YY_CURRENT_BUFFER;
        include_stack[include_stack_ptr].file  = file;
        include_stack_ptr++;

        yyin = fopen( file.c_str(), "r" );
        if ( !yyin )
        {
            g_parse->error("can't open file:" + file);
        }

        yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );

        g_parse->pushFile(file);
    }
    BEGIN(INITIAL);
}

<<EOF>> {
    --include_stack_ptr;
    if ( include_stack_ptr < 0 )
    {
        include_stack_ptr = 0;
        yyterminate();
    }
    else
    {
        yy_delete_buffer( YY_CURRENT_BUFFER );
        fclose(yyin);
        yy_switch_to_buffer(include_stack[include_stack_ptr].state );
        g_parse->popFile();
    }
}

"::" {
    return JCE_SCOPE_DELIMITER;
}

"//" {
    // C++-style comment
    int c;
    do
    {
    	c = yyinput();
    	if(c == '\n')
    	{
    	    g_parse->nextLine();
    	}
    }
    while(c != '\n' && c != EOF);
}

"/*" {
    // C-style comment
    string comment = yytext + 2;
    while(true)
    {
    	int c = yyinput();
    	if(c == '\n')
    	{
    	    comment += static_cast<char>(c);
            g_parse->nextLine();
    	}
    	else if(c == '*')
    	{
    	    int next = yyinput();
    	    if(next == '/')
    	    {
        		break;  //到注释尾部
    	    }
    	    else
    	    {
        		comment += static_cast<char>(c);
    	    	unput(next);
    	    }
    	}
    	else if(c == EOF)
    	{
    	    g_parse->error("EOF in comment");   //到文件尾部, 但是没有出现*/
    	    break;
    	}
    	else
    	{
    	    comment += static_cast<char>(c);
    	}
    }
    if(comment[0] == '*')
    {
        //comment是注释内容
    }
}

{identifier}    {
    StringTokPtr ident  = new StringTok;
    ident->v            = yytext;
    yylval              = ident;
    return g_parse->checkKeyword(ident->v);
}

{identifier}[[:space:]]*"(" {
    StringTokPtr ident  = new StringTok;
    ident->v            = yytext;
    ident->v.erase(ident->v.find_first_of(" \t\v\n\r\f("));

    yylval = ident;

    return JCE_OP;
}

\"  {
    StringTokPtr str = new StringTok;
    while(true)
    {
    	char c = static_cast<char>(yyinput());
    	if(c == '"')
    	{
    	    break;
    	}
    	else if(c == EOF)
    	{
    	    g_parse->error("EOF in string");
    	    break;
    	}
    	else if(c == '\n')
    	{
    	    g_parse->error("newline in string");
    	}
    	else if(c == '\\')
    	{
    	    char next = static_cast<char>(yyinput());
    	    switch(next)
    	    {
        		case '\\':
        		case '"':
        		case '\'':
        		{
        		    str->v += next;
        		    break;
        		}
        		case 'n':
        		{
        		    str->v += '\n';
        		    break;
        		}
        		case 'r':
        		{
        		    str->v += '\r';
        		    break;
        		}
        		case 't':
        		{
        		    str->v += '\t';
        		    break;
        		}
        		case 'v':
        		{
        		    str->v += '\v';
        		    break;
        		}
        		case 'f':
        		{
        		    str->v += '\f';
        		    break;
        		}
        		case 'a':
        		{
        		    str->v += '\a';
        		    break;
        		}
        		case 'b':
        		{
        		    str->v += '\b';
        		    break;
        		}
        		case '?':
        		{
        		    str->v += '\?';
        		    break;
        		}
        		case '0':
        		case '1':
        		case '2':
        		case '3':
        		{
        		    static string octalDigits = "01234567";
        		    unsigned short us = next - '0';
        		    if(octalDigits.find_first_of(next = static_cast<char>(yyinput())) != string::npos)
        		    {
        		    	us = us * 8 + next - '0';
            			if(octalDigits.find_first_of(next = static_cast<char>(yyinput())) != string::npos)
            			{
            			    us = us * 8 + next - '0';
            			}
            			else
            			{
            			    unput(next);
            			}
        		    }
        		    else
        		    {
        		    	unput(next);
        		    }
        		    if(us == 0)
        		    {
        			    g_parse->error("illegal NUL character in string constant");
        		    }
        		    str->v += static_cast<char>(us);
        		    break;
        		}
        		case 'x':
        		{
        		    long long ull = 0;
        		    while(isxdigit(next = static_cast<char>(yyinput())))
        		    {
            			ull *= 16;
            			if(isdigit(next))
            			{
            			    ull += next - '0';
            			}
            			else if(islower(next))
            			{
            			    ull += next - 'a' + 10;
            			}
            			else
            			{
            			    ull += next - 'A' + 10;
            			}
        		    }
        		    unput(next);
        		    if(ull == 0)
        		    {
        			    g_parse->error("illegal NUL character in string constant");
        		    }
        		    str->v += static_cast<char>(ull);
        		    break;
        		}
        		// TODO: add universal character names
        		default:
        		{
        		    str->v += c;
        		    unput(next);
        		}
            }
	    }
        else
	    {
	        str->v += c;
	    }
    }
    yylval = str;
    return JCE_STRING_LITERAL;
}

{integer_constant} {
    IntegerTokPtr itp = new IntegerTok;
    yylval = itp;
    if(!stringToInt64(string(yytext), itp->v))
    {
	    assert(itp->v != 0);
	    string msg = "integer constant `";
	    msg += yytext;
	    msg += "' out of range";
	    g_parse->error(msg);
    }
    return JCE_INTEGER_LITERAL;
}

{floating_literal} {
    errno = 0;
    FloatTokPtr ftp = new FloatTok;
    yylval = ftp;
    string literal(yytext);
    char lastChar = literal[literal.size() - 1];
    if(lastChar == 'f' || lastChar == 'F')
    {
    	literal = literal.substr(0, literal.size() - 1);	// Clobber trailing 'f' or 'F' suffix
    }
    ftp->v = strtod(literal.c_str(), 0);
    if((ftp->v == HUGE_VAL || ftp->v == -HUGE_VAL) && errno == ERANGE)
    {
	    string msg = "floating-point constant `";
	    msg += yytext;
	    msg += "' too large (overflow)";
	    g_parse->error(msg);
    }
    else if(ftp->v == 0 && errno == ERANGE)
    {
	    string msg = "floating-point constant `";
	    msg += yytext;
	    msg += "' too small (underflow)";
	    g_parse->error(msg);
    }
    return JCE_FLOATING_POINT_LITERAL;
}

[[:space:]]     {
    if(yytext[0] == '\n')
    {
        g_parse->nextLine();
    }
}

{anychar}       {
    if(yytext[0] < 32 || yytext[0] > 126)
    {
        stringstream s;
        s << "illegal input character: '\\";
        s.width(3);
        s.fill('0');
        s << oct << static_cast<int>(static_cast<unsigned char>(yytext[0]));
        s << "'";
        g_parse->error(s.str());
        return BAD_CHAR;
    }
    return yytext[0];
}

%%

