#include "prof_parse_inc.h"

#define FGET_TYPE(VAR, FILE_HANDLE) fread(&VAR, sizeof(VAR), 1, FILE_HANDLE)

using namespace boost::program_options;

namespace po = boost::program_options;

double CParseProcess::get_ms_duration(const function_node_data& lhs, const function_node_data& rhs)
{
	return double(int64_t(lhs.nsec) - rhs.nsec) / 1000000 + (int64_t(lhs.sec) - rhs.sec) * 1000;
}

int CParseProcess::layout_result_cpuprof_csv(FILE* file)
{
	if (empty_function_fix != 0)
	{
		fprintf(file, "Self,Real,Call,Count,Avg,Fix-Self,Fix-Avg,Function\n");
	}
	else
	{
		fprintf(file, "Self,Real,Call,Count,Avg,Function\n");
	}
	for (auto& pr : function_time)
	{
		auto& func_addr = pr.first;
		auto& info = pr.second;

		double self_time = info.real_time - info.call_time;

		std::string function_name = "\"";
		function_name += addr_convert.getline(func_addr);
		function_name += "\"";


		if (empty_function_fix != 0)
		{
			double fix_self_time = self_time - info.call_count*empty_function_fix;
			fprintf(file, "%.3lf,%.3lf,%.3lf,%.3u,%.3lf,%.3lf,%.3lf,%s\n",
				self_time,                      //Self
				info.real_time,                 //Real
				info.call_time,                 //Call
				info.call_count,                //Count
				self_time / info.call_count,      //Avg
				fix_self_time,
				fix_self_time / info.call_count,
				function_name.c_str());         //Function
		}
		else
		{
			fprintf(file, "%.3lf,%.3lf,%.3lf,%.3u,%.3lf,%s\n",
				self_time,                      //Self
				info.real_time,                 //Real
				info.call_time,                 //Call
				info.call_count,                //Count
				self_time / info.call_count,      //Avg
				function_name.c_str());         //Function
		}
	}
}

int CParseProcess::layout_result_mthread_csv(FILE* file)
{
	fprintf(file, "Funtion, Threads, Thread List\n");
	for (auto& pr : function_time)
	{
		auto& func_addr = pr.first;
		auto& info = pr.second;

		std::string function_name = "\"";
		function_name += addr_convert.getline(func_addr);
		function_name += "\"";

		if (info.threadid_set.size() > 1)
		{
			fprintf(file, "%s,", function_name.c_str());         //Function
			fprintf(file, "%d,", info.threadid_set.size());         //Function

			for (auto& threadid : info.threadid_set)
			{
				if (thread_name.count(threadid) != 0)
				{
					fprintf(file, "%s,", thread_name[threadid].c_str());         //Function
				}
				else
				{
					fprintf(file, "%x,", threadid);         //Function
				}
			}

			fprintf(file, "\n");         //Function
		}
	}
}

bool CParseProcess::parse_cpuprofile()
{
	FILE* profile = fopen(profile_name.c_str(), "r");

	if (profile)
	{
		int read_count = 0;
		PROF_NODE_TYPE type;
		while ((read_count = fread(&type, sizeof(PROF_NODE_TYPE), 1, profile)) != 0)
		{
			//STRING类型记录
			if (type & PROF_STRING_MASK)
			{
				PROF_SIZE_TYPE size = 0;
				read_count = FGET_TYPE(size, profile);
				if (size != 0)
				{
					std::string data(size, 0);
					read_count = fread(&data[0], sizeof(char), size, profile);
					assert(read_count == size);

					//maps记录
					if (type == PROF_TYPE_MAPSFILE)
					{
						addr_convert.load_maps(data);
					}
					else if (type == PROF_TYPE_THREAD_NAME)
					{
						parse_thread_name(data);
					}
				}
			}
			//函数进出记录
			else if (type == PROF_TYPE_FUNCTION_BEGIN
				|| type == PROF_TYPE_FUNCTION_EXIT)
			{
				function_node_data data;
				read_count = fread(&data, sizeof(data), 1, profile);

				CALL_STACK& call_stack = stack_map[data.pid];
				if (type == PROF_TYPE_FUNCTION_BEGIN)
				{
					call_stack.push_back(data);
					function_time[data.func_addr].call_count++;
					function_time[data.func_addr].threadid_set.insert(data.pid);
				}
				else if (type == PROF_TYPE_FUNCTION_EXIT)
				{
					double func_duration = get_ms_duration(data, call_stack.back());
					function_time[data.func_addr].real_time += func_duration;
					call_stack.pop_back();

					//上次层的calltime加上自己调用时间
					if (!call_stack.empty())
					{
						function_time[call_stack.back().func_addr].call_time += func_duration;
					}
				}
			}
		}
	}
	else
	{
		YERR("Can not open file %s.\n", profile_name.c_str());
		fclose(profile);
		return false;
	}

	fclose(profile);
	return true;
}


bool CParseProcess::parse_thread_name(std::string data)
{
	std::string::size_type pos = data.find(":");
	if (pos != std::string::npos)
	{
		std::string strid = data.substr(0, pos);
		std::string strname = data.substr(pos + 1);
		PROF_THREADID_TYPE  id = strtoul(strid.c_str(), NULL, 16);
		thread_name.insert(std::make_pair(id, strname));
	}
}

bool CParseProcess::run(int argc, char* argv[])
{
	variables_map pargs;

	if (!parse_program_args(argc, argv))
	{
		return 1;
	}

	parse_cpuprofile();

	FILE *output = stdout;
	if (pargs.count("output"))
	{
		output = fopen(output_name.c_str(), "w");
		if (output == 0)
		{
			YERR("can not open file %s", output_name.c_str());
			return 1;
		}
	}

	if (pargs.count("mtprof"))
	{
		layout_result_mthread_csv(output);
	}
	if (pargs.count("cpuprof"))
	{
		layout_result_cpuprof_csv(output);
	}

	return 0;
}

bool CParseProcess::parse_program_args(int argc, char* argv[])
{
	po::variables_map pargs;

	/* boost参数解析 开始 */
	po::options_description opts("prof_parse [options] <executable> <profile> [solibpath]");

	opts.add_options()
		("help,h", "Show this page.")
		("executable,e", value<std::string>(&exec_name), "Exectuable name.")
		("profile,p", value<std::string>(&profile_name), "profile name")
		("solibpath,s", value<std::string>(&libdir)->default_value(""), "so libraries search path.")
		("toolset-prefix,t", value<std::string>(&toolset_prefix)->default_value(""), "toolset prefix for CROSS prof.")
		("output,o", value<std::string>(&output_name), "output name, to stdout if empty.")
		("fix,f", value<double>(&empty_function_fix), "decrease this value each function called.")
		("cpuprof,C", "prof cpu time.")
		("mtprof,M", "prof which Function was Called by more than one thread.")
		;

	po::positional_options_description pod;
	pod.add("executable", 1);
	pod.add("profile", 1);
	pod.add("solibpath", 1);

	try {
		auto pr = po::command_line_parser(argc, argv).options(opts).positional(pod).run();
		po::store(pr, pargs);
	}
	catch (std::exception& err)
	{
		YERR(err.what());
		return false;
	}
	pargs.notify();
	/* boost参数解析 结束 */

	if (pargs.count("help"))
	{
		std::cerr << opts << std::endl;
		return false;
	}

	addr_convert.set_so_search_path(libdir);
	addr_convert.set_exec(exec_name);
	addr_convert.set_toolset_prefix(toolset_prefix);

	return true;
}