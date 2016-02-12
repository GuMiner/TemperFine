#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.02";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 48;
	static const long REVISION  = 252;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 156;
	#define RC_FILEVERSION 1,0,48,252
	#define RC_FILEVERSION_STRING "1, 0, 48, 252\0"
	static const char FULLVERSION_STRING [] = "1.0.48.252";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 48;
	

}
#endif //VERSION_H
