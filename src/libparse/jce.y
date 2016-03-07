%{
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

#include "parse/parse.h"
#define YYDEBUG 1
#define YYINITDEPTH 10000
%}

%defines
%debug

//keyword token
%token JCE_VOID
%token JCE_STRUCT
%token JCE_BOOL
%token JCE_BYTE
%token JCE_SHORT
%token JCE_INT
%token JCE_DOUBLE
%token JCE_FLOAT
%token JCE_LONG
%token JCE_STRING
%token JCE_VECTOR
%token JCE_MAP
%token JCE_NAMESPACE
%token JCE_INTERFACE
%token JCE_IDENTIFIER
%token JCE_OUT
%token JCE_OP
%token JCE_KEY
%token JCE_ROUTE_KEY
%token JCE_REQUIRE
%token JCE_OPTIONAL
%token JCE_INTEGER_LITERAL
%token JCE_FLOATING_POINT_LITERAL
%token JCE_FALSE
%token JCE_TRUE
%token JCE_STRING_LITERAL
%token JCE_SCOPE_DELIMITER
%token JCE_CONST
%token JCE_ENUM
%token JCE_UNSIGNED
%token BAD_CHAR

%%
start: definitions
;

// ----------------------------------------------------------------------
definitions
// ----------------------------------------------------------------------
: definition
{
}
';' definitions
| error ';'
{
    yyerrok;
}
definitions
| definition
{
    g_parse->error("`;' missing after definition");
}
|
{
}
;

// ----------------------------------------------------------------------
definition
// ----------------------------------------------------------------------
: namespace_def
{
    assert($1 == 0 || NamespacePtr::dynamicCast($1));
}
| interface_def
{
    assert($1 == 0 || InterfacePtr::dynamicCast($1));
}
| struct_def
{
    assert($1 == 0 || StructPtr::dynamicCast($1));
}
| key_def
{
}
| enum_def
{
    assert($1 == 0 || EnumPtr::dynamicCast($1));
}
| const_def
{
    assert($1 == 0 || ConstPtr::dynamicCast($1));
}
;

// ----------------------------------------------------------------------
enum_def
// ----------------------------------------------------------------------
: enum_id
{
    $$ = $1;
}
'{' enumerator_list '}'
{
    if($3)
    {
        g_parse->popContainer();
        $$ = $3;
    }
    else
    {
        $$ = 0;
    }

    $$ = $2;
}
;

// ----------------------------------------------------------------------
enum_id
// ----------------------------------------------------------------------
: JCE_ENUM JCE_IDENTIFIER
{
    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!c)
    {
        g_parse->error("enum must define in namespace");
    }
    StringTokPtr ident  = StringTokPtr::dynamicCast($2);
    EnumPtr e = c->createEnum(ident->v);
    g_parse->pushContainer(e);

    $$ = e;
}
| JCE_ENUM keyword
{
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumeration name");
    $$ = $2;
}
;

// ----------------------------------------------------------------------
enumerator_list
// ----------------------------------------------------------------------
: enumerator ',' enumerator_list
{
    $$ = $2;
}
| enumerator
{
}
;

// ----------------------------------------------------------------------
enumerator
// ----------------------------------------------------------------------
: JCE_IDENTIFIER
{
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringTokPtr ident  = StringTokPtr::dynamicCast($1);
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    tPtr->disableDefault();
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    $$ = e;
}
| keyword
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumerator");
}
| JCE_IDENTIFIER  '=' const_initializer 
{
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringTokPtr ident  = StringTokPtr::dynamicCast($1);
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    ConstTokPtr sPtr    = ConstTokPtr::dynamicCast($3);
    g_parse->checkConstValue(tPtr, sPtr->t);
    tPtr->setDefault(sPtr->v);
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    $$ = e;
}
| 
{
}
;

// ----------------------------------------------------------------------
namespace_def
// ----------------------------------------------------------------------
: JCE_NAMESPACE JCE_IDENTIFIER
{
    StringTokPtr ident  = StringTokPtr::dynamicCast($2);
    ContainerPtr c      = g_parse->currentContainer();
    NamespacePtr n      = c->createNamespace(ident->v);
    if(n)
    {
        g_parse->pushContainer(n);
        $$ = GrammarBasePtr::dynamicCast(n);
    }
    else
    {
        $$ = 0;
    }
}
'{' definitions '}'
{
    if($3)
    {
        g_parse->popContainer();
        $$ = $3;
    }
    else
    {
        $$ = 0;
    }
}
;


//结构key------------------------------------------------------
key_def
// ----------------------------------------------------------------------
: JCE_KEY '[' scoped_name ','
{
    StringTokPtr ident = StringTokPtr::dynamicCast($3);
    StructPtr sp = StructPtr::dynamicCast(g_parse->findUserType(ident->v));
    if(!sp)
    {
        g_parse->error("struct '" + ident->v + "' undefined!");
    }

    g_parse->setKeyStruct(sp);
}
key_members ']'
{
}
;

// 结构key------------------------------------------------------
key_members
// ----------------------------------------------------------------------
: JCE_IDENTIFIER
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        $$ = 0;
    }
}
| key_members ',' JCE_IDENTIFIER
{
    StringTokPtr ident = StringTokPtr::dynamicCast($3);
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        $$ = 0;
    }   
}
;


// ----------------------------------------------------------------------
interface_def
// ----------------------------------------------------------------------
: interface_id
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);

    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());

    InterfacePtr cl = c->createInterface(ident->v);
    if(cl)
    {
        g_parse->pushContainer(cl);
        $$ = GrammarBasePtr::dynamicCast(cl);
    }
    else
    {
        $$ = 0;
    }
}
'{' interface_exports '}'
{
    if($2)
    {
       g_parse->popContainer();
       $$ = GrammarBasePtr::dynamicCast($2);
    }
    else
    {
       $$ = 0;
    }
}
;

// ----------------------------------------------------------------------
interface_id
// ----------------------------------------------------------------------
: JCE_INTERFACE JCE_IDENTIFIER
{
    $$ = $2;
}
| JCE_INTERFACE keyword
{
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    g_parse->error("keyword `" + ident->v + "' cannot be used as interface name");
    $$ = $2;
}
;

// ----------------------------------------------------------------------
interface_exports
// ----------------------------------------------------------------------
: interface_export ';' interface_exports
{
}
| error ';' interface_exports
{
}
| interface_export
{
    g_parse->error("`;' missing after definition");
}
|
{
}
;

// ----------------------------------------------------------------------
interface_export
// ----------------------------------------------------------------------
: operation
;

// ----------------------------------------------------------------------
operation
// ----------------------------------------------------------------------
: operation_preamble parameters ')'
{
    if($1)
    {
        g_parse->popContainer();
        $$ = GrammarBasePtr::dynamicCast($1);
    }
    else
    {
        $$ = 0;
    }
}
;

// ----------------------------------------------------------------------
operation_preamble
// ----------------------------------------------------------------------
: return_type JCE_OP
{
    TypePtr returnType = TypePtr::dynamicCast($1);
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    string name        = ident->v;
    InterfacePtr cl    = InterfacePtr::dynamicCast(g_parse->currentContainer());
    if(cl)
    {
         OperationPtr op = cl->createOperation(name, returnType);
         if(op)
         {
             g_parse->pushContainer(op);
             $$ = GrammarBasePtr::dynamicCast(op);
         }
         else
         {
             $$ = 0;
         }
    }
    else
    {
        $$ = 0;
    }
}
;

// ----------------------------------------------------------------------
return_type
// ----------------------------------------------------------------------
: type
| JCE_VOID
{
    $$ = 0;
}
;


// ----------------------------------------------------------------------
parameters
// ----------------------------------------------------------------------
: // empty
{
}
| type_id
{
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast($1);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
| parameters ',' type_id
{
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast($3);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
| out_qualifier type_id
{
    BoolTokPtr isOutParam  = BoolTokPtr::dynamicCast($1);
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast($2);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
| parameters ',' out_qualifier type_id
{
    BoolTokPtr isOutParam  = BoolTokPtr::dynamicCast($3);
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast($4);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
| routekey_qualifier type_id
{
    BoolTokPtr isRouteKeyParam  = BoolTokPtr::dynamicCast($1);
    TypeIdPtr  tsp              = TypeIdPtr::dynamicCast($2);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
| parameters ',' routekey_qualifier type_id
{
    BoolTokPtr isRouteKeyParam = BoolTokPtr::dynamicCast($3);
    TypeIdPtr  tsp             = TypeIdPtr::dynamicCast($4);

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
| out_qualifier 
{
    g_parse->error("'out' must be defined with a type");
}
| routekey_qualifier 
{
    g_parse->error("'routekey' must be defined with a type");
}
;

// ----------------------------------------------------------------------
routekey_qualifier
// ----------------------------------------------------------------------
: JCE_ROUTE_KEY
{
    BoolTokPtr routekey = new BoolTok;
    routekey->v = true;
    $$ = GrammarBasePtr::dynamicCast(routekey);
}
;

// ----------------------------------------------------------------------
out_qualifier
// ----------------------------------------------------------------------
: JCE_OUT
{
    BoolTokPtr out = new BoolTok;
    out->v = true;
    $$ = GrammarBasePtr::dynamicCast(out);
}
;

// 结构定义--------------------------------------------------------------
struct_def
// ----------------------------------------------------------------------
: struct_id
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
         StructPtr sp = np->createStruct(ident->v);
         if(sp)
         {
             g_parse->pushContainer(sp);
             $$ = GrammarBasePtr::dynamicCast(sp);
         }
         else
         {
             $$ = 0;
         }
    }
    else
    {
       g_parse->error("struct '" + ident->v + "' must definition in namespace");
    }
}
'{' struct_exports '}'
{
    if($2)
    {
        g_parse->popContainer();
    }
    $$ = $2;

    //不能有空结构
    StructPtr st = StructPtr::dynamicCast($$);
    assert(st);
    if(st->getAllMemberPtr().size() == 0)
    {
        g_parse->error("struct `" + st->getSid() + "' must have at least one member");
    }
}
;

// 结构名称定义----------------------------------------------------------
struct_id
// ----------------------------------------------------------------------
: JCE_STRUCT JCE_IDENTIFIER
{
    $$ = $2;
}
| JCE_STRUCT keyword
{
    StringTokPtr ident = StringTokPtr::dynamicCast($2);

    g_parse->error("keyword `" + ident->v + "' cannot be used as struct name");
}
| JCE_STRUCT error
{
    g_parse->error("abstract declarator '<anonymous struct>' used as declaration");
}
;

// 结构成员变量部分------------------------------------------------------
struct_exports
// ----------------------------------------------------------------------
: data_member ';' struct_exports
{

}
| data_member
{
   g_parse->error("';' missing after definition");
}
|
{
}
;

// 
// 
// 数据成员--------------------------------------------------------------
data_member
// ----------------------------------------------------------------------
: struct_type_id
{
    $$ = GrammarBasePtr::dynamicCast($1);
}
;

// 结构的数据成员--------------------------------------------------------------
struct_type_id
// ----------------------------------------------------------------------
: JCE_INTEGER_LITERAL JCE_REQUIRE type_id
{
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast($1);
        g_parse->checkTag(iPtr->v);

        TypeIdPtr tPtr  = TypeIdPtr::dynamicCast($3);
        tPtr->setRequire(iPtr->v);
        np->addTypeId(tPtr);
        $$ = GrammarBasePtr::dynamicCast($3);
    }
    else
    {
        $$ = 0;
    }
}
| JCE_INTEGER_LITERAL JCE_REQUIRE type_id '=' const_initializer
{
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast($1);
        g_parse->checkTag(iPtr->v);

        //判断类型和数值类型是否一致
        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast($3);
        ConstTokPtr sPtr = ConstTokPtr::dynamicCast($5);
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setRequire(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        $$ = GrammarBasePtr::dynamicCast($3);
    }
    else
    {
        $$ = 0;
    }
}
| JCE_INTEGER_LITERAL JCE_OPTIONAL type_id '=' const_initializer
{
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast($1);
        g_parse->checkTag(iPtr->v);

        //判断类型和数值类型是否一致
        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast($3);
        ConstTokPtr sPtr = ConstTokPtr::dynamicCast($5);
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setOptional(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        $$ = GrammarBasePtr::dynamicCast($3);
    }
    else
    {
        $$ = 0;
    }
}
| JCE_INTEGER_LITERAL JCE_OPTIONAL type_id
{
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntegerTokPtr iPtr = IntegerTokPtr::dynamicCast($1);
        g_parse->checkTag(iPtr->v);
        TypeIdPtr tPtr = TypeIdPtr::dynamicCast($3);
        tPtr->setOptional(iPtr->v);
        np->addTypeId(tPtr);
        $$ = GrammarBasePtr::dynamicCast($3);
    }
    else
    {
        $$ = 0;
    }
}
| JCE_REQUIRE type_id
{
    g_parse->error("struct member need 'tag'");
}
| JCE_OPTIONAL type_id
{
    g_parse->error("struct member need 'tag'");
}
| JCE_INTEGER_LITERAL type_id
{
    g_parse->error("struct member need 'require' or 'optional'");
}
| type_id
{
    g_parse->error("struct member need 'tag' or 'require' or 'optional'");
}
;

// ----------------------------------------------------------------------
const_initializer
// ----------------------------------------------------------------------
: JCE_INTEGER_LITERAL
{
    IntegerTokPtr intVal = IntegerTokPtr::dynamicCast($1);
    ostringstream sstr;
    sstr << intVal->v;
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::VALUE;
    c->v = sstr.str();
    $$ = c;
}
| JCE_FLOATING_POINT_LITERAL
{
    FloatTokPtr floatVal = FloatTokPtr::dynamicCast($1);
    ostringstream sstr;
    sstr << floatVal->v;
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::VALUE;
    c->v = sstr.str();
    $$ = c;
}
| JCE_STRING_LITERAL
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::STRING;
    c->v = ident->v;
    $$ = c;
}
| JCE_FALSE
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::BOOL;
    c->v = ident->v;
    $$ = c;
}
| JCE_TRUE
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    ConstTokPtr c = new ConstTok();
    c->t = ConstTok::BOOL;
    c->v = ident->v;
    $$ = c;
}
;

// 常量定义--------------------------------------------------------------
const_def
// ----------------------------------------------------------------------
: JCE_CONST type_id '=' const_initializer
{
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!np)
    {
        g_parse->error("const type must define in namespace");
    }

    TypeIdPtr t   = TypeIdPtr::dynamicCast($2);
    ConstTokPtr c = ConstTokPtr::dynamicCast($4);
    ConstPtr cPtr = np->createConst(t, c);
    $$ = cPtr;
}
;

// 变量定义--------------------------------------------------------------
type_id
// ----------------------------------------------------------------------
: type JCE_IDENTIFIER
{
    TypePtr type = TypePtr::dynamicCast($1);
    StringTokPtr ident = StringTokPtr::dynamicCast($2);

    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);

    $$ = GrammarBasePtr::dynamicCast(typeIdPtr);
}
|type JCE_IDENTIFIER  '[' JCE_INTEGER_LITERAL ']'
{
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast($1));
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast($4);
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    $$ = GrammarBasePtr::dynamicCast(typeIdPtr);
}
|type '*' JCE_IDENTIFIER  
{
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast($1));
    //IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast($4);
    g_parse->checkPointerVaid(type);
    type->setPointer(true);
    StringTokPtr ident = StringTokPtr::dynamicCast($3);
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    $$ = GrammarBasePtr::dynamicCast(typeIdPtr);
}
|type JCE_IDENTIFIER  ':' JCE_INTEGER_LITERAL 
{
    TypePtr type = TypePtr::dynamicCast($1);
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast($3);
    g_parse->checkArrayVaid(type,iPtrSize->v);
    $$ = GrammarBasePtr::dynamicCast(typeIdPtr);
}
| type keyword
{
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    g_parse->error("keyword `" + ident->v + "' cannot be used as data member name");
}
| type
{
    g_parse->error("missing data member name");
}
| error
{
    g_parse->error("unkown type");
}
;

// 类型------------------------------------------------------------------
type
// ----------------------------------------------------------------------
: type_no ':' JCE_INTEGER_LITERAL 
{

    TypePtr type = TypePtr::dynamicCast($1);
    IntegerTokPtr iPtrSize = IntegerTokPtr::dynamicCast($3);
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    $$ = type;
}
| type_no
{
    $$ = $1;
}
| type_no ':' error
{
   g_parse->error("array missing size");
}
;

// 没有默认大小的类型------------------------------------------------------------------
type_no
// ----------------------------------------------------------------------
: JCE_BOOL
{
    $$ = g_parse->createBuiltin(Builtin::KindBool);
}
| JCE_BYTE
{
    $$ = g_parse->createBuiltin(Builtin::KindByte);
}
| JCE_UNSIGNED JCE_BYTE //unsigned char 对应 short
{
    $$ = g_parse->createBuiltin(Builtin::KindShort,true);
}
| JCE_SHORT
{
    $$ = g_parse->createBuiltin(Builtin::KindShort);
}
| JCE_UNSIGNED JCE_SHORT
{
    $$ = g_parse->createBuiltin(Builtin::KindInt,true);
}
| JCE_INT
{
    $$ = g_parse->createBuiltin(Builtin::KindInt);
}
| JCE_UNSIGNED JCE_INT
{
    $$ = g_parse->createBuiltin(Builtin::KindLong,true);
}
| JCE_LONG
{
    $$ = g_parse->createBuiltin(Builtin::KindLong);
}
| JCE_FLOAT
{
    $$ = g_parse->createBuiltin(Builtin::KindFloat);
}
| JCE_DOUBLE
{
    $$ = g_parse->createBuiltin(Builtin::KindDouble);
}
| JCE_STRING
{
    $$ = g_parse->createBuiltin(Builtin::KindString);
}
| vector
{
   $$ = GrammarBasePtr::dynamicCast($1);
}
| map
{
   $$ = GrammarBasePtr::dynamicCast($1);
}
| scoped_name
{
    StringTokPtr ident = StringTokPtr::dynamicCast($1);
    TypePtr sp = g_parse->findUserType(ident->v);
    if(sp)
    {
        $$ = GrammarBasePtr::dynamicCast(sp);
    }
    else
    {
        g_parse->error("'" + ident->v + "' undefined!");
    }
}
;

// 关键字----------------------------------------------------------------
vector
// ----------------------------------------------------------------------
: JCE_VECTOR '<' type '>'
{
   $$ = GrammarBasePtr::dynamicCast(g_parse->createVector(TypePtr::dynamicCast($3)));
}
| JCE_VECTOR '<' error
{
   g_parse->error("vector error");
}
| JCE_VECTOR '<' type error
{
   g_parse->error("vector missing '>'");
}
| JCE_VECTOR error
{
   g_parse->error("vector missing type");
}
;

// 关键字----------------------------------------------------------------
map
// ----------------------------------------------------------------------
: JCE_MAP '<' type ',' type '>'
{
   $$ = GrammarBasePtr::dynamicCast(g_parse->createMap(TypePtr::dynamicCast($3), TypePtr::dynamicCast($5)));
}
| JCE_MAP '<' error
{
   g_parse->error("map error");
}
;

// ----------------------------------------------------------------------
scoped_name
// ----------------------------------------------------------------------
: JCE_IDENTIFIER
{
}
| JCE_SCOPE_DELIMITER JCE_IDENTIFIER
{
    StringTokPtr ident = StringTokPtr::dynamicCast($2);
    ident->v = "::" + ident->v;
    $$ = GrammarBasePtr::dynamicCast(ident);
}
| scoped_name JCE_SCOPE_DELIMITER JCE_IDENTIFIER
{
    StringTokPtr scoped = StringTokPtr::dynamicCast($1);
    StringTokPtr ident  = StringTokPtr::dynamicCast($3);
    scoped->v += "::";
    scoped->v += ident->v;
    $$ = GrammarBasePtr::dynamicCast(scoped);
}
;

// 关键字----------------------------------------------------------------
keyword
// ----------------------------------------------------------------------
: JCE_STRUCT
{
}
| JCE_VOID
{
}
| JCE_BOOL
{
}
| JCE_BYTE
{
}
| JCE_SHORT
{
}
| JCE_INT
{
}
| JCE_FLOAT
{
}
| JCE_DOUBLE
{
}
| JCE_STRING
{
}
| JCE_VECTOR
{
}
| JCE_KEY
{
}
| JCE_MAP
{
}
| JCE_NAMESPACE
{
}
| JCE_INTERFACE
{
}
| JCE_OUT
{
}
| JCE_REQUIRE
{
}
| JCE_OPTIONAL
{
}
| JCE_INTEGER_LITERAL
{
}
| JCE_FLOATING_POINT_LITERAL
{
}
| JCE_FALSE
{
}
| JCE_TRUE
{
}
| JCE_STRING_LITERAL
{
}
| JCE_CONST
{
}
| JCE_ENUM
{
}
| JCE_UNSIGNED
{
}
;

%%


