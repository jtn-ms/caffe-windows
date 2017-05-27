#include <string>
#include <vector>

__declspec(dllexport) void InitializeClassifier(std::string model_file, std::string trained_file, std::string mean_file, std::string label_file);
__declspec(dllexport) std::vector<std::string> recognizeImage(unsigned char * data, int width, int height);
__declspec(dllexport) void InitializeDetector(std::string model_file, std::string trained_file, std::string label = "", std::string mean_file = "", std::string mean_value = "");
__declspec(dllexport) std::vector<std::vector<float>> detectObject(unsigned char * data,int width, int height);
__declspec(dllexport) std::string getCaption(int classid);