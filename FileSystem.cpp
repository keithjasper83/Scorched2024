
#include <string>
#include <fstream>

class FileSystem {
public:
	static std::string fileExists(std::string FileName) {
		std::ifstream file(FileName);
		//file.good();
		if (file.good()) {
			return FileName;
		}
		else {
			printf("FileSystem - Non-Fatal Error: File %s does not exist\n", FileName.c_str());
			return "FILENOTFOUND";
		}
	}
};