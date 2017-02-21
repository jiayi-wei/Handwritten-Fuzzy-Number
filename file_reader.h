#include <iostream>
#include <fstream>
#include <io.h>

using namespace std;

bool get_filelist(string path,vector<string>&files){
	long hFile=0;
	struct _finddata_t fileinfo;
	files.clear();
	if((hFile=_findfirst(path.c_str(),&fileinfo))!=-1){
		do{
			if(!(fileinfo.attrib&_A_SUBDIR))
				files.push_back(fileinfo.name);
		}
		while(_findnext(hFile,&fileinfo)==0);
		_findclose(hFile);
		return true;
	}
	else
		return false;
}

