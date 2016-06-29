#pragma once

EXTERN_DECLARE void to_lower_string(string& str);

EXTERN_DECLARE void to_upper_string(string& str);

EXTERN_DECLARE bool copy_file(const string& srcFile, const string& dstFile);

EXTERN_DECLARE long get_file_size(FILE* pFile);

EXTERN_DECLARE std::string dump_file(const std::string& fileName);

EXTERN_DECLARE std::string get_stdout(const std::string& commad);

EXTERN_DECLARE size_t get_stdout(const std::string& commad, char** pData);

EXTERN_DECLARE void trim_left(std::string& str, const std::string& trim_str /*= ' '*/);

EXTERN_DECLARE void trim_right(std::string& str, const std::string& trim_str /*= ' '*/);

EXTERN_DECLARE void trim_both(std::string& str, const std::string& trim_str /*= ' '*/);

EXTERN_DECLARE std::string fmt_string(const char * format, ...);

