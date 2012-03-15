#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 122;
	static const long REVISION = 722;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 135;
	#define RC_FILEVERSION 1,1,122,722
	#define RC_FILEVERSION_STRING "1, 1, 122, 722\0"
	static const char FULLVERSION_STRING[] = "1.1.122.722";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 22;
	

}
#endif //VERSION_H