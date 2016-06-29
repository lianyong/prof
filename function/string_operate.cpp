#include "string_operate.h"


void to_lower_string(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ptr_fun(static_cast<int(*)(int)>(tolower)));
}

void to_upper_string(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ptr_fun(static_cast<int(*)(int)>(toupper)));
}

bool copy_file(const string& srcFile, const string& dstFile)
{
	try
	{
		std::ifstream ifs(srcFile.c_str(), ios::binary);
		std::ofstream ofs(dstFile.c_str(), ios::binary | ios::trunc);

#if 0
		ofs << ifs.rdbuf();
		ofs.close();
#else
		std::streambuf* backup = ofs.rdbuf(ifs.rdbuf());
		ofs.close();
		ofs.rdbuf(backup);
#endif
		
		return ifs && ofs;
	}
	catch (std::exception& err)
	{
		YERR(err.what());
	}

#if 0
	ifstream ifs(srcFile.c_str(), ios::binary);
	ofstream ofs(dstFile.c_str(), ios::binary | ios::trunc);

	const size_t size = 256;
	if (ifs && ofs)
	{
		char buffer[size] = { 0 };
		while (ifs.read(buffer, size))
		{
			ofs.write(buffer, ifs.gcount());
		}

		return true;
	}
	else
	{
		cerr << "copy err when [" << srcFile << "] => [" << dstFile << "]" << endl;
	}

	return false;
#endif
}

long get_file_size(FILE* pFile)
{
	if (pFile == NULL)
	{
		return 0;
	}

	long curpos;
	curpos = ftell(pFile);
	fseek(pFile, 0L, SEEK_END);
	long iFileSize = ftell(pFile);
	fseek(pFile, curpos, SEEK_SET);
	return iFileSize;
}

std::string dump_file(const std::string& fileName)
{
	FILE* file = fopen(fileName.c_str(), "rb");
	std::string file_buffer;
	if (file != nullptr)
	{
		// 获取文件长度.
		int nLength = get_file_size(file);
		file_buffer.resize(nLength + 1);
		fread(&file_buffer[0], sizeof(std::string::value_type), nLength, file);
		fclose(file);
	}
	return file_buffer;
}

std::string get_stdout(const std::string& commad)
{
	try
	{
		//打开命令
		FILE* pFile = popen(commad.c_str(), "r");
		if (pFile == 0)
		{
			fprintf(stderr, "popen file [%p] failed\n", pFile);
		}

		const size_t buffer_size = 256;
		char buffer[buffer_size];
		size_t size = 0;
		std::string ret;
		while (size = fread((void*)buffer, 1, buffer_size, pFile))
		{
			ret.append(buffer, size);
		}

		pclose(pFile);
		return ret;
	}
	catch (std::exception& err)
	{
		YERR(err.what());
	}
}

size_t get_stdout(const std::string& commad, char** pData)
{
	//打开命令
	FILE* pFile = popen(commad.c_str(), "rb");
	if (pFile == 0)
	{
		fprintf(stderr, "popen file [%p] failed\n", pFile);
	}

	long lSize = get_file_size(pFile);
	size_t uRet = 0;

	try
	{
		*pData = new char[lSize];
		uRet = fread((void*)(*pData), 1, lSize, pFile);
	}
	catch (std::exception& err)
	{
		YERR(err.what());
	}

	pclose(pFile);
	return uRet;
}

void trim_left(std::string& str, const std::string& trim_str /*= ' '*/)
{
	std::string::size_type pos = str.find_first_not_of(trim_str);
	str.erase(0, pos);
}

void trim_right(std::string& str, const std::string& trim_str /*= ' '*/)
{
	std::string::size_type pos = str.find_last_not_of(trim_str);
	str.erase(pos + 1);
}

void trim_both(std::string& str, const std::string& trim_str /*= ' '*/)
{
	trim_left(str, trim_str);
	trim_right(str, trim_str);
}

