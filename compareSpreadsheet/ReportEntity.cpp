#include "stdafx.h"
#include "ReportEntity.h"
#include <map>
#include <iostream>
#include <fstream>

int ReportEntity::gSortTag = 0;
bool ReportEntity::gSortAsc = false;

ReportEntity::ReportEntity(void)
{
	this->init();
}

ReportEntity::~ReportEntity(void)
{
}


void ReportEntity::init(void)
{
	this->m_line_count=0;
	this->m_column_count=0;
	this->m_header.clear();
	this->m_content.clear();
}


void ReportEntity::headerAddField(std::string field)
{
	this->m_header[m_column_count] = field;
	this->m_column_count+=1;
}

void ReportEntity::contentAddRow(ReportRow row)
{
	this->m_content.push_back(row);
	this->m_line_count+=1;
}

void ReportEntity::print(void)
{
	for(ReportHeader::iterator iter = this->m_header.begin();
		iter != this->m_header.end(); ++iter)
	{
		std::cout << iter->first <<" " << iter->second << " |";
	}

	std::cout << std::endl;

	for(Content::iterator iter = this->m_content.begin();
		iter != this->m_content.end(); ++iter)
	{
		ReportRow& row = *iter;

		for(ReportRow::iterator iter_r = row.begin();
			iter_r != row.end(); ++ iter_r)
		{
			std::cout << iter_r->get()->toStringEx() << " |";
		}

		std::cout << std::endl;

	}
}

void ReportEntity::save(std::string filepath)
{
	using std::locale;
	//locale &loc=locale::global(locale(locale(),"",LC_CTYPE));
	locale &loc=locale::global(locale("chs", locale::ctype));

	std::ofstream fs(filepath, std::ios::out|std::ios::binary);

	
	for(ReportHeader::iterator iter = this->m_header.begin();
		iter != this->m_header.end(); ++iter)
	{
		const char * header_colname = iter->second.c_str();
		fs << header_colname;

		ReportHeader::iterator iter_check = iter;
		if(++iter_check == this->m_header.end()){
			fs << std::endl;
		}else{
			fs << ",";
		}
	}	

	for(Content::iterator iter = this->m_content.begin();
		iter != this->m_content.end(); ++iter)
	{
		ReportRow& row = *iter;

		for(ReportRow::iterator iter_r = row.begin();
			iter_r != row.end(); ++ iter_r)
		{
	
			fs << iter_r->get()->toString();
			if(iter_r+1 == row.end()){
				fs << std::endl;
			}else{
				fs << ",";
			}
		}

	}

	locale::global(loc);
	fs.close();
};

int ReportEntity::getFieldCount(void)
{
	return this->m_column_count;
}

std::string ReportEntity::getFieldName(int index){
	if(index < 0 || index > this->m_column_count)
		return "OutOfIndex";

	return this->m_header[index];
}

int ReportEntity::getLineCount(void)
{
	return this->m_line_count;
}

ReportEntity::ReportRow& ReportEntity::getContentRow(int line)
{
	assert(line >= 0 || line <= this->m_line_count);
	return this->m_content[line];
}

void ReportEntity::setSortType(int tTag, bool vAsc)
{		
	gSortTag = tTag;
	gSortAsc = vAsc;	
}

void ReportEntity::getSortType(int& tTag, bool& vAsc)
{
	tTag = gSortTag;
	vAsc = gSortAsc;
}

bool ReportEntity::compareRow(const ReportRow& row1, const ReportRow& row2)
{
	int maxn=row1.size();
	int sortTag;
	bool sortAsc;
	ReportEntity::getSortType(sortTag, sortAsc);
	if(sortTag >= maxn)
		sortTag = 0;

	DataBase *d1;
	DataBase *d2;

	if(sortAsc){
		d1 = row1[sortTag].get();
		d2 = row2[sortTag].get();
	}else{
		d2 = row1[sortTag].get();
		d1 = row2[sortTag].get();
	}


	if(d1->dataType() != d2->dataType()){
		return d1->dataType() < d2->dataType();

	} else if(d1->dataType() == DTYPE_INT){
		return d1->getInt() < d2->getInt();			

	} else if(d1->dataType() == DTYPE_FLOAT){
		return d1->getFloat() < d2->getFloat();			

	}else if(d1->dataType() == DTYPE_STRING){
		return d1->getString() < d2->getString();	

	}else if(d1->dataType() == DTYPE_BOOL){
		return d1->getBool() < d2->getBool();	

	}

	return false;
}
