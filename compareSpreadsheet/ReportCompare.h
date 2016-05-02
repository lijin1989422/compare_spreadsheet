#pragma once
#include "ReportEntity.h"

class ReportCompare
{
public:
	typedef std::map<std::string, ReportEntity::ReportRow*> RowMap;

	ReportCompare(void);
	~ReportCompare(void);

	static void compare(ReportEntity& e1, ReportEntity& e2, ReportEntity& eout );

	static void setDelta(double delta) {gDelta=delta;}
	static void setRelDelta(double reldelta) {gRelDelta=reldelta;}
	static double getDelta(void) {return gDelta;}
	static double getRelDelta(void) {return gRelDelta;}

private:
	static double gDelta;
	static double gRelDelta;
};

