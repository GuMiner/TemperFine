#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.02";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 15;
	static const long REVISION  = 88;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 99;
	#define RC_FILEVERSION 1,0,15,88
	#define RC_FILEVERSION_STRING "1, 0, 15, 88\0"
	static const char FULLVERSION_STRING [] = "1.0.15.88";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
