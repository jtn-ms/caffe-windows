
//void InitializeClassifier();
//void InitializeClassifier(const char* model_file, const char* trained_file, const char* mean_file, const char* label_file);
__declspec(dllexport) void InitializeClassifier(std::string model_file, std::string trained_file, std::string mean_file, std::string label_file);
__declspec(dllexport) std::vector<std::string> recognizeImage(unsigned char * data,
												 int width,
												 int height);
