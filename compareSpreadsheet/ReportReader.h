#pragma once

#include <string>
#include <map>
#include <stdio.h>

#include "ReportEntity.h"

class ReportReader
{
public:
	ReportReader(void);
	~ReportReader(void);

	static void read(std::string filepath, ReportEntity& entity);

};

