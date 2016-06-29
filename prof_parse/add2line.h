#pragma once

#include "prof.h"

class addr2line
{
	struct addr_bounds
	{
		PROF_ADDR_TYPE start_addr;
		PROF_ADDR_TYPE end_addr;
		addr_bounds(PROF_ADDR_TYPE StartAddr = 0,
			PROF_ADDR_TYPE EndAddr = 0)
			:start_addr(StartAddr),
			end_addr(EndAddr)
		{}
	};

public:
	addr2line();
	explicit addr2line(const std::string& exec_name, const std::string& so_search_path = "", const std::string& maps = "");

	bool set_exec(const std::string& exec_name);
	bool set_so_search_path(const std::string& path);
	bool set_toolset_prefix(const std::string& toolset);
	bool load_maps(const std::string& maps);

	std::string getline(PROF_ADDR_TYPE addr);

protected:
	std::string get_addr_locate(PROF_ADDR_TYPE addr);
	PROF_ADDR_TYPE   get_addr_in_object(PROF_ADDR_TYPE addr);

private:
	typedef std::map<std::string, addr_bounds> ADDR_BOUNDS_MAP;
	ADDR_BOUNDS_MAP		map_addr_bounds;//´æ·ÅÇø¶ÎµØÖ·
	std::string			exec_name;
	std::string			so_search_path;
	std::string			toolset;
};