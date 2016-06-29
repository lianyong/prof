#pragma once

#include "prof_parse_data.h"
#include "prof.h"
#include "add2line.h"

class CParseProcess
{
public:
	static double get_ms_duration(const function_node_data& lhs, const function_node_data& rhs);
	int layout_result_cpuprof_csv(FILE* file);
	int layout_result_mthread_csv(FILE* file);
	bool parse_cpuprofile();
	bool parse_thread_name(std::string data);

	bool run(int argc, char* argv[]);

private:
	bool parse_program_args(int argc, char* argv[]);

private:
	std::string profile_name, exec_name, libdir, toolset_prefix, output_name;
	double  empty_function_fix;

	CALL_STACK      call_stack;
	STACK_MAP       stack_map;
	FUNCTION_TIME   function_time;          //包括调用其他函数时间
	std::map<PROF_THREADID_TYPE, std::string>   thread_name;
	addr2line       addr_convert;
};
