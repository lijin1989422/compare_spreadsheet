#pragma once

#include <string>

enum EM_DATA_TYPE {
	DTYPE_BASE,
	DTYPE_INT,
	DTYPE_FLOAT,
	DTYPE_STRING,
	DTYPE_BOOL,
	DTYPE_NIL
};

class DataBase
{
public:
	DataBase(void);
	virtual ~DataBase(void);

	template<class T>
	DataBase(const T& data){
		setData(data);
	}

	DataBase(const DataBase& data){
		setData(data);
	}

	int dataType(void);

	void setData(const DataBase *data);
	void setData(const DataBase &data);
	void setData(int data);
	void setData(double data);
	void setData(std::string data);
	void setData(bool data);
	void setData(void);

	int getInt(void);
	double getFloat(void);
	std::string getString(void);
	bool getBool(void);

	std::string toString(void);
	std::string toStringEx(void);
	bool compare(DataBase *data);
	DataBase diffrence(DataBase *data);

	void init(void);

private:

	int m_intData;
	double m_floatData;
	std::string m_strData;
	bool m_boolData;

	int m_dataType;
};

extern DataBase NilData;