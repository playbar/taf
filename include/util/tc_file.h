#ifndef __TC_FILE_H_
#define __TC_FILE_H_

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include "util/tc_ex.h"
#include "util/tc_common.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 文件处理类
// Author : j@syswin.com              
// Modify: j@syswin.com              
/////////////////////////////////////////////////
/**
* 文件异常类
*/
struct TC_File_Exception : public TC_Exception
{
    TC_File_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_File_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_File_Exception() throw(){};
};

/**
* 常用文件名处理函数
*/
class TC_File
{
public:

    /**
    * 获取文件大小, 如果文件不存在, 则返回0
    * @param    sFullFileName: 文件全路径(所在目录和文件名)
    * @return   ofstream::pos_type: 文件大小
    */
    static ifstream::pos_type getFileSize(const string &sFullFileName);

    /**
     * 是否是绝对路径, 忽略空格以'/'开头
     * @param sFullFileName
     *
     * @return bool
     */
    static bool isAbsolute(const string &sFullFileName);

    /**
    * 判断给定路径的文件是否存在
    * 注意: 如果文件是符号连接,则以符号连接判断
    *       而不是以符号连接指向的文件判断
    * @param sFullFileName: 文件全路径
    * @param iFileType: 文件类型, 缺省S_IFREG
    * @return bool: 存在, 返回true; 否则返回 false.
    */
    static bool isFileExist(const string &sFullFileName, mode_t iFileType = S_IFREG);

    /**
    * 判断给定路径的文件是否存在
    * 注意: 如果文件是符号连接,则以符号连接指向的文件判断
    * @param sFullFileName: 文件全路径
    * @param iFileType: 文件类型, 缺省S_IFREG
    * @return bool: 存在, 返回true; 否则返回 false.
    */
    static bool isFileExistEx(const string &sFullFileName, mode_t iFileType = S_IFREG);

    /**
     * 规则化目录名称, 把一些不用的去掉, 例如./等
     * @param path
     *
     * @return string
     */
    static string simplifyDirectory(const string& path);

    /**
    * 创建目录, 如果目录已经存在, 则也返回成功
    * @param sFullPath: 目录
    * @param iFlag: 权限, 默认 S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP
    *             | S_IXGRP | S_IROTH |  S_IXOTH
    * @return bool: 创建成功, 返回true; 否则返回 false.
    */
    static bool makeDir(const string &sDirectoryPath, mode_t iFlag = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH |  S_IXOTH);

     /**
    * 循环创建目录, 如果目录已经存在, 则也返回成功
    * @param sFullPath: 目录
     *@param iFlag: 权限, 默认 S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP
     *            | S_IXGRP | S_IROTH |  S_IXOTH
    * @return bool: 创建成功, 返回true; 否则返回 false.
    */

    static bool makeDirRecursive(const string &sDirectoryPath, mode_t iFlag = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH |  S_IXOTH);

    /**
     * 设置文件是否可执行
     * @param sFullFileName
     * @param canExecutable, true: 可执行, false:不可之行
     *
     * @return int,成功返回0, 其他失败
     */
    static int setExecutable(const string &sFullFileName, bool canExecutable);

    /**
     * 是否可执行
     * @param sFullFileName
     *
     * @return bool
     */
    static bool canExecutable(const string &sFullFileName);

    /**
     * 删除一个文件或目录
     * @param sFullFileName
     * @param bRecursive, 如果是目录是否递归删除
     * @return int, 0:成功 <0, 失败, 可以通过errno查看失败的原因
     */
    static int removeFile(const string &sFullFileName, bool bRecursive);

    /**
    * 读取文件到string, 文件不存在或者读取文件错误的时候, 返回为空
    * @param sFullFileName: 文件名称
    * @return string : 文件数据
    */
    static string load2str(const string &sFullFileName);

    /**
    * 写文件
    * @param sFullFileName: 文件名称
    * @param sFileData: 文件内容
    * @return 无
    */
    static void save2file(const string &sFullFileName, const string &sFileData);

    /**
     * 写文件
     * @param sFullFileName: 文件名
     * @param sFileData:数据指针
     * @param length:长度
     * 
     * @return int, 0:成功,-1:失败
     */
    static int save2file(const string &sFullFileName, const char *sFileData, size_t length);

    /**
     * 获取前当可执行文件路径
     *
     * @return string
     */
    static string getExePath();

    /**
    * 从一个完全文件名中去掉路径:例如:/usr/local/temp.gif 获取temp.gif
    *@param sFullFileName: 文件的完全名称
    *@return string 文件名称
    */
    static string extractFileName(const string &sFullFileName);

    /**
    * 从一个完全文件名中提取文件的路径
    * 例如1: "/usr/local/temp.gif" 获取"/usr/local/"
    * 例如2: "temp.gif" 获取 "./"
    * @param sFullFileName: 文件的完全名称
    * @return string 文件名称
    */
    static string extractFilePath(const string &sFullFileName);

    /**
    * 提取文件扩展名
    * 例如1: "/usr/local/temp.gif" 获取"gif"
    * 例如2: "temp.gif" 获取"gif"
    *@param sFullFileName: 文件名称
    *@return string 文件扩展名
    */
    static string extractFileExt(const string &sFullFileName);

    /**
    * 提取文件名称,去掉扩展名
    * 例如1: "/usr/local/temp.gif" 获取"/usr/local/temp"
    * 例如2: "temp.gif" 获取"temp"
    * @param sFullFileName: 文件名称
    * @return string 文件名称,去掉扩展名
    */
    static string excludeFileExt(const string &sFullFileName);

    /**
    * 替换文件扩展名, 如果无扩展名,则加上扩展名 =?1:
    * "/usr/local/temp.gif" 替换 "jpg" 得到"/usr/local/temp.jpg"
    * 例如1: "/usr/local/temp" 替换 "jpg" 得到"/usr/local/temp.jpg"
    * @param sFullFileName: 文件名称
    * @param sExt : 扩展名
    * @return string 提过扩展名后的文件名
    */
    static string replaceFileExt(const string &sFullFileName, const string &sExt);

    /**
    * 从一个url中获取完全文件名
    * 获取除http://外,第一个'/'后面的所有字符
    * 例如1:http://www.qq.com/tmp/temp.gif 获取tmp/temp.gif
    * 例如2:www.qq.com/tmp/temp.gif 获取tmp/temp.gif
    * 例如3:/tmp/temp.gif 获取tmp/temp.gif
    * @param sUrl: 文件的完全名称
    * @return string 文件名称
    */
    static string extractUrlFilePath(const string &sUrl);

    /**
    * 遍历文件时确定是否选择
    * 返回 1 则选择, 返回0则不选择
    */
    typedef int (*FILE_SELECT)(const dirent *);

    /**
    * 扫描一个目录
    * @param sFilePath : 搜索的路径
    * @param vtMatchFiles : 返回的文件名矢量表
    * @param f : 匹配函数,为NULL表示所有文件都获取
    * @param iMaxSize : 最大文件个数,iMaxSize <=0时,返回所有匹配文件
    * @return size_t : 文件个数
    */
    static size_t scanDir(const string &sFilePath, vector<string> &vtMatchFiles, FILE_SELECT f = NULL, int iMaxSize = 0);

    /**
     * 遍历目录, 获取目录下面的所有文件和子目录
     * @param path, 路径
     * @param files, 返回路径下面所有文件
     * @param bRecursive, 是否递归子目录
     *
     **/
    static void listDirectory(const string &path, vector<string> &files, bool bRecursive);
    
     /**
     * 复制文件或目录sExistFile到sNewFile
     * @param sExistFile
     * @param sNewFile
     * @param bRemove 是否先删除sNewFile再copy 防止Textfile busy导致复制失败
     * 
     * @return void
     */
    static void copyFile(const string &sExistFile, const string &sNewFile,bool bRemove = false);
};

}
#endif // TC_FILE_H
