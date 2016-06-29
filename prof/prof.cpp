#include "prof_inc.h"

#ifdef __cplusplus
extern "C"
{
#endif

	FILE* profile = nullptr;

	static function_node node;

#if PROF_USE_CPU_TIME
	static timespec nstime;
#else
	static timeval tv;
#endif

	void __cyg_profile_func_enter(void *this_func, void *call_site)
	{
		print_dl_info(this_func);

		if (profile != nullptr)
		{
#if PROF_USE_CPU_TIME
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &nstime);
			node.data.sec = nstime.tv_sec;
			node.data.nsec = nstime.tv_nsec;
#else
			gettimeofday(&tv, NULL);
			node.data.sec = tv.tv_sec;
			node.data.nsec = tv.tv_usec * 1000;
#endif

			node.type = (int)PROF_TYPE_FUNCTION_BEGIN;
			node.data.pid = getpid();
			node.data.func_addr = (PROF_ADDR_TYPE)this_func;
			fwrite(&node, sizeof(node), 1, profile);
		}
	}

	void __cyg_profile_func_exit(void *this_func, void *call_site)
	{
		if (profile != nullptr)
		{
#if PROF_USE_CPU_TIME
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &nstime);
			node.data.sec = nstime.tv_sec;
			node.data.nsec = nstime.tv_nsec;
#else
			gettimeofday(&tv, NULL);
			node.data.sec = tv.tv_sec;
			node.data.nsec = tv.tv_usec * 1000;
#endif
			node.type = (int)PROF_TYPE_FUNCTION_EXIT;
			node.data.pid = getpid();
			node.data.func_addr = (PROF_ADDR_TYPE)this_func;

			fwrite(&node, sizeof(node), 1, profile);
		}
	}

	void print_dl_info(void *this_func)
	{
		if (this_func == nullptr)
		{
			return;
		}

		Dl_info DLInfo;
		if (dladdr(this_func, &DLInfo) != 0)
		{
			YOUT("dladdr dli_fname=[%s] dli_fbase=[%p] dli_sname=[%s] dli_saddr=[%p]",
				DLInfo.dli_fname, DLInfo.dli_fbase, DLInfo.dli_sname, DLInfo.dli_saddr);
		}
	}

	void before_main()
	{
		if (profile == nullptr)
		{
			profile = fopen("./prof.dat", "w");
			if (profile == nullptr)
			{
				exit(0);
			}

			dump_maps();
		}
	}

	void after_main()
	{
		if (profile != nullptr)
		{
			fclose(profile);
			profile = 0;
		}
	}

	size_t dump_data(int type, const char* str, size_t size, FILE* file)
	{
		if (file == nullptr || str == nullptr)
		{
			return -1;
		}

		std::string str_write(sizeof(PROF_NODE_TYPE) + sizeof(PROF_SIZE_TYPE) + size, 0);
		char* p = &str_write[0];

		//type
		*((PROF_NODE_TYPE*)p) = (PROF_NODE_TYPE)type;
		p += sizeof(PROF_NODE_TYPE);

		//size
		*((PROF_SIZE_TYPE*)p) = (PROF_SIZE_TYPE)size;
		p += sizeof(PROF_SIZE_TYPE);

		//data
		memcpy(p, str, size);

		return fwrite(&str_write[0], sizeof(char), str_write.size(), file);
	}

	void dump_maps()
	{
		try
		{
			std::string strmaps;
			FILE* pf = fopen("/proc/self/maps", "r");
			if (pf != NULL)
			{
				const size_t buffer_size = 256;
				char buf[buffer_size] = { '\0' };
				size_t size = 0;

				while (size = fread(buf, buffer_size, 1, pf))
				{
					strmaps.append(buf, size);
				}

				fclose(pf);
			}

			dump_data(PROF_TYPE_MAPSFILE, strmaps.c_str(), strmaps.size(), profile);
		}
		catch (...)
		{
			YERR(COMMON_FATAL_ERROR);
		}
	}
#ifdef __cplusplus
}
#endif