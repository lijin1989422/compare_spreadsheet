#include "stdafx.h"
#include "ReportReader.h"
#include "ReportEntity.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>

#define LINE_BUFF_LEN 2048

ReportReader::ReportReader(void)
{
}


ReportReader::~ReportReader(void)
{
}

bool checkInt(const char * str)
{
	if(!str || str[0] == '\0') 
		return false;

	char* endpos;
	int i = strtol(str, &endpos, 10);
	return *endpos=='\0';
}

bool checkFloat(const char * str)
{
	if(!str || str[0] == '\0') 
		return false;

	char* endpos;
	double d = strtod(str, &endpos);
	return *endpos=='\0';
}

bool checkNil(const char * str){
	std::string nilstr = "#NA";
	return nilstr== str;
}


int convertStr2Int(const char * str)
{
	char* endpos;
	int i = strtol(str, &endpos, 10);
	if(*endpos=='\0'){
		return i;
	}

	return 0;
}

double convertStr2Float(const char * str)
{
	char* endpos;
	double d = strtod(str, &endpos);
	if(*endpos=='\0'){
		return d;
	}

	return 0;
}


void test(const char * str){
	bool bInt = checkInt(str);
	bool bFloat = checkFloat(str);

	std::cout << "str: " << str << std::endl;

	if(bInt){
		int i = convertStr2Int(str);
		std::cout << "int: " << i << std::endl;
	}

	else if(bFloat){
		double f = convertStr2Float(str);
		std::cout << "float: "<< std::fixed << f << std::endl;
	}

}

void split(std::string& s, std::string delim,std::vector< std::string >& ret)  
{  
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		ret.push_back(s.substr(last,index-last));  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	
	ret.push_back(s.substr(last));  
}

void ReportReader::read(std::string filepath, ReportEntity& entity)
{
	std::cout <<filepath<<std::endl;

	static char buff[LINE_BUFF_LEN] = {0};	
	std::ifstream fs(filepath);

	//read header
	if(fs.good())
	{
		memset(buff, 0, sizeof(buff));
		fs.getline(buff, LINE_BUFF_LEN);
		std::string header(buff);

		std::vector< std::string > ret;
		split(header, ";", ret);


		entity.headerAddField("Line num");
		for(std::vector< std::string>::iterator iter = ret.begin();
			iter != ret.end(); ++iter)
		{
			//std::cout << *iter << "|";
			entity.headerAddField(*iter);
		}

		//std::cout << std::endl;
	}

	int linenum = 0;
	while(fs.good())
	{
		memset(buff, 0, sizeof(buff));
		fs.getline(buff, LINE_BUFF_LEN);
		std::string content(buff);

		linenum +=1;
		ReportEntity::ReportRow row;
		ReportEntity::rowAddField(row, linenum);
		//row.push_back(ReportEntity::shareRow(new DataBase(linenum)));

		std::vector< std::string > ret;
		split(content, ";", ret);
		for(std::vector< std::string>::iterator iter = ret.begin();
			iter != ret.end(); ++iter)
		{
			const char *char_str = iter->c_str();
			if(checkInt(char_str)){
				int i = convertStr2Int(char_str);
				ReportEntity::rowAddField(row, i);

			}else if(checkFloat(char_str)){
				double d = convertStr2Float(char_str);
				ReportEntity::rowAddField(row, d);

			} else if(checkNil(char_str)){
				ReportEntity::rowAddField(row, NilData);

			} else {
				ReportEntity::rowAddField(row, *iter);
			}

			//std::cout << *iter << "|";
		}

		entity.contentAddRow(row);
		//std::cout << std::endl;
	}
}
/*
int ReportReader::get_column_index(std::string colunm_name)
{
	int index = -1;
	for(ReportHeader::iterator iter = m_header.begin(); 
		iter != m_header.end(); ++iter)
	{
		if(iter->second == colunm_name)
		{
			index = iter->first;
			break;
		}
	}

	return index;
}

std::string ReportReader::get_column_name(int index)
{
	std::string name;
	ReportHeader::iterator iter = m_header.find(index);
	if(iter != m_header.end()){
		name = iter->second;
	}

	return name;
}*/