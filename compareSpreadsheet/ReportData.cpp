#include "stdafx.h"
#include "ReportData.h"
#include <iostream>

DataBase NilData;

DataBase::DataBase(void)
{
	this->init();
}

DataBase::~DataBase(void)
{
	//std::cout << "~" <<this->toString() << std::endl;
}

int DataBase::dataType(void)
{
	return m_dataType;
}

void DataBase::setData(const DataBase *data)
{
	this->m_intData = data->m_intData;
	this->m_floatData = data->m_floatData;
	this->m_strData  = data->m_strData;
	this->m_boolData = data->m_boolData;
	this->m_dataType = data->m_dataType;
}

void DataBase::setData(const DataBase& data)
{
	*this = data;
}

void DataBase::setData(int data)
{
	this->init();
	this->m_dataType = DTYPE_INT;
	this->m_intData = data;
}

void DataBase::setData(double data)
{
	this->init();
	this->m_dataType = DTYPE_FLOAT;
	this->m_floatData = data;
}
void DataBase::setData(std::string data)
{
	this->init();
	this->m_dataType = DTYPE_STRING;
	this->m_strData = data;
}
void DataBase::setData(bool data)
{
	this->init();
	this->m_dataType = DTYPE_BOOL;
	this->m_boolData = data;
}
void DataBase::setData(void)
{
	this->init();
	this->m_dataType = DTYPE_NIL;
}

int DataBase::getInt(void)
{
	return m_intData;
}

double DataBase::getFloat(void)
{
	return m_floatData;
}


std::string DataBase::getString(void)
{
	return m_strData;
}


bool DataBase::getBool(void)
{
	return m_boolData;
}

std::string DataBase::toString(void)
{
	char buff[32] = {0};
	std::string str;

	switch(m_dataType){
	case DTYPE_BASE:
		sprintf(buff, "BASE_OBJ");
		str = buff;
		break;
	case DTYPE_INT:
		sprintf(buff, "%d", m_intData);
		str = buff;
		break;
	case DTYPE_FLOAT:
		sprintf(buff, "%.6f", m_floatData);
		str = buff;
		break;
	case DTYPE_BOOL:
		sprintf(buff, "%s", m_boolData ? "true":"false" );
		str = buff;
		break;
	case DTYPE_NIL:
		sprintf(buff, "N/A" );
		str = buff;
		break;
	case DTYPE_STRING:
		str = m_strData;
		break;
	}

	return str;
}

std::string DataBase::toStringEx(void)
{
	char buff[32] = {0};
	std::string str;

	switch(m_dataType){
	case DTYPE_BASE:
		sprintf(buff, "BASE_OBJ");
		str = buff;
		break;
	case DTYPE_INT:
		sprintf(buff, "(int)%d", m_intData);
		str = buff;
		break;
	case DTYPE_FLOAT:
		sprintf(buff, "(float)%f", m_floatData);
		str = buff;
		break;
	case DTYPE_BOOL:
		sprintf(buff, "(bool)%s", m_boolData ? "true":"false" );
		str = buff;
		break;
	case DTYPE_NIL:
		sprintf(buff, "N/A" );
		str = buff;
		break;
	case DTYPE_STRING:
		str = "(string)"+m_strData;
		break;
	}

	return str;
}

bool DataBase::compare(DataBase *data)
{
	if(this->m_dataType != data->m_dataType){
		return false;
	}

	bool b = false;

	switch(m_dataType){
	case DTYPE_BASE:
		b = true;
		break;
	case DTYPE_INT:
		b = this->m_intData == data->m_intData;
		break;
	case DTYPE_FLOAT:
		if((int)this->m_floatData == (int)data->m_floatData){
			b = true;
		}else{
			double sum = abs(data->m_floatData + this->m_floatData);
			double diff = abs(data->m_floatData - this->m_floatData);

			if (sum != 0 && diff/sum < 0.05){
				b = true;
			}
		}
		break;
	case DTYPE_BOOL:
		b = this->m_boolData == data->m_boolData;
		break;
	case DTYPE_NIL:
		b = true;
		break;
	case DTYPE_STRING:
		b = this->m_strData == data->m_strData;
		break;
	}

	return b;
}

DataBase DataBase::diffrence(DataBase *data)
{
	DataBase rdata;

	if(this->m_dataType != data->m_dataType){
		rdata.setData(false);
		return rdata;
	}

	switch(this->m_dataType){
		case DTYPE_INT:
			rdata.setData(this->m_intData - data->m_intData);
			break;
		case DTYPE_FLOAT:
			rdata.setData(this->m_floatData - data->m_floatData);
			break;
		default:
			rdata.setData(this->compare(data));
	}

	return rdata;

}

void DataBase::init(void)
{
	m_intData = 0;
	m_floatData = 0.0f;
	m_strData  = "";
	m_boolData = false;
	m_dataType = DTYPE_NIL;
}