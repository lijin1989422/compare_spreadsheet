#pragma once
#include"ReportData.h"

class ReportEntity
{
public:
	typedef std::map<int, std::string> ReportHeader;
	typedef std::shared_ptr<DataBase> shareData;
	typedef std::vector<shareData> ReportRow;
	typedef std::vector< ReportRow > Content;
	

public:
	ReportEntity(void);
	~ReportEntity(void);

	void init(void);
	void headerAddField(std::string field);
	void contentAddRow(ReportRow row);
	template<class T>
	static void rowAddField(ReportRow &row, const T& data){
		row.push_back(shareData(new DataBase(data)));
	}

	int getFieldCount(void);
	std::string getFieldName(int index);
	int getLineCount(void);
	ReportRow& getContentRow(int line);

	static void setSortType(int tTag, bool vAsc);
	static void getSortType(int& tTag, bool& vAsc);
	static bool compareRow(const ReportRow& row1, const ReportRow& row2);

	void save(std::string filepath);
	void print(void);

public:
	Content m_content;

private:
	int m_column_count;
	int m_line_count;
	ReportHeader m_header;

	static int gSortTag;
	static bool gSortAsc;

};

