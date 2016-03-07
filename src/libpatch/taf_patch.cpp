#include "util/tc_file.h"
#include "util/tc_option.h"
#include "util/tc_md5.h"
#include "log/taf_logger.h"
#include "patch/taf_patch.h"

#include <iostream>

namespace taf
{
TafPatch::TafPatch() : _bRemove(false)
{
}

void TafPatch::checkLocalDir()
{
	if (!taf::TC_File::isFileExistEx(_local_dir, S_IFDIR))
	{
		throw TafPatchException("local dir '" + _local_dir + "' must be a directory");
	}
}

void TafPatch::init(const PatchPrx &patchPrx, const string &remote_dir, const string &local_dir, const bool bCheck)
{
	_patchPrx   = patchPrx;
	_remote_dir = remote_dir;
	_local_dir  = local_dir;

	if (bCheck)	checkLocalDir();
}

void TafPatch::setRemove(bool bRemove)
{
	_bRemove = bRemove;
}

void TafPatch::download(const TafPatchNotifyInterfacePtr &pPtr)
{
	//记录下载本次服务的总的时间开始
	time_t timeBegin = TC_TimeProvider::getInstance()->getNow();

	checkLocalDir();

	if (_bRemove)
	{
		string tmp = TC_File::simplifyDirectory(_local_dir);// + "/" + _remote_dir);

		if (pPtr) pPtr->onRemoveLocalDirectory(tmp);

		int ret = TC_File::removeFile(tmp, true);
		if (ret != 0)
		{
			throw TafPatchException("remove '" + tmp + "' error", errno);
		}

		if (pPtr) pPtr->onRemoveLocalDirectoryOK(tmp);
	}

	if (pPtr) pPtr->onListFile();

	//获取同步列表
	vector<FileInfo> vf;
	int ret = _patchPrx->listFileInfo(_remote_dir, vf);
	if (ret != 0 && ret != 1)
	{
		throw TafPatchException("listFileInfo error");
	}
	if (pPtr) pPtr->onListFileOK(vf);

	if (ret == 0)
	{
		//path是路径, 对每个文件下载
		for (size_t i = 0; i < vf.size(); i++)
		{
			download(true, vf[i], pPtr);
		}
	}
	else if (ret == 1)
	{
		//path是文件
		download(false, vf[0], pPtr);
	}

	time_t timeEnd = TC_TimeProvider::getInstance()->getNow();
	if (pPtr) pPtr->onDownloadAllOK(vf, timeBegin, timeEnd);
}

void TafPatch::download(bool bDir, const FileInfo &fi, const TafPatchNotifyInterfacePtr &pPtr)
{
	if (pPtr) pPtr->onDownload(fi);

	//获取本地文件目录
	string file_dir = _local_dir;

	if (bDir)
	{
		file_dir = taf::TC_File::simplifyDirectory(_local_dir + "/" + taf::TC_File::extractFilePath(fi.path));

		if (!file_dir.empty())
		{
			//创建本地目录
			if (!taf::TC_File::makeDirRecursive(file_dir))
			{
				throw TafPatchException("create directory '" + file_dir + "' failed!");
			}
		}
		else
		{
			file_dir = ".";
		}
	}

	//本地文件名
	string file = taf::TC_File::simplifyDirectory(file_dir + "/" + taf::TC_File::extractFileName(fi.path));

	FILE *fp = fopen(file.c_str(), "wb");
	if (!fp)
	{
		throw TafPatchException("fopen file '" + file + "' error", errno);
	}


	time_t timeBegin = TC_TimeProvider::getInstance()->getNow();
	try
	{
		//循环下载文件到本地
		vector<char> v;
		int pos = 0;
		while (true)
		{
			v.clear();
			int ret;

			if (bDir)
			{
				ret = _patchPrx->download(taf::TC_File::simplifyDirectory(_remote_dir + "/" + fi.path), pos, v);
			}
			else
			{
				ret = _patchPrx->download(taf::TC_File::simplifyDirectory(_remote_dir), pos, v);
			}
			if (ret < 0)
			{
				throw TafPatchException("download file '" + file + "' error!");
			}
			else if (ret == 0)
			{
				size_t r = fwrite((void*)&v[0], 1, v.size(), fp);
				if (r == 0)
				{
					throw TafPatchException("fwrite file '" + file + "' error!", errno);
				}
				pos += r;
				if (pPtr) pPtr->onDownloading(fi, pos, file);
			}
			else if (ret == 1)
			{
				break;
			}
		}
	}
	catch (...)
	{
		fclose(fp);
		fp = NULL;
		throw;
	}

	fclose(fp);

	if (fi.canExec)
	{
		int ret = taf::TC_File::setExecutable(file, true);
		if (ret == 0)
		{
			if (pPtr) pPtr->onSetExecutable(fi);
		}
		else
		{
			throw TafPatchException("set file '" + file + "' executable error!");
		}
	}

	//检查MD5值
	if (!fi.md5.empty())
	{
		std::string smd5 = taf::TC_MD5::md5file(file);
		if (smd5 != fi.md5)
		{
			LOG->error() << "L:" << __FILE__ << "," << __LINE__ << "|" << fi.path << "|" << fi.md5 << "|" << smd5 << endl;
			throw TafPatchException(fi.path + "'s md5 is not equal to the file in patch server!");;
		}
	}

	time_t timeEnd = TC_TimeProvider::getInstance()->getNow();
	if (pPtr) pPtr->onReportTime(fi.path, timeBegin, timeEnd);

	if (pPtr) pPtr->onDownloadOK(fi, file);
}


void TafPatch::downloadSubborn(const TafPatchNotifyInterfacePtr &pPtr)
{
	//STEP0: 记录下载本次服务的总的时间开始
	time_t timeBegin = TC_TimeProvider::getInstance()->getNow();

	//STEP1: 获取同步列表
	if (pPtr) pPtr->onListFile();
	vector<FileInfo> vf;
	int ret = _patchPrx->listFileInfoSubborn(_remote_dir, vf);
	if (ret != 0 && ret != 1)
	{
		throw TafPatchException("listFileInfo error");
	}
	if (pPtr) pPtr->onListFileOK(vf);

	//STEP2: 根据远端路径类型，生成临时存储目录
	std::string sTmpPth = _local_dir + "/tmpfortafpatch_" + taf::TC_Common::tostr<time_t>(timeBegin);

	//STEP3: 判断是否删除老的临时目录
	if (_bRemove)
	{
		int ret = TC_File::removeFile(sTmpPth, true);
		if (ret != 0)
		{
			throw TafPatchException("remove '" + sTmpPth + "' error", errno);
		}
	}

	//STEP4: 创建临时下载目录
	//如果已经存在删除该目录下的所有文件
	TC_File::removeFile(sTmpPth, true);
	if (!TC_File::makeDirRecursive(sTmpPth))
	{
		throw TafPatchException("create '" + sTmpPth + "' error", errno);
	}

	//STEP5: 开始下载文件
	if (ret == 0)
	{
		//path是路径, 对每个文件下载
		for (size_t i = 0; i < vf.size(); i++)
		{
			downloadSubborn(sTmpPth, true, vf[i], pPtr);
		}
	}
	else
	{
		//path是文件
		downloadSubborn(sTmpPth, false, vf[0], pPtr);
	}

	//STEP6: 将临时目录下的程序拷贝到目标路径下
	TC_File::copyFile(sTmpPth, _local_dir, true);

	//STEP7: 删除临时目录
	TC_File::removeFile(sTmpPth, true);

	//STEP8: 上报数据
	time_t timeEnd = TC_TimeProvider::getInstance()->getNow();
	if (pPtr) pPtr->onDownloadAllOK(vf, timeBegin, timeEnd);
}

void TafPatch::downloadSubborn(const string & sTmpPth, bool bDir, const FileInfo & fi, const TafPatchNotifyInterfacePtr & pPtr)
{
	if (pPtr) pPtr->onDownload(fi);
	string sFilePth = sTmpPth;

	if (bDir)
	{
		sFilePth = taf::TC_File::simplifyDirectory(sTmpPth + "/" + taf::TC_File::extractFilePath(fi.path));

		if (!sFilePth.empty())
		{
			//创建本地目录
			if (!taf::TC_File::makeDirRecursive(sFilePth))
			{
				throw TafPatchException("create directory '" + sFilePth + "' failed!");
			}
		}
		else
		{
			sFilePth = ".";
		}
	}

	//本地文件名
	string file = taf::TC_File::simplifyDirectory(sFilePth + "/" + taf::TC_File::extractFileName(fi.path));

	FILE *fp = fopen(file.c_str(), "wb");
	if (!fp)
	{
		throw TafPatchException("fopen file '" + file + "' error", errno);
	}

	time_t timeBegin = TC_TimeProvider::getInstance()->getNow();
	try
	{
		//循环下载文件到本地
		vector<char> v;
		int pos = 0;
		while (true)
		{
			v.clear();
			int ret;

			if (bDir)
			{
				ret = _patchPrx->downloadSubborn(taf::TC_File::simplifyDirectory(_remote_dir + "/" + fi.path), pos, v);
			}
			else
			{
				ret = _patchPrx->downloadSubborn(taf::TC_File::simplifyDirectory(_remote_dir), pos, v);
			}
			if (ret < 0)
			{
				throw TafPatchException("download file '" + file + "' error!");
			}
			else if (ret == 0)
			{
				size_t r = fwrite((void*)&v[0], 1, v.size(), fp);
				if (r == 0)
				{
					throw TafPatchException("fwrite file '" + file + "' error!", errno);
				}
				pos += r;
				if (pPtr) pPtr->onDownloading(fi, pos, file);
			}
			else if (ret == 1)
			{
				break;
			}
		}
	}
	catch (...)
	{
		fclose(fp);
		fp = NULL;
		throw;
	}

	fclose(fp);

	if (fi.canExec)
	{
		int ret = taf::TC_File::setExecutable(file, true);
		if (ret == 0)
		{
			if (pPtr) pPtr->onSetExecutable(fi);
		}
		else
		{
			throw TafPatchException("set file '" + file + "' executable error!");
		}
	}

	//检查MD5值
	if (!fi.md5.empty())
	{
		std::string smd5 = taf::TC_MD5::md5file(file);
		if (smd5 != fi.md5)
		{
			LOG->error() << "L:" << __FILE__ << "," << __LINE__ << "|" << fi.path << "|" << fi.md5 << "|" << smd5 << endl;
			throw TafPatchException(fi.path + "'s md5 is not equal to the file in patch server!");;
		}
	}

	time_t timeEnd = TC_TimeProvider::getInstance()->getNow();
	if (pPtr) pPtr->onReportTime(fi.path, timeBegin, timeEnd);
	if (pPtr) pPtr->onDownloadOK(fi, file);
}

}


