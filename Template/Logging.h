#pragma once
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <filesystem>

namespace LogPS {
	std::ofstream LogFile;
	std::string directory = "Logs";
	std::string fileName = "Log";
	std::string file = "";
	std::string extension = ".txt";
	bool hasFile = false;

	bool CheckFile(std::string& fileName, std::time_t timeStamp) {
		if (!std::filesystem::create_directories(directory)) {
			LogFile.open("ERROR_COULD_NOT_CREATE_LOGS_FOLDER.txt");
			LogFile << "Could not create <->" + directory + "<-> directory. Please create manually";
			LogFile.close();
		}


		tm* ltm = localtime(&timeStamp);
		std::string temp = directory + "/" + fileName + "." + std::to_string(ltm->tm_mday) + "." + std::to_string(1 + ltm->tm_mon) + "." + std::to_string(1900 + ltm->tm_year) + ".0";
		while (std::filesystem::exists(temp + extension)) {
			temp[temp.size() - 1] += 1;
		}
		file = temp + extension;
		hasFile = true;
		return true;
	}
}

void Log(std::string logData) {
	std::time_t timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	if (!LogPS::hasFile) {
		LogPS::CheckFile(LogPS::fileName, timeStamp);
	}
	else if (!std::filesystem::exists(LogPS::file)) {
		LogPS::hasFile = false;
		LogPS::CheckFile(LogPS::fileName, timeStamp);
	}


	LogPS::LogFile.open(LogPS::file, std::ofstream::app);

	LogPS::LogFile << logData << "\n";

	LogPS::LogFile.close();
}
