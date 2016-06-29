#pragma once

#include "prof_inc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PROF_USE_CPU_TIME 0

	typedef size_t		PROF_NODE_TYPE;
	typedef size_t		PROF_SIZE_TYPE;
	typedef uint16_t	PROF_THREADID_TYPE;
	typedef size_t		PROF_ADDR_TYPE;

#pragma pack(2)
	struct function_node_data
	{
		PROF_THREADID_TYPE      pid;
		uint32_t                sec;
		uint32_t                nsec;
		PROF_ADDR_TYPE          func_addr;
	};

	struct  function_node
	{
		PROF_NODE_TYPE           type;
		function_node_data		 data;

		function_node() : type(0), data() {}
	};


#pragma pack()

//#define YERR(...) fprintf(stderr, ,__VA_ARGS__);
//#define YOUT(...) fprintf(stdout, __VA_ARGS__);

	//函数进出类型
#define PROF_TYPE_FUNCTION_BEGIN         1      //struct function_node_data
#define PROF_TYPE_FUNCTION_EXIT          0      //struct function_node_data

//String Type
#define PROF_STRING_MASK                 (0x1 << 8)        //(u32)length|(string)data

//Mapfile
#define PROF_TYPE_MAPSFILE               (0 | PROF_STRING_MASK)      //(u32)length|(string)Mapfile

//Thread Name
#define PROF_TYPE_THREAD_NAME            (1 | PROF_STRING_MASK)      //(u32)length|(string)Thread Name



	bool dump_thread_name()                                         PROF_API;
	void dump_maps()                                                PROF_API;

	void __cyg_profile_func_enter(void *this_func, void *call_site) PROF_API;
	void __cyg_profile_func_exit(void *this_func, void *call_site)  PROF_API;

	size_t dump_data(int type, const char* str, size_t size, FILE* file)   PROF_API;

	void print_dl_info(void *this_func)				  PROF_API;

	PROF_CONSTRCUCTOR void before_main() PROF_API;
	PROF_DESSTRCUCTOR void after_main()  PROF_API;


#ifdef __cplusplus
}
#endif