#include "stdafx.h"
#include "ReportCompare.h"
#include <map>
#include <string>
#include <iostream>

double ReportCompare::gDelta=0.0;
double ReportCompare::gRelDelta=0.0;

ReportCompare::ReportCompare(void)
{
}


ReportCompare::~ReportCompare(void)
{
}

static void intToCharArr(int x, char* arr)
{
	arr[0] = x & 0xff;
	arr[1] = x>>4 & 0xff;
	arr[2] = x>>8 & 0xff;
	arr[3] = x>>12 & 0xff;
}

static int charArrToInt(char* arr)
{
	int x=0;

	x |= (arr[0]);
	x |= (arr[1]<<4);
	x |= (arr[2]<<8);
	x |= (arr[3]<<12);

	return x;
}

void ReportCompare::makeRowIndex(ReportEntity& entity, KeyMap& keymap, RowMap& rowmap)
{
	for(int i=0; i< entity.getLineCount(); ++i){
		ReportEntity::ReportRow& row = entity.getContentRow(i);

		char buff[64] = {0};
		int buffindex = 0;

		for(int i=0; i<row.size(); ++i){
			if(keymap.find(i) != keymap.end())
			{
				int key = row[i]->getInt();
				intToCharArr(key, buff+buffindex);
				buffindex+=4;

				assert(row[i]->dataType() == DTYPE_INT);
			}
		}


		std::string key(buff);
		rowmap[key] = &row;
	}
}

void ReportCompare::compare(ReportEntity& e1, ReportEntity& e2, ReportEntity& eout )
{
	std::map<std::string, bool> keyname;
	keyname["column1"] = true;
	keyname["column3"] = true;
	keyname["column4"] = true;

	std::map<std::string, bool> numericname;
	numericname["column2"] = true;

	KeyMap keyindex;
	KeyMap numericindex;

	for(int i=0; i< e1.getFieldCount(); ++i){
		std::string name = e1.getFieldName(i);

		if(keyname.find(name) != keyname.end() && keyname[name]){
			keyindex[i] = name;
			std::cout << i <<"=" << name << std::endl;

			// insert into eout
			eout.headerAddField(name+"*");
		}else{
			eout.headerAddField(name+" I");
			eout.headerAddField(name+" II");

			if(numericname.find(name) != numericname.end() && numericname[name]){
				numericindex[i] = name;
				eout.headerAddField("жд");
				eout.headerAddField("ж─");
			}else{
				eout.headerAddField("жд");
			}

		}
	}	

	RowMap rowmap1,rowmap2;
	makeRowIndex(e1, keyindex, rowmap1);
	makeRowIndex(e2, keyindex, rowmap2);

	RowMap mapmatch;
	for(RowMap::iterator iter= rowmap1.begin();
		iter != rowmap1.end(); ++iter){
			std::string key = iter->first;

			if(rowmap2.find(key) == rowmap2.end())
				continue;

			mapmatch[key] = nullptr;

			ReportEntity::ReportRow& row1 = *rowmap1[key];
			ReportEntity::ReportRow& row2 = *rowmap2[key];

			ReportEntity::ReportRow insertrow;
			for(int i=0; i<row1.size(); ++i){
				if(keyindex.find(i) != keyindex.end()){ 
					//key value
					ReportEntity::rowAddField(insertrow, row1[i].get());

				} else {
					DataBase diffData, deltaData;
					diffData.setData(true);
					deltaData.setData(0.0);

					DataBase *d1 = row1[i].get();
					DataBase *d2 = row2[i].get();

					if(d1->dataType() != d2->dataType()){
						diffData.setData(false);

					} else if (d1->dataType() == DTYPE_STRING){
						if(d1->getString() != d2->getString())
							diffData.setData(false);

					}else if (d1->dataType() == DTYPE_INT){
						int diff = abs(d1->getInt()-d2->getInt());
						diffData.setData(diff);
						double delta = diff / (d1->getInt() + d2->getInt());	
						deltaData.setData(delta);

					}else if (d1->dataType() == DTYPE_FLOAT){
						double diff = abs(d1->getFloat()-d2->getFloat());
						diffData.setData(diff);
						double delta = diff / (d1->getFloat() + d2->getFloat());
						deltaData.setData(delta);
					}

					ReportEntity::rowAddField(insertrow, d1);
					ReportEntity::rowAddField(insertrow, d2);

					if(numericindex.find(i) != numericindex.end()){
						ReportEntity::rowAddField(insertrow, diffData);
						ReportEntity::rowAddField(insertrow, deltaData);	
					}else{
						ReportEntity::rowAddField(insertrow, diffData);
					}
				}
			}

		eout.contentAddRow(insertrow);

	}

	for(RowMap::iterator iter= rowmap1.begin();
		iter != rowmap1.end(); ++iter){
			std::string key = iter->first;

			if(mapmatch.find(key) != mapmatch.end())
				continue;

			ReportEntity::ReportRow& row1 = *rowmap1[key];
			ReportEntity::ReportRow insertrow;

			for(int i=0; i<row1.size(); ++i){
				if(keyindex.find(i) != keyindex.end()){ 
					//key value
					ReportEntity::rowAddField(insertrow, row1[i].get());

				} else {
					DataBase diffData, deltaData;
					diffData.setData(false);
					deltaData.setData(0.0);

					DataBase *d1 = row1[i].get();
					if (d1->dataType() == DTYPE_INT){
						int diff = abs(d1->getInt());
						diffData.setData(diff);	
						deltaData.setData(1.0);

					}else if (d1->dataType() == DTYPE_FLOAT){
						double diff = abs(d1->getFloat());
						diffData.setData(diff);
						deltaData.setData(1.0);
					}

					ReportEntity::rowAddField(insertrow, d1);
					ReportEntity::rowAddField(insertrow, NilData);

					if(numericindex.find(i) != numericindex.end()){
						ReportEntity::rowAddField(insertrow, diffData);
						ReportEntity::rowAddField(insertrow, deltaData);	
					}else{
						ReportEntity::rowAddField(insertrow, diffData);
					}
				}
			}

		eout.contentAddRow(insertrow);
	}

	for(RowMap::iterator iter= rowmap2.begin();
		iter != rowmap2.end(); ++iter){
			std::string key = iter->first;

			if(mapmatch.find(key) != mapmatch.end())
				continue;

			ReportEntity::ReportRow& row2 = *rowmap2[key];
			ReportEntity::ReportRow insertrow;

			for(int i=0; i<row2.size(); ++i){
				if(keyindex.find(i) != keyindex.end()){ 
					//key value
					ReportEntity::rowAddField(insertrow, row2[i].get());

				} else {
					DataBase diffData, deltaData;
					diffData.setData(false);
					deltaData.setData(0.0);

					DataBase *d2 = row2[i].get();
					if (d2->dataType() == DTYPE_INT){
						int diff = abs(d2->getInt());
						diffData.setData(diff);	
						deltaData.setData(1.0);

					}else if (d2->dataType() == DTYPE_FLOAT){
						double diff = abs(d2->getFloat());
						diffData.setData(diff);
						deltaData.setData(1.0);
					}

					ReportEntity::rowAddField(insertrow, NilData);
					ReportEntity::rowAddField(insertrow, d2);					

					if(numericindex.find(i) != numericindex.end()){
						ReportEntity::rowAddField(insertrow, diffData);
						ReportEntity::rowAddField(insertrow, deltaData);	
					}else{
						ReportEntity::rowAddField(insertrow, diffData);
					}
				}
			}

		eout.contentAddRow(insertrow);
	}
}
