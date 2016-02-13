#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.02";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 2;
	static const long BUILD  = 201;
	static const long REVISION  = 1100;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 356;
	#define RC_FILEVERSION 1,2,201,1100
	#define RC_FILEVERSION_STRING "1, 2, 201, 1100\0"
	static const char FULLVERSION_STRING [] = "1.2.201.1100";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
