#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Jce_c.h"

//返回值定义
const Int32 JCE_SUCCESS        =  0;	  //成功
const Int32 JCE_ATTR_NOT_FOUND = -1;	  //查找不到相关属性
const Int32 JCE_ENCODE_ERROR   = -2;	  //编码错误
const Int32 JCE_DECODE_ERROR   = -3;	  //解码错误
const Int32 JCE_RUNTIME_ERROR  = -4;	  //其他运行时错误
const Int32 JCE_MALLOC_ERROR   = -5;	  //内存申请失败错误
const Int32 JCE_DECODE_EOPNEXT = -6;	  //可选字段不存在

Int64 jce__bswap_constant_64(Int64 x)
{
#ifdef __MTK_64 
	Int64 __t__;
	__t__.high = jce__bswap_constant_32(x.high);
	__t__.low  = jce__bswap_constant_32(x.low);
	return __t__;
#else
	return((((x) & 0xff00000000000000ull) >> 56)                      \
		   | (((x) & 0x00ff000000000000ull) >> 40)                     \
		   | (((x) & 0x0000ff0000000000ull) >> 24)                     \
		   | (((x) & 0x000000ff00000000ull) >> 8)                      \
		   | (((x) & 0x00000000ff000000ull) << 8)                      \
		   | (((x) & 0x0000000000ff0000ull) << 24)                     \
		   | (((x) & 0x000000000000ff00ull) << 40)                     \
		   | (((x) & 0x00000000000000ffull) << 56));
#endif
}

Int64 jce_ntohll(Int64 x)
{
	return jce__bswap_constant_64(x);
}

Float jce_ntohf(Float x)
{
	Int32 __t__ = jce__bswap_constant_32((*((Int32 *)&x)));
	return *((Float *) &__t__);
}

Double jce_ntohd(Double x)
{
	Int64 __t__ = jce__bswap_constant_64((*((Int64 *)&x)));
	return *((Double *) &__t__);
}
/////////////////////////////////////////////////////////////////////////////////////

void JString_del(JString **st)
{
	if (*st != NULL && (*st)->_data) JceFree((*st)->_data);
	if (*st != NULL) JceFree(*st);
	*st = NULL;
}

void JString_copy(char * dest, const char * src, uint32_t len)
{
	char * pe = dest + len;
	for (; dest != pe; ++dest, ++src)
		*dest = *src;
}

Int32 JString_copyChar(JString * s, char * data, uint32_t len)
{
	char * p = JceMalloc(sizeof(char) * len + 1);
	if (!p)
		return JCE_MALLOC_ERROR;

	JceFree(s->_data);

	s->_data = p;
	s->_len = len;
	s->_buf_len = len + 1;
	JString_copy(s->_data, data, len);

	return 0;
}

void JString_clear(JString *s)          { s->_len = 0; s->_data[0] = 0;}

Int32 JString_reserve(JString *s, uint32_t n)
{
	if (s->_buf_len < n + 1)
	{
		char * p = JceMalloc(n + 1);

		if (!p)
			return JCE_MALLOC_ERROR;

		JString_copy(p, s->_data, s->_len + 1);
		JceFree(s->_data);
		s->_data = p;
		s->_buf_len = n + 1;
	}
	return 0;
}

Int32 JString_assign(JString *s, const char * data, uint32_t len)
{
	Int32 ret=0;
	JString_clear(s);

	ret = JString_reserve(s, len);
	if (ret)
		return ret;

	JString_copy(s->_data, data, len);
	s->_len = len;
	s->_data[s->_len] = 0;

	return 0;
}

Int32 JString_append(JString * s, const char * data, uint32_t len)
{
	if (s->_len + len + 1 > s->_buf_len)
	{
		Int32 ret = JString_reserve(s, 2*(s->_len + len));
		if (ret)
			return ret;
	}

	JString_copy(s->_data + s->_len, data, len);
	s->_len += len;
	s->_data[s->_len] = 0;

	return JCE_SUCCESS;
}

Int32 JString_insert(JString *s, uint32_t pos, char v)
{
	if (s->_len >= s->_buf_len)
	{
		Int32 ret = JString_reserve(s, s->_len * 2);
		if (ret)
			return ret;
	}
	JString_copy(s->_data + pos + 1, s->_data + pos, s->_len - pos);
	s->_data[pos] = v;
	++(s->_len);

	return JCE_SUCCESS;
}

uint32_t JString_size(JString *s)       { return s->_len;}
int JString_empty(JString *s)           { return s->_len == 0;}
uint32_t JString_capacity(JString *s)   { return s->_buf_len;}
char * JString_data(JString *s)         { return s->_data;}
Int32 JString_resize(JString *s, uint32_t n)
{
	if (n > JString_size(s))
	{
		Int32 ret = JString_reserve(s, n);
		return ret;
	}
	return 0;
}

Int32 JString_init(JString *s)
{
	s->_data = JceMalloc(1);
	if (!s->_data)
		return JCE_MALLOC_ERROR;

	s->_data[0] = 0;
	s->_len  = 0;
	s->_buf_len = 1;

	return JCE_SUCCESS;
}

JString * JString_new()
{
	Int32 ret;
	JString *s = JceMalloc(sizeof(JString));
	if (!s)
		return NULL;

	ret = JString_init(s);
	if (ret)
	{
		JceFree(s);
		return NULL;
	}

	return s;
}

void JArray_del(JArray ** arr)
{
	if ((*arr)->elem_type_name != NULL)	JceFree((*arr)->elem_type_name);
	if ((*arr)->list           != NULL)	JceFree((*arr)->list);
	if ((*arr)->buff           != NULL)	JceFree((*arr)->buff);

	JceFree(*arr);
	*arr = NULL;
}

Int32 JArray_reserveList(JArray *arr, uint32_t num)
{
	if (arr->list_len < num)
	{
		int * p = JceMalloc(num);
		if (!p)
			return JCE_MALLOC_ERROR;

		memcpy(p, arr->list, arr->list_len);
		JceFree(arr->list);
		arr->list = p;
		arr->list_len = num;
	}

	return 0;
}

Int32 JArray_reserveBuff(JArray *arr, uint32_t len)
{
	if (arr->buff_len < len)
	{
		char * p = JceMalloc(len);
		if (!p)
			return JCE_MALLOC_ERROR;

		memcpy(p, arr->buff, arr->buff_len);
		JceFree(arr->buff);
		arr->buff = p;
		arr->buff_len = len;
	}
	return 0;
}

Int32 JArray_pushBack(JArray *arr, const char * data, uint32_t len)
{
	Int32 ret;
	++ arr->elem_num;

	if (arr->list_len <= arr->elem_num * sizeof(int))
	{
		ret = JArray_reserveList(arr, 2* (arr->list_len + sizeof(int)) );
		if (ret)
			return ret;
	}
	if (arr->buff_len < arr->buff_used + len)
	{
		ret = JArray_reserveBuff(arr, 2*(arr->buff_len + len));
		if (ret)
			return ret;
	}

	arr->list[arr->elem_num - 1] = (int)(arr->buff_used);
	memcpy(arr->buff + arr->buff_used, data, len);
	arr->buff_used += len;

	arr->list[arr->elem_num] = (int)(arr->buff_used);

	return 0;
}

Int32 JArray_pushBackString(JArray *arr, const char * data)
{
	return JArray_pushBack(arr, data, strlen(data));
}

Int32 JArray_get(JArray *arr, unsigned index, char *data, uint32_t * len)
{
	unsigned data_len = 0;
	if (index > arr->elem_num - 1)
	{
		*len = 0;
		return -1;
	}

	data_len = arr->list[index + 1] - arr->list[index];
	if (*len < data_len)
	{
		*len = 0;
		return -2;
	}
	*len = data_len;
	memcpy(data, arr->buff + arr->list[index], *len);

	return 0;
}

char *JArray_getPtr(JArray *arr, unsigned index)
{
	if (index > arr->elem_num - 1)
	{
		return NULL;
	}

	return arr->buff + arr->list[index];
}


int JArray_getLength(JArray *arr, unsigned index)
{
	if (index > arr->elem_num - 1)
	{
		return 0;
	}

	return arr->list[index + 1] - arr->list[index];
}


int JArray_size(JArray *arr)
{
	return arr->elem_num;
}

int JArray_empty(JArray *arr)
{
	return JArray_size(arr) == 0;
}

void JArray_clear(JArray *arr)
{
	arr->elem_num = 0;
	arr->buff_used = 0;
}

void JArray_init(JArray *arr)
{
/*
	arr->elem_type_name = NULL;
	arr->elem_num       = 0;
	arr->list_len       = 0;
	arr->list           = NULL;
	arr->buff_used      = 0;
	arr->buff_len       = 0;
	arr->buff           = NULL;
  */ 
	memset(arr, 0, sizeof(JArray));
}

JArray * JArray_new(const char * type)
{
	unsigned len = 0;
	JArray * arr = JceMalloc(sizeof(JArray));
	if (!arr)
		return NULL;

	JArray_init(arr);

	len = strlen(type);
	arr->elem_type_name = JceMalloc(len+1);
	if (!arr->elem_type_name)
	{
		JceFree(arr);
		return NULL;
	}

	memcpy(arr->elem_type_name, type, len+1);

	return arr;
}

void JMapWrapper_del(JMapWrapper ** m)
{
	JArray_del(&((*m)->first));
	JArray_del(&((*m)->second));

	JceFree(*m);
	*m = NULL;
}

Int32 JMapWrapper_put(JMapWrapper *m, const char * first_data, unsigned first_len, const char *value_data, unsigned value_len)
{
	Int32 ret = 0;
	ret = JArray_pushBack(m->first, first_data, first_len);
	if (ret)
		return ret;

	return JArray_pushBack(m->second, value_data, value_len);
}

int JMapWrapper_find(JMapWrapper *m, const char * first_data, unsigned first_len, char **value_data, unsigned * value_len)
{
	uint32_t i;

	for (i = 0; i< JMapWrapper_size(m); ++i)
	{
		if (first_len == JArray_getLength(m->first, i) 
			&& memcmp(JArray_getPtr(m->first, i), first_data, first_len) == 0)
		{
			*value_len = JArray_getLength(m->second, i);
			*value_data =  JArray_getPtr(m->second, i);
			return 0;
		}
	}

	return JCE_ATTR_NOT_FOUND;
}


int JMapWrapper_size(JMapWrapper * m)
{
	return JArray_size(m->first);
}

int JMapWrapper_getByIndex(JMapWrapper *m, unsigned index, char *first, uint32_t * first_len, char* second, uint32_t * second_len)
{
	JArray_get(m->first, index, first, first_len);
	JArray_get(m->second, index, second, second_len);
	return 0;
}

int JMapWrapper_empty(JMapWrapper *m)
{
	return JArray_empty(m->first) && JArray_empty(m->second);
}

void JMapWrapper_clear(JMapWrapper *m)
{
	JArray_clear(m->first);
	JArray_clear(m->second);
}

void JMapWrapper_init(JMapWrapper *m)
{
}

JMapWrapper * JMapWrapper_new(const char * first_type, const char * second_type)
{
	JMapWrapper * m = JceMalloc(sizeof(JMapWrapper));
	if (!m)
		return NULL;

	JMapWrapper_init(m);

	m->first = JArray_new(first_type);
	if (!m->first)
	{
		JceFree(m);
		return NULL;
	}
	m->second = JArray_new(second_type);
	if (!m->second)
	{
		JArray_del(&m->first);
		JceFree(m);
		return NULL;
	}

	return m;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t helper_getTag(helper *h)
{
	return h->type_tag >> 4;
}

void helper_setTag(helper *h, unsigned int t)
{
	t <<= 4;
	h->type_tag &= 0x0F;
	h->type_tag |= t;
}

uint8_t helper_getType(helper *h)
{
	return h->type_tag & 0x0F;
}

void helper_setType(helper *h, unsigned int t)
{
	h->type_tag &= 0xF0;
	h->type_tag |= (t & 0x0F);
}

void DataHead_del(DataHead ** head)
{
	JceFree(*head);
	*head = NULL;
}


uint8_t DataHead_getTag(DataHead * head)              { return head->_tag;}
//void DataHead_setTag(DataHead * head, uint8_t t)      { head->_tag = t; }
uint8_t DataHead_getType(DataHead * head)             { return head->_type;}
//void DataHead_setType(DataHead * head, uint8_t t)     { head->_type = t; }

/// 读取头信息，但不前移流的偏移量
Int32 DataHead_peekFrom(DataHead * head, JceInputStream* is, uint32_t *n)
{
	Int32 ret = 0;
	helper h;
	*n = sizeof(h);
	ret = JceInputStream_peekBuf(is, &h, sizeof(h), 0);
	if (JCE_SUCCESS != ret)	return ret;

	head->_type = helper_getType(&h);
	if (helper_getTag(&h) == 15)
	{
		ret = JceInputStream_peekBuf(is, &head->_tag, sizeof(head->_tag), sizeof(h));
		if (JCE_SUCCESS != ret)	return ret;

		*n += sizeof(head->_tag);
	}
	else
	{
		head->_tag = helper_getTag(&h);
	}
	return JCE_SUCCESS;
}

/// 读取数据头信息
Int32 DataHead_readFrom(DataHead * head, JceInputStream* is)
{
	Int32 ret = 0;
	uint32_t n;

	ret = DataHead_peekFrom(head, is, &n);
	if (JCE_SUCCESS != ret)	return ret;

	ret = JceInputStream_skip(is, n);
	return ret;
}


/// 写入数据头信息
Int32 DataHead_writeTo(DataHead * head, JceOutputStream* os)
{
	Int32 ret;
	helper h;
	helper_setType(&h, head->_type);
	if (head->_tag < 15)
	{
		helper_setTag(&h, head->_tag);
		return JceOutputStream_writeBuf(os, &h, sizeof(h));
	}
	else
	{
		helper_setTag(&h, 15);
		ret = JceOutputStream_writeBuf(os, &h, sizeof(h));
		if (JCE_SUCCESS != ret)	return ret;
		return JceOutputStream_writeBuf(os, &head->_tag, sizeof(head->_tag));
	}
	return JCE_SUCCESS;
}
Int32 DataHead_setAndWriteTo(DataHead * head, unsigned int type, unsigned int tag,JceOutputStream* os)
{
	head->_tag = tag;
	head->_type = type;
	return DataHead_writeTo(head, os);
}


void DataHead_init(DataHead * head)
{
	head->_type = 0;
	head->_tag  = 0;
}

DataHead * DataHead_new()
{
	DataHead * head = JceMalloc(sizeof(DataHead));
	if (!head)
	{
		return NULL;
	}
	DataHead_init(head);
	return head;
}
////////////////////////////////////////////////////////////////////////////
static void JceStream_del(JceStream ** js)
{
	JString_del(& (*js)->_buf);
	DataHead_del(& (*js)->_h);

	JceFree(*js);
	*js = NULL;
}

static Int32 JceStream_init(JceStream* js)
{
	js->_buf = JString_new();
	if (!js->_buf)
		return JCE_MALLOC_ERROR;

	js->_h   = DataHead_new();
	if (!js->_h)
	{
		JString_del(&js->_buf);
		return JCE_MALLOC_ERROR;
	}

	js->_cur = 0;
	js->_err[0] = 0;

	return JCE_SUCCESS;
}

static JceStream * JceStream_new()
{
	Int32 ret;
	JceStream * js = JceMalloc(sizeof(JceStream));
	if (!js)
	{
		return NULL;
	}

	ret = JceStream_init(js);
	if (ret)
	{
		JceFree(js);
		return NULL;
	}

	return js;
}

////////////////////////////////////////////////////////////////////////////

void JceInputStream_del(JceInputStream ** is)
{
	JceStream_del(is);
}

void JceInputStream_reset(JceInputStream * is)
{
	is->_cur = 0;
}

/// 读取缓存
Int32 JceInputStream_readBuf(JceInputStream * is, void * buf, uint32_t len)
{
	Int32 ret;

	ret = JceInputStream_peekBuf(is, buf, len, 0);
	if (JCE_SUCCESS != ret)	return ret;

	is->_cur += len;
	return JCE_SUCCESS;
}

/// 读取缓存，但不改变偏移量
Int32 JceInputStream_peekBuf(JceInputStream * is, void * buf, uint32_t len, uint32_t offset)
{
	if (is->_cur + offset + len > JString_size(is->_buf))
	{
		snprintf(is->_err, sizeof(is->_err), "buffer overflow when peekBuf, over %u.", (uint32_t)JString_size(is->_buf));
		return JCE_DECODE_ERROR;
	}
	memcpy(buf, JString_data(is->_buf) + is->_cur + offset, len);
	return JCE_SUCCESS;
}



/// 跳过len个字节
Int32 JceInputStream_skip(JceInputStream * is, uint32_t len)
{
	is->_cur += len;
	return JCE_SUCCESS;
}

/// 设置缓存
Int32 JceInputStream_setBuffer(JceInputStream * is, const char * buf, uint32_t len)
{
	Int32 ret = JString_assign(is->_buf, buf, len);
	if (ret)
		return ret;

	is->_cur = 0;
	return JCE_SUCCESS;
}


/// 跳到指定标签的元素前
Int32 JceInputStream_skipToTag(JceInputStream * is, uint8_t tag)
{
	Int32 ret;
	while (1)
	{
		uint32_t len;
		ret = DataHead_peekFrom(is->_h, is, &len);
		if (JCE_SUCCESS != ret)	return ret;

		if (tag <= DataHead_getTag(is->_h) || DataHead_getType(is->_h) == eStructEnd)
		{
			if (tag != DataHead_getTag(is->_h))
			{
				snprintf(is->_err, sizeof(is->_err), "tag %d not found", DataHead_getTag(is->_h));
				return JCE_DECODE_ERROR;
			}
			return JCE_SUCCESS;
		}
		ret = JceInputStream_skip(is, len);
		if (JCE_SUCCESS != ret)	return ret;

		ret = JceInputStream_skipFieldByType(is, DataHead_getType(is->_h));
		if (JCE_SUCCESS != ret)	return ret;
	}
	snprintf(is->_err, sizeof(is->_err), "tag %d not found", DataHead_getTag(is->_h));
	return JCE_DECODE_ERROR;
}

/// 跳到当前结构的结束
Int32 JceInputStream_skipToStructEnd(JceInputStream * is)
{
	Int32 ret;
	Int32 level = 1; //判断结构嵌套的深度
	do
	{
		ret = DataHead_readFrom(is->_h, is);
		if (JCE_SUCCESS != ret)	return ret;

		if (DataHead_getType(is->_h) == eStructBegin)
			level++;
		else if (DataHead_getType(is->_h) == eStructEnd)
			level--;
		else
		{
			ret = JceInputStream_skipFieldByType(is, DataHead_getType(is->_h));
			if (JCE_SUCCESS != ret)	return ret;
		}
	}while (DataHead_getType(is->_h) != eStructEnd || level != 0);

	return JCE_SUCCESS;
}

/// 跳过一个字段
Int32 JceInputStream_skipField(JceInputStream * is)
{
	Int32 ret;

	ret = DataHead_readFrom(is->_h, is);
	if (JCE_SUCCESS != ret)	return ret;

	ret = JceInputStream_skipFieldByType(is, DataHead_getType(is->_h));
	if (JCE_SUCCESS != ret)	return ret;

	return JCE_SUCCESS;
}

/// 跳过一个字段，不包含头信息
Int32 JceInputStream_skipFieldByType(JceInputStream * is, uint8_t type)
{
	Int32 ret;
	uint32_t len = 0;

	switch (type)
	{
	case eChar:
		JceInputStream_skip(is, sizeof(Char));
		break;
	case eShort:
		JceInputStream_skip(is, sizeof(Short));
		break;
	case eInt32:
		JceInputStream_skip(is, sizeof(Int32));
		break;
	case eInt64:
		JceInputStream_skip(is, sizeof(Int64));
		break;
	case eFloat:
		JceInputStream_skip(is, sizeof(Float));
		break;
	case eDouble:
		JceInputStream_skip(is, sizeof(Double));
		break;
	case eString1:
		{
			uint8_t n;
			ret = JceInputStream_readBuf(is, &n, sizeof(n));
			if (JCE_SUCCESS != ret)	return ret;

			len = n;
			JceInputStream_skip(is, len);
		}
		break;
	case eString4:
		{
			uint32_t n;
			ret = JceInputStream_readBuf(is, &n, sizeof(n));
			if (JCE_SUCCESS != ret)	return ret;

			len = jce_ntohl(n);
			JceInputStream_skip(is, len);
		}
		break;
	case eMap:
		{
			Int32 size, i;
			ret = JceInputStream_readInt32(is, &size, 0, 1);
			if (JCE_SUCCESS != ret)	return ret;

			for (i = 0; i < size * 2; ++i)
			{
				ret = JceInputStream_skipField(is);
				if (JCE_SUCCESS != ret)	return ret;
			}
		}
		break;
	case eList:
		{
			Int32 size, i;
			ret = JceInputStream_readInt32(is, &size, 0, 1);
			if (JCE_SUCCESS != ret)	return ret;

			for (i = 0; i < size; ++i)
			{
				ret = JceInputStream_skipField(is);
				if (JCE_SUCCESS != ret)	return ret;
			}
		}
		break;
	case eSimpleList:
		{   
			Int32 size;

			ret = DataHead_readFrom(is->_h, is);
			if (JCE_SUCCESS != ret)	return ret;

			if (DataHead_getType(is->_h) != eChar)
			{

				snprintf(is->_err, sizeof(is->_err), "skipField with invalid type, type value: %d, %d.", type, DataHead_getType(is->_h));
				return JCE_DECODE_ERROR;
			}
			ret = JceInputStream_readInt32(is, &size, 0, 1);
			if (JCE_SUCCESS != ret)	return ret;

			ret = JceInputStream_skip(is, size);
			if (JCE_SUCCESS != ret)	return ret;
		}
		break;
	case eStructBegin:
		ret = JceInputStream_skipToStructEnd(is);
		if (JCE_SUCCESS != ret)	return ret;
		break;
	case eStructEnd:
	case eZeroTag:
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "skipField with invalid type, type value:%d.", type);
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_checkValid(JceInputStream * is,uint8_t tag, Bool isRequire)
{
	if (!JceInputStream_skipToTag(is, tag))
	{
		Int32 ret = DataHead_readFrom(is->_h, is);
		return ret;
	}
	else if (isRequire)
	{
		snprintf(is->_err, sizeof(is->_err), "require field not exist, tag: %d", tag);
		return JCE_DECODE_ERROR;
	}
	return JCE_DECODE_EOPNEXT;
}

Int32 JceInputStream_readByChar(JceInputStream * is, Char * n)
{
	Int32 ret;
	ret = JceInputStream_readBuf(is, n, sizeof(*n));
	return ret;
}
Int32 JceInputStream_readByShort(JceInputStream * is, Short * n)
{
	Int32 ret;
	ret = JceInputStream_readBuf(is, n, sizeof(*n));
	return ret;
}


Int32 JceInputStream_readByInt32(JceInputStream * is, Int32 * n)
{
	Int32 ret;
	ret = JceInputStream_readBuf(is, n, sizeof(*n));
	return ret;
}

Int32 JceInputStream_readBool(JceInputStream * is, Bool* b, uint8_t tag, Bool isRequire)
{
	Int32 ret;
	Char c = *b;
	ret = JceInputStream_readChar(is, &c, tag, isRequire);
	*b = c ? 1 : 0;

	return ret;
}

Int32 JceInputStream_readChar(JceInputStream * is, Char* c, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*c = 0;
		break;
	case eChar:
		ret = JceInputStream_readBuf(is, c, sizeof(*c));
		if (JCE_SUCCESS != ret)	return ret;

		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Char' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readUInt8	(JceInputStream * is, UInt8 * n, uint8_t tag, Bool isRequire)
{
	Short ns = (Short)*n;
	Int32 ret = JceInputStream_readShort(is, &ns, tag, isRequire);
	if (ret != JCE_SUCCESS) return ret;
	*n = (UInt8)ns;

	return JCE_SUCCESS;
}

Int32 JceInputStream_readShort(JceInputStream * is, Short* n, uint8_t tag, Bool isRequire)
{
	Char c;
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = 0;
		break;
	case eChar:
		ret = JceInputStream_readByChar(is, &c);
		if (JCE_SUCCESS != ret)	return ret;
		*n = c;

		break;
	case eShort:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohs(*n);
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Short' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readUInt16	(JceInputStream *is, UInt16 *n, uint8_t tag, Bool isRequire)
{
	Int32 ns = (Int32)*n;
	Int32 ret = JceInputStream_readInt32(is, &ns, tag, isRequire);
	if (ret != JCE_SUCCESS) return ret;
	*n = (UInt16)ns;

	return JCE_SUCCESS;
}

Int32 JceInputStream_readInt32(JceInputStream * is, Int32* n, uint8_t tag, Bool isRequire)
{
	Char c;
	Short sh;
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = 0;
		break;
	case eChar:
		ret = JceInputStream_readByChar(is, &c);
		if (JCE_SUCCESS != ret)	return ret;
		*n = c;

		break;
	case eShort:
		ret = JceInputStream_readByShort(is, &sh);
		if (JCE_SUCCESS != ret)	return ret;
		*n = (Short) jce_ntohs(sh);
		break;
	case eInt32:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohl(*n);
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Int32' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;

}

Int32 JceInputStream_readUInt32	(JceInputStream *is, UInt32 * n, uint8_t tag, Bool isRequire)
{
	Int64 ns = (Int64)*n;
	Int32 ret = JceInputStream_readInt64(is, &ns, tag, isRequire);
	if (ret != JCE_SUCCESS) return ret;
	*n = (UInt32)ns;

	return JCE_SUCCESS;
}

Int32 JceInputStream_readInt64(JceInputStream * is, Int64* n, uint8_t tag, Bool isRequire)
{

	Char c;
	Short sh;
	Int32 i32;
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;
#ifdef __MTK_64
	Int64 i64;
	i64.high = 0;
	i64.low = 0;
	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = i64;
		break;
	case eChar:
		ret = JceInputStream_readByChar(is, &c);
		if (JCE_SUCCESS != ret)	return ret;
		i64.low = c;
		*n = i64;
		break;
	case eShort:
		ret = JceInputStream_readByShort(is, &sh);
		if (JCE_SUCCESS != ret)	return ret;
		i64.low = (Short) jce_ntohs(sh);
		*n = i64;
		break;
	case eInt32:
		ret = (Int32) JceInputStream_readByInt32(is, &i32);
		if (JCE_SUCCESS != ret)	return ret;
		i64.low = (Int32) jce_ntohl(i32);
		*n = i64;
		break;
	case eInt64:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohll(*n);
		break;
#else

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = 0;
		break;
	case eChar:
		ret = JceInputStream_readByChar(is, &c);
		if (JCE_SUCCESS != ret)	return ret;
		*n = c;
		break;
	case eShort:
		ret = JceInputStream_readByShort(is, &sh);
		if (JCE_SUCCESS != ret)	return ret;
		*n = (Short) jce_ntohs(sh);
		break;
	case eInt32:
		ret = (Int32) JceInputStream_readByInt32(is, &i32);
		if (JCE_SUCCESS != ret)	return ret;
		*n = (Int32) jce_ntohl(i32);
		break;
	case eInt64:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohll(*n);
		break;
#endif
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Int64' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;

}
/*
Int32 JceInputStream_readInt64_HL(JceInputStream * is, Int32* high, Int32* low, uint8_t tag, Bool isRequire)
{
	Int64 n = 0;
	JceInputStream_readInt64(is, &n, tag,isRequire );
	*low     = (Int32)n;
	*high    = (Int32)(n>>32);
}
*/

Int32 JceInputStream_readFloat(JceInputStream * is, Float* n, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = 0;
		break;
	case eFloat:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohf(*n);
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Float' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;

}

Int32 JceInputStream_readDouble(JceInputStream * is, Double* n, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eZeroTag:
		*n = 0;
		break;
	case eFloat:
		{
			Float n1;
			ret = JceInputStream_readBuf(is, &n1, sizeof(n1));
			if (JCE_SUCCESS != ret)	return ret;

			*n = n1;
			break;
		}
	case eDouble:
		ret = JceInputStream_readBuf(is, n, sizeof(*n));
		if (JCE_SUCCESS != ret)	return ret;
		*n = jce_ntohd(*n);
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'Double' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readString(JceInputStream * is, JString* s, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eString1:
		{
			uint8_t n;
			uint32_t len;
			char ss[256];
			ret = JceInputStream_readBuf(is, &n, sizeof(n));
			if (JCE_SUCCESS != ret)	return ret;
			len = n;
			ret = JceInputStream_readBuf(is, ss, len);
			if (JCE_SUCCESS != ret)	return ret;

			ret = JString_assign(s, ss, len);
			if (JCE_SUCCESS != ret)	return ret;
		}
		break;
	case eString4:
		{
			uint32_t len;
			char *ss;
			ret = JceInputStream_readBuf(is, &len, sizeof(len));
			if (JCE_SUCCESS != ret)	return ret;
			len = jce_ntohl(len);
			if (len > JCE_MAX_STRING_LENGTH)
			{
				snprintf(is->_err, sizeof(is->_err), "invalid string size, tag: %d, size: %d", tag, len);
				return JCE_DECODE_ERROR;
			}
			ss = JceMalloc(sizeof(Char)*len);
			if (!ss)
				return JCE_MALLOC_ERROR;

			ret = JceInputStream_readBuf(is, ss, len);
			if (JCE_SUCCESS != ret)
			{
				JceFree(ss);
				return ret;
			}

			ret = JString_assign(s, ss, len);

			JceFree(ss);
			return ret;
		}
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'string' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readMap(JceInputStream * is, JMapWrapper* m, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eMap:
		{
			Int32 size, i;
			ret = JceInputStream_readInt32(is, &size, 0, true);
			if (JCE_SUCCESS != ret)	return ret;

			if (size < 0)
			{
				snprintf(is->_err, sizeof(is->_err), "invalid map, tag: %d, size: %d", tag, size);
				return JCE_DECODE_ERROR;
			}
			for (i = 0; i < size; ++i)
			{

				uint32_t p1, p2, p3;
				p1 = is->_cur;
				ret = JceInputStream_skipField(is);
				if (JCE_SUCCESS != ret)	return ret;
				p2 = is->_cur;
				ret = JceInputStream_skipField(is);
				if (JCE_SUCCESS != ret)	return ret;
				p3 = is->_cur;

				ret = JMapWrapper_put(m, JString_data(is->_buf)+p1, p2-p1, JString_data(is->_buf)+p2, p3-p2);
				if (JCE_SUCCESS != ret)	return ret;
			}
		}
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'map' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readVectorChar(JceInputStream * is, JString *v, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eSimpleList:
		{
			Int32 size;
			DataHead *hh = DataHead_new();
			if (!hh) return JCE_MALLOC_ERROR;

			ret = DataHead_readFrom(hh, is);
			if (JCE_SUCCESS != ret)
			{
				DataHead_del(&hh);
				return ret;
			}

			if (DataHead_getType(hh) != eChar)
			{
				snprintf(is->_err, sizeof(is->_err), "type mismatch, tag: %d, type: %d, %d", tag, DataHead_getType(is->_h), DataHead_getType(hh));
				DataHead_del(&hh);
				return JCE_DECODE_ERROR;
			}

			ret = JceInputStream_readInt32(is, &size, 0, true);
			if (JCE_SUCCESS != ret)
			{
				DataHead_del(&hh);
				return ret;
			}
			if (size < 0)
			{
				snprintf(is->_err, sizeof(is->_err), "invalid size, tag: %d, type: %d, %d, size: %d", tag, DataHead_getType(is->_h), DataHead_getType(hh), size);
				DataHead_del(&hh);
				return JCE_DECODE_ERROR;
			}

			DataHead_del(&hh);
			ret = JString_resize(v, size);
			if (JCE_SUCCESS != ret)	return ret;

			ret = JceInputStream_readBuf(is, JString_data(v), size);
			if (JCE_SUCCESS != ret)	return ret;

			v->_len = size;

		}
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "type mismatch, tag: %d, type: %d", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}

Int32 JceInputStream_readVector(JceInputStream * is, JArray* v, uint8_t tag, Bool isRequire)
{
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	switch (DataHead_getType(is->_h))
	{
	case eList:
		{
			Int32 size, i;
			ret = JceInputStream_readInt32(is, &size, 0, true);
			if (JCE_SUCCESS != ret)	return ret;

			if (size < 0)
			{
				snprintf(is->_err, sizeof(is->_err), "invalid size, tag: %d, type: %d, size: %d", tag, DataHead_getType(is->_h), size);
				return JCE_DECODE_ERROR;
			}
			for (i = 0; i < size; ++i)
			{
				uint32_t p1, p2;
				p1 = is->_cur;
				ret = JceInputStream_skipField(is);
				if (JCE_SUCCESS != ret)	return ret;

				p2 = is->_cur;

				ret = JArray_pushBack(v, JString_data(is->_buf)+p1, p2-p1);
				if (JCE_SUCCESS != ret)	return ret;
			}
		}
		break;
	default:
		{
			snprintf(is->_err, sizeof(is->_err), "read 'vector' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
			return JCE_DECODE_ERROR;
		}
	}
	return JCE_SUCCESS;
}


/// 读取结构
Int32 JceInputStream_readStruct(JceInputStream * is, void * st, uint8_t tag, Bool isRequire)
{
	Int32 ret=0;
	const JStructBase* jst = st; 
	JString *s = JString_new();
	JceInputStream *i = JceInputStream_new();
	do
	{
		if (!s || !i)
		{
			ret = JCE_MALLOC_ERROR; break;
		}
		ret = JceInputStream_readStructString(is, s, tag, isRequire);
		if ( JCE_SUCCESS != ret ) break;
		ret= JceInputStream_setBuffer(i, JString_data(s), JString_size(s));
		if ( JCE_SUCCESS != ret ) break;
		ret = jst->readFrom(st, i);
	}while (0);
	if (s) JString_del(&s);
	if (i) JceInputStream_del(&i);
	return ret;
}

Int32 JceInputStream_readStructString(JceInputStream * is, JString * st, uint8_t tag, Bool isRequire)
{
	uint32_t pos1,pos2;
	Int32 ret = JceInputStream_checkValid(is,tag,isRequire);
	if (JCE_DECODE_EOPNEXT == ret) return JCE_SUCCESS;
	if (JCE_SUCCESS != ret)	return ret;

	if (DataHead_getType(is->_h) != eStructBegin)
	{
		snprintf(is->_err, sizeof(is->_err), "read 'struct' type mismatch, tag: %d, get type: %d.", tag, DataHead_getType(is->_h));
		return JCE_DECODE_ERROR;
	}
	pos1 = is->_cur;
	ret = JceInputStream_skipToStructEnd(is);
	if (JCE_SUCCESS != ret)	return ret;
	pos2 = is->_cur;

	ret = JString_assign(st, (const char*)(JString_data(is->_buf) + pos1), pos2-pos1-1);
	if (JCE_SUCCESS != ret)	return ret;

	return JCE_SUCCESS;
} 


Int32 JceInputStream_init(JceInputStream* is)
{
	return JceStream_init(is);
}

JceInputStream * JceInputStream_new()
{
	return JceStream_new();
}

////////////////////////////////////////////////
void JceOutputStream_del(JceOutputStream ** os)
{
	JceStream_del(os);
}


void JceOutputStream_reset(JceOutputStream * os)
{
	JString_clear(os->_buf);
}

Int32 JceOutputStream_writeBuf(JceOutputStream * os, const void * buf, uint32_t len)
{
	char * p = (char *) buf;
	return JString_append(os->_buf, p, len);
}

char * JceOutputStream_getBuffer(JceOutputStream * os)          { return JString_data(os->_buf);}
uint32_t JceOutputStream_getLength(JceOutputStream * os)       { return JString_size(os->_buf);}


Int32 JceOutputStream_writeBool(JceOutputStream * os, Bool b, uint8_t tag)
{
	JceOutputStream_writeChar(os, (Char) b, tag);

	return JCE_SUCCESS;
}

Int32 JceOutputStream_writeChar(JceOutputStream * os, Char n, uint8_t tag)
{
	Int32 ret;
	//DataHead_setTag(os->_h, tag);
	if (n == 0)
	{
		//DataHead_setType(os->_h, eZeroTag);
		//ret = DataHead_writeTo(os->_h, os);
		ret = DataHead_setAndWriteTo(os->_h, eZeroTag, tag, os);

		if (JCE_SUCCESS != ret)	return ret;
	}
	else
	{
		//DataHead_setType(os->_h, eChar);
		//ret = DataHead_writeTo(os->_h, os);
		ret = DataHead_setAndWriteTo(os->_h, eChar, tag, os);
		if (JCE_SUCCESS != ret)	return ret;

		return JceOutputStream_writeBuf(os, &n, sizeof(n));
	}

	return JCE_SUCCESS;
}

Int32 JceOutputStream_writeUInt8(JceOutputStream * os, UInt8 n, uint8_t tag)
{
	return JceOutputStream_writeShort(os, (Short)n, tag);
}

Int32 JceOutputStream_writeShort(JceOutputStream * os, Short n, uint8_t tag)
{
	Int32 ret;

	if (n >= (-128) && n <= 127)
	{
		return JceOutputStream_writeChar(os, (Char) n, tag);
	}
	else
	{
		ret = DataHead_setAndWriteTo(os->_h, eShort, tag, os);
		if (JCE_SUCCESS != ret)	return ret;

		n = jce_htons(n);
		return JceOutputStream_writeBuf(os, &n, sizeof(n));
	}

	return JCE_SUCCESS;
}

Int32 JceOutputStream_writeUInt16(JceOutputStream * os, UInt16 n, uint8_t tag)
{
	return JceOutputStream_writeInt32(os, (Int32)n, tag);
}

Int32 JceOutputStream_writeInt32(JceOutputStream * os, Int32 n, uint8_t tag)
{
	Int32 ret;

	if (n >= (-32768) && n <= 32767)
	{
		return JceOutputStream_writeShort(os, (Short) n, tag);
	}
	else
	{
		ret = DataHead_setAndWriteTo(os->_h, eInt32, tag, os);
		if (JCE_SUCCESS != ret)	return ret;

		n = jce_htonl(n);
		return JceOutputStream_writeBuf(os, &n, sizeof(n));
	}

	return JCE_SUCCESS;
}

Int32 JceOutputStream_writeUInt32(JceOutputStream * os, UInt32 n, uint8_t tag)
{
	return JceOutputStream_writeInt64(os, (Int64)n, tag);
}

Int32 JceOutputStream_writeInt64(JceOutputStream * os, Int64 n, uint8_t tag)
{
	Int32 ret;

#ifdef __MTK_64 
	if (n.high == 0 )
	{
		return JceOutputStream_writeInt32(os, (Int32)n.low, tag);
	}
#else
	if (n >= (-2147483647-1) && n <= 2147483647)
	{
		return JceOutputStream_writeInt32(os, (Int32) n, tag);
	}
#endif
	else
	{
		ret = DataHead_setAndWriteTo(os->_h, eInt64, tag, os);
		if (JCE_SUCCESS != ret)	return ret;

		n = jce_htonll(n);
		return JceOutputStream_writeBuf(os, &n, sizeof(n));
	}

	return JCE_SUCCESS;
}



Int32 JceOutputStream_writeFloat(JceOutputStream * os, Float n, uint8_t tag)
{
	Int32 ret;

	ret = DataHead_setAndWriteTo(os->_h, eFloat, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	n = jce_htonf(n);
	return JceOutputStream_writeBuf(os, &n, sizeof(n));
}

Int32 JceOutputStream_writeDouble(JceOutputStream * os, Double n, uint8_t tag)
{
	Int32 ret=0;
	//DataHead_setType(os->_h, eDouble);
	//DataHead_setTag(os->_h, tag);

	//ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eDouble, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	n = jce_htond(n);
	return JceOutputStream_writeBuf(os, &n, sizeof(n));
}
Int32 JceOutputStream_writeString(JceOutputStream * os, JString* s, uint8_t tag)
{
	return JceOutputStream_writeStringBuffer(os, JString_data(s), JString_size(s), tag);
}

Int32 JceOutputStream_writeStringBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag)
{
	Int32 ret=0;

	//DataHead_setType(os->_h, eString1);
	//DataHead_setTag(os->_h, tag);

	if (len > 255)
	{
		uint32_t n;

		//DataHead_setType(os->_h, eString4);
		//ret = DataHead_writeTo(os->_h, os);
		ret = DataHead_setAndWriteTo(os->_h, eString4, tag, os);
		if (JCE_SUCCESS != ret)	return ret;

		n = jce_htonl(len);
		ret = JceOutputStream_writeBuf(os, &n, sizeof(n));
		if (JCE_SUCCESS != ret)	return ret;

		return JceOutputStream_writeBuf(os, buff, len);
	}
	else
	{
		uint8_t n;

		if (len > JCE_MAX_STRING_LENGTH)
		{

			snprintf(os->_err, sizeof(os->_err), "invalid string size, tag: %d, size: %u", tag, (uint32_t)len);
			return JCE_ENCODE_ERROR;
		}

		//ret = DataHead_writeTo(os->_h, os);
		ret = DataHead_setAndWriteTo(os->_h, eString1, tag, os);

		n = len;
		ret = JceOutputStream_writeBuf(os, &n, sizeof(n));
		if (JCE_SUCCESS != ret)	return ret;

		return JceOutputStream_writeBuf(os, buff, len);
	}

	return JCE_SUCCESS;
}


Int32 JceOutputStream_writeMap(JceOutputStream * os, JMapWrapper* m, uint8_t tag)
{
	uint32_t i;
	Int32 n;
	Int32 ret;

	//DataHead_setType(os->_h, eMap);
	//DataHead_setTag(os->_h, tag);

	// ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eMap, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	n = JMapWrapper_size(m);
	ret = JceOutputStream_writeInt32(os, n, 0);
	if (JCE_SUCCESS != ret)	return ret;

	for (i = 0; i < JMapWrapper_size(m); ++i)
	{
		ret = JceOutputStream_writeBuf(os, JArray_getPtr(m->first, i), JArray_getLength(m->first, i));
		if (JCE_SUCCESS != ret)	return ret;

		ret = JceOutputStream_writeBuf(os, JArray_getPtr(m->second, i), JArray_getLength(m->second, i));
		if (JCE_SUCCESS != ret)	return ret;
	}

	return JCE_SUCCESS;
}


Int32 JceOutputStream_writeVector(JceOutputStream * os, JArray* v, uint8_t tag)
{
	Int32 i, n, ret;

	//DataHead_setType(os->_h, eList);
	// DataHead_setTag(os->_h, tag);

	//ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eList, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	n = JArray_size(v);
	ret = JceOutputStream_writeInt32(os, n, 0);
	if (JCE_SUCCESS != ret)	return ret;

	for (i = 0; i < JArray_size(v); ++i)
	{
		ret = JceOutputStream_writeBuf(os, JArray_getPtr(v, i), JArray_getLength(v, i));
		if (JCE_SUCCESS != ret)	return ret;
	}

	return JCE_SUCCESS;
}

Int32 JceOutputStream_writeVectorChar(JceOutputStream * os, JString *v, uint8_t tag)
{
	return JceOutputStream_writeVectorCharBuffer(os, JString_data(v), JString_size(v), tag);
}

Int32 JceOutputStream_writeVectorCharBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag)
{
	Int32 ret;
	DataHead *hh;

	//DataHead_setType(os->_h, eSimpleList);
	// DataHead_setTag(os->_h, tag);

	//ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eSimpleList, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	hh = DataHead_new();
	if (!hh) return JCE_MALLOC_ERROR;

	//DataHead_setType(hh, eChar);
	//DataHead_setTag(hh, 0);

	//ret = DataHead_writeTo(hh, os);
	ret = DataHead_setAndWriteTo(hh, eChar, 0, os);
	DataHead_del(&hh);

	if (JCE_SUCCESS != ret)	return ret;

	ret = JceOutputStream_writeInt32(os, len, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return JceOutputStream_writeBuf(os, buff, len);
}


Int32 JceOutputStream_writeStruct(JceOutputStream * os,const void * st,  uint8_t tag)
{
	Int32 ret=0;
	const JStructBase* jst = st; 
	JceOutputStream *o = JceOutputStream_new();
	do
	{
		if (!o)
		{
			ret = JCE_MALLOC_ERROR; break;
		}
		ret = jst->writeTo(st, o);
		if ( JCE_SUCCESS != ret ) break;
		ret = JceOutputStream_writeStructBuffer(os, JceOutputStream_getBuffer(o),JceOutputStream_getLength(o), tag);
	}while (0);
	if (o) JceOutputStream_del(&o);
	return ret;
}

Int32 JceOutputStream_writeStructString(JceOutputStream * os, JString * v,  uint8_t tag)
{
	return JceOutputStream_writeStructBuffer(os, JString_data(v), JString_size(v), tag);
}


Int32 JceOutputStream_writeStructBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag)
{
	Int32 ret;
	//DataHead_setType(os->_h, eStructBegin);
	//DataHead_setTag(os->_h, tag);

	//ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eStructBegin, tag, os);
	if (JCE_SUCCESS != ret)	return ret;

	ret = JceOutputStream_writeBuf(os, buff, len);
	if (JCE_SUCCESS != ret)	return ret;

	//DataHead_setType(os->_h, eStructEnd);
	//DataHead_setTag(os->_h, 0);

	//ret = DataHead_writeTo(os->_h, os);
	ret = DataHead_setAndWriteTo(os->_h, eStructEnd, 0, os);
	return ret;
}


Int32 JceOutputStream_init(JceOutputStream * os)
{
	return JceStream_init(os);
}


JceOutputStream * JceOutputStream_new()
{
	return JceStream_new();
}


