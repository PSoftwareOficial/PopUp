#pragma once
#ifndef OLC_PGE_APPLICATION
//	#define OLC_PGE_APPLICATION
//	#include "olcPixelGameEngine.h"
#endif
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <algorithm>
#include "Logging.h"


bool ToDouble(std::string value, double &result) {
	try {
		result = std::stod(value);
		return true;
	}
	catch(std::invalid_argument e) {
		return false;
	}
}
bool ToFloat(std::string value, float& result) {
	try {
		result = std::stof(value);
		return true;
	}
	catch (std::invalid_argument e) {
		return false;
	}
}
bool ToInt(std::string value, int& result) {
	try {
		result = std::stoi(value);
		return true;
	}
	catch (std::invalid_argument e) {
		return false;
	}
}








std::vector<std::string> GetAllFiles(std::string& directory) {
	std::vector<std::string> tempDirEntrys;
	tempDirEntrys.reserve(10);
	try {
		for (auto const& e : std::filesystem::directory_iterator(directory)) {
			tempDirEntrys.push_back(e.path().filename().generic_string());
		}
	}
	catch (std::filesystem::filesystem_error error) {
		Log("No such directory : " + directory);
	}
	tempDirEntrys.shrink_to_fit();
	return tempDirEntrys;
}

inline bool is_not_alnum_space(char c)
{
	return !(isalpha(c) || isdigit(c) || (c == ' ') || (c == '-') || (c == '.'));
}
inline bool is_not_num(char c)
{
	return !(isdigit(c) || (c == ' ') || (c == '-') || (c == '.'));
}


namespace InputError{
	const std::string Retry = "Retry&?!";
const std::string DoubleError = "Could not convert to double";
const std::string IntegerError = "Could not convert to integer";
const std::string FloatError = "Could not convert to float";
const std::string ErrorNonNum = "Contains Nonnumeric characters";


}

enum InputType {
	NOINPUT,
	SAVEFILE,
	VALUE,
	SELECTFILE,
	SELECTOPTIONS,

};
enum FilterOption {
	NOFILTER,
	EXTENSION,
	INCLUDES
};
enum KeyBoard {
	NOKEY,
NUMERIC,
ALPHABETIC,
ARROW,
ALPHANUMERIC,
ALPHANUMARR
};

struct Format {
	InputType inputType = NOINPUT;
	FilterOption filterOption = NOFILTER;
	KeyBoard keyBoard = NOKEY;
};

class Input {
public:
	inline std::vector<std::string>& GetOptions() {
		return variable;
	}
	virtual void SetOptions(const std::vector<std::string> opt) {
		variable = opt;
	}
	inline std::string& GetTitle() {
		return title;
	}
	inline Format GetFormat() {
		return format;
	}
	virtual bool ConvertValue(std::string& errorMessage) {
		return true;
	}

	virtual bool StartUp() {
		return true;
	}
	inline void SetTitle(const std::string newTitle) {
		title = newTitle;
	}

	std::string result = "";
	int selection = 0;


	static void SetValue(std::shared_ptr<Input> input, std::string value) {
		input->result = value;

		std::string errorMessage = "";
		if (!input->ConvertValue(errorMessage)) {
			Log("Error: Input: calculating: " + errorMessage + " : SetValue: " + value);
		}
	}
protected:
	Format format;
	
	//Check if correct result, if not says
	std::string filterValue = "";
	std::string title = "";
	//A string for variable data. If multiple data needs to be accessed, “;:;” are considered separators
	std::vector<std::string> variable;


};


class InputString : public Input {
public:
	InputString() {
		format.keyBoard = ALPHANUMERIC;
		format.inputType = VALUE;
	}

	//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) override {
			value = result;
			return true;
	}
	std::string value = "";

	void SetValue(std::string x) {
		value = x;
		result = x;
	}
};

class InputDouble : public Input{
public:
	InputDouble() {
		format.keyBoard = NUMERIC;
		format.inputType = VALUE;
	}

//Check if correct result, if not says
virtual bool ConvertValue(std::string & errorMessage) override {
	try{
	value = std::stod(result);
	result = std::to_string(value);
	return true;
	} catch (std::invalid_argument e) {
		errorMessage = InputError::DoubleError;
		if (!(std::find_if(result.begin(), result.end(), is_not_num) == result.end())) {
			errorMessage += "\n" + InputError::ErrorNonNum;
		}
		return false;
	}

}
double value = 0;

void SetValue(double x) {
	value = x;
	result = std::to_string(x);
}
};

class InputFloat : public Input {
public:
	InputFloat() {
		format.keyBoard = NUMERIC;
		format.inputType = VALUE;
	}

	//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) override {
		try {
			value = std::stof(result);
			result = std::to_string(value);
			return true;
		}
		catch (std::invalid_argument e) {
			errorMessage = InputError::FloatError;
			if (!(std::find_if(result.begin(), result.end(), is_not_num) == result.end())) {
				errorMessage += "\n" + InputError::ErrorNonNum;
			}
			return false;
		}

	}
	float value = 0;
	void SetValue(float x) {
		value = x;
		result = std::to_string(x);
	}
};

class InputInteger : public Input {
public:
	InputInteger() {
		format.keyBoard = NUMERIC;
		format.inputType = VALUE;
	}

	//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) override {
		try {
			value = std::stoi(result);
			result = std::to_string(value);
			return true;
		}
		catch (std::invalid_argument e) {
			errorMessage = InputError::IntegerError;
			if (!(std::find_if(result.begin(), result.end(), is_not_num) == result.end())) {
				errorMessage += "\n" + InputError::ErrorNonNum;
			}
			return false;
		}

	}
	int value = 0;

	void SetValue(int x) {
		value = x;
		result = std::to_string(x);
	}

};


class InputSelecOpt : public Input{
public:
	InputSelecOpt() {
format.keyBoard = KeyBoard::ARROW;
format.inputType = SELECTOPTIONS;
}

//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) {
		if (selection >= 0 && selection < variable.size()) {
			value = variable[selection];
			result = value;
			return true;
		}
		errorMessage = "Error: Could not select option: Was a Window opened?";
		return false;
	}

	void SetOptions(std::vector<std::string> &options) {
		variable = options;
	}
std::string value;
void SetValue(std::string x) {
	value = x;
	result = x;
}

};

class InputSelecFiles : public Input {
public:
	InputSelecFiles(){
		*this = InputSelecFiles(".");
	}
	InputSelecFiles(std::string directory) : directory(directory) {
		format.keyBoard = KeyBoard::ARROW;
		format.inputType = SELECTFILE;
	}
	//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) {
			//Check if is File
			if (std::filesystem::is_directory(directory + "/" + result)) {
				errorMessage = InputError::Retry;
				if (result == "..") {
					auto it = directory.find_last_of('/');
					if (it == std::string::npos) {
						directory = std::filesystem::current_path().generic_string();
						it = directory.find_last_of('/');
					}
					directory.erase(it);
				}
				else {
					directory = directory + "/" + result;
				}
				return false;
			}
			else if (std::filesystem::exists(directory + "/" + result)) {
				value = directory + "/" + result;
				result = value;
				return true;
			}
			else {
				errorMessage = "Error: Could not select file: File does not exists.";
				return false;
			}
		errorMessage = "Error: Could not select file: Was a Window opened?";
		return false;
	}
	virtual bool StartUp() override {
		SetOptions(GetAllFiles(directory));
		return true;
	}
	std::string value;
	void SetValue(std::string x) {
		value = x;
		result = x;
	}

	inline virtual void SetOptions(const std::vector<std::string> opt) override {
		variable = opt;
		variable.insert(variable.begin(), "..");
	}
protected:
	std::string directory;
	
};
class InputSaveFile : public InputSelecFiles {
public:
	InputSaveFile() {
		*this = InputSaveFile(".");
	}
	InputSaveFile(std::string directory) : InputSelecFiles(directory) {
		format.keyBoard = KeyBoard::ALPHANUMARR;
		format.inputType = InputType::SAVEFILE;
	}
	//Check if correct result, if not says
	virtual bool ConvertValue(std::string& errorMessage) {
			//Check if is File
		//Check if is File
		if (std::filesystem::is_directory(directory + "/" + result)) {
			errorMessage = InputError::Retry;
			if (result == "..") {
				auto it = directory.find_last_of('/');
				if (it == std::string::npos) {
					directory = std::filesystem::current_path().generic_string();
					it = directory.find_last_of('/');
				}
				directory.erase(it);
			}
			else {
				directory = directory + "/" + result;
			}
			return false;
		}
		else {
			value = result[0] == '.' ? result : directory + "/" + result;
			result = value;
			return true;
		}
		errorMessage = "Error: Could not save file: Was a Window opened?";
		return false;
	}
	
};


namespace PopUp {

	enum Status : uint8_t {
		ACTIVE,
		IDLE,
		WARNING,
		RETURN
	};




	class System {
	public:
		System(olc::PixelGameEngine* GE) : GE(GE), input(std::make_shared<Input>()),currentStatus(IDLE) {}

		void Set(std::shared_ptr<Input> inputPara) {
			input = inputPara;
			currentStatus = ACTIVE;
			cursorPosition = 0;
		}
		void Set(std::string errorMessage) {
			DisplayNote(errorMessage);
			currentStatus = WARNING;
			cursorPosition = 0;
		}

		void SetIdle() {
			input = nullptr;
			currentStatus = IDLE;
		}

		bool Update();
		inline Status GetStatus() {
			return currentStatus;
		}
	private:
		bool ShowOptions(bool AllowValue = false);
		void DisplayOptions(std::vector<std::string>& entrys, bool AllowValue = false);
		void DisplayValue();
		void DisplayNote(std::string& errorMessage);
		olc::PixelGameEngine* GE;
		std::shared_ptr<Input> input = nullptr;
		std::string directory;
		Status currentStatus = IDLE;
		bool GetKeyValue();
		bool AlphaKeys();
		bool NumKeys();
		bool ArrKeys();
		std::string errorMessage = "";

		uint16_t cursorPosition = 0;

		static const std::vector<std::tuple<olc::Key, std::string, std::string>> Alpha;
		static const std::vector<std::tuple<olc::Key, std::string, std::string>> Num;
		static const std::vector<std::tuple<olc::Key, std::string, std::string>> NumExtra;
		static const std::vector<std::tuple<olc::Key>> Arr;
	};

	const std::vector<std::tuple<olc::Key, std::string, std::string>> System::Alpha =
	{ { olc::Key::A, "a", "A" }, { olc::Key::B, "b", "B" }, { olc::Key::C, "c", "C" }, { olc::Key::D, "d", "D" }, { olc::Key::E, "e", "E" },
	{ olc::Key::F, "f", "F" }, { olc::Key::G, "g", "G" }, { olc::Key::H, "h", "H" }, { olc::Key::I, "i", "I" }, { olc::Key::J, "j", "J" },
	{ olc::Key::K, "k", "K" }, { olc::Key::L, "l", "L" }, { olc::Key::M, "m", "M" }, { olc::Key::N, "n", "N" }, { olc::Key::O, "o", "O" },
	{ olc::Key::P, "p", "P" }, { olc::Key::Q, "q", "Q" }, { olc::Key::R, "r", "R" }, { olc::Key::S, "s", "S" }, { olc::Key::T, "t", "T" },
	{ olc::Key::U, "u", "U" }, { olc::Key::V, "v", "V" }, { olc::Key::W, "w", "W" }, { olc::Key::X, "x", "X" }, { olc::Key::Y, "y", "Y" },
	{ olc::Key::Z, "z", "Z" },
	{olc::Key::NP_MUL, "*", "*"}, {olc::Key::NP_DIV, "/", "/"}, {olc::Key::NP_ADD, "+", "+"}, {olc::Key::NP_SUB, "-", "-"},	{olc::Key::NP_DECIMAL, ".", "."},

	{olc::Key::PERIOD, ".", ":"}, {olc::Key::COMMA, ",", ";"}, {olc::Key::MINUS, "-", "_"}, {olc::Key::SPACE, " ", " "}
	};

	const std::vector<std::tuple<olc::Key, std::string, std::string>> System::Num = {
		{olc::Key::K0, "0", "="}, {olc::Key::K1, "1", "!"}, {olc::Key::K2, "2", "\""}, {olc::Key::K3, "3", "#"},	{olc::Key::K4, "4", "$"},
		{olc::Key::K5, "5", "%"}, {olc::Key::K6, "6", "&"}, {olc::Key::K7, "7", "/"}, {olc::Key::K8, "8", "("},	{olc::Key::K9, "9", ")"},

		{olc::Key::NP0, "0", "0"}, {olc::Key::NP1, "1", "1"}, {olc::Key::NP2, "2", "2"}, {olc::Key::NP3, "3", "3"},	{olc::Key::NP4, "4", "4"},
		{olc::Key::NP5, "5", "5"}, {olc::Key::NP6, "6", "6"}, {olc::Key::NP7, "7", "7"}, {olc::Key::NP8, "8", "8"},	{olc::Key::NP9, "9", "9"},
	};
	const std::vector<std::tuple<olc::Key>> System::Arr = {
		{olc::Key::RIGHT}, {olc::Key::LEFT},{olc::Key::UP},{olc::Key::DOWN}
	};

	const std::vector<std::tuple<olc::Key, std::string, std::string>> System::NumExtra = {
		{ olc::Key::PERIOD, ".", ":" }, { olc::Key::MINUS, "-", "_" } 
	};





	bool System::AlphaKeys() {
		if (GE->IsFocused()) {
			for (const auto& key : Alpha) {
				if (GE->GetKey(std::get<0>(key)).bPressed)
				{
					(input->result).insert(cursorPosition, GE->GetKey(olc::Key::SHIFT).bHeld ? std::get<2>(key) : std::get<1>(key));
					cursorPosition++;
				}
			}
		}
		return true;
	}
	bool System::NumKeys() {
		if (GE->IsFocused()) {
			for (const auto& key : Num) {
				if (GE->GetKey(std::get<0>(key)).bPressed)
				{
					(input->result).insert(cursorPosition, std::get<1>(key));
					cursorPosition++;
				}
			}
			if (input->GetFormat().keyBoard == NUMERIC) {
				for (const auto& key : NumExtra) {
					if (GE->GetKey(std::get<0>(key)).bPressed)
					{
						(input->result).insert(cursorPosition, std::get<1>(key));
						cursorPosition++;
					}
				}
			}

		}
		return true;
	}
	bool System::ArrKeys() {
		if (GE->IsFocused()) {
			if (GE->GetKey(std::get<0>(Arr[2])).bPressed)
			{
				(input->selection) = std::clamp((input->selection) - 1, 0, int(input->GetOptions().size()) - 1);
				input->result = input->GetOptions()[input->selection];
			}
			if (GE->GetKey(std::get<0>(Arr[3])).bPressed)
			{
				(input->selection) = std::clamp((input->selection) + 1, 0, int(input->GetOptions().size()) - 1);
				input->result = input->GetOptions()[input->selection];
			}
		}
		return true;
	}

	bool System::Update() {
		switch (currentStatus) {
		case ACTIVE:
			if (!input->StartUp()) {
				return false;
			}
			switch (input->GetFormat().inputType) {
			case SELECTFILE:
			case SELECTOPTIONS:
				ShowOptions();
				break;
			case VALUE:
				DisplayValue();
				break;
			case SAVEFILE:
				ShowOptions(true);
				break;
			default:
				break;
			}

			GetKeyValue();
			break;
		case IDLE:
			break;
		case WARNING:
			DisplayNote(errorMessage);
			if (GE->GetKey(olc::Key::ENTER).bPressed) {
				if (input == nullptr) {
					SetIdle();
				}
				else {
					currentStatus = ACTIVE;
				}
			}
			else if ((GE->GetKey(olc::Key::ESCAPE).bPressed)) {
				SetIdle();
			}
			break;
		default:
			break;
		}

	}

	bool System::GetKeyValue() {
		switch (input->GetFormat().keyBoard) {
		case ALPHABETIC:
			AlphaKeys();
			break;
		case NUMERIC:
			NumKeys();
			break;
		case ARROW:
			ArrKeys();
			break;
		case ALPHANUMERIC:
			AlphaKeys();
			NumKeys();
			break;
		case ALPHANUMARR:
			AlphaKeys();
			NumKeys();
			ArrKeys();
			break;

		}

		if (input->GetFormat().keyBoard != ARROW) {
			if (GE->GetKey(olc::Key::RIGHT).bPressed) {
				if (cursorPosition < input->result.size()) {
					cursorPosition++;
				}
			}
			if (GE->GetKey(olc::Key::LEFT).bPressed) {
				if (cursorPosition > 0) {
					cursorPosition--;
				}
			}

		}

		if (GE->GetKey(olc::Key::DEL).bPressed) {
			if (input->result.size() > 0 && cursorPosition >= 0 && cursorPosition < input->result.size()) {
				input->result.erase(cursorPosition, 1);
			}
		}
		else if (GE->GetKey(olc::Key::BACK).bPressed) {
			if (input->result.size() > 0 && cursorPosition > 0 && cursorPosition <= input->result.size()) {
				input->result.erase(cursorPosition - 1, 1);
				cursorPosition--;
			}
		}
		else if (GE->GetKey(olc::Key::ENTER).bPressed) {
			if (input == nullptr && currentStatus == ACTIVE) {
				SetIdle();
			}
			else if (input->ConvertValue(errorMessage)) {
				currentStatus = RETURN;
			}
			else if (errorMessage == InputError::Retry) {
				currentStatus = ACTIVE;
				input->StartUp();
			}
			else {
				currentStatus = WARNING;
				Log(errorMessage);
			}
		}
		else if (GE->GetKey(olc::Key::ESCAPE).bPressed) {
			SetIdle();
		}

		return true;
	}



	bool System::ShowOptions(bool AllowValue) {
		DisplayOptions(input->GetOptions(), AllowValue);
		return true;
	}



	void System::DisplayValue() {


		//WriteSize
		uint16_t h = GE->ScreenHeight();
		uint16_t w = GE->ScreenWidth();
		float factorX = 0.6f;
		float factorY = 0.3f;

		std::pair<uint16_t, uint16_t> startPos = std::make_pair<uint16_t, uint16_t>(w * (1 - factorX) / 2, h * (1 - factorX) / 2);
		std::pair<uint16_t, uint16_t> size = std::make_pair<uint16_t, uint16_t>(w * factorX, h * factorY);
		
		GE->FillRect({ startPos.first, startPos.second }, { size.first, size.second }, olc::GREY);

		factorX *= 0.9;
		factorY *= 0.9;
		startPos = std::make_pair<uint16_t, uint16_t>(w * (1 - factorX) / 2, h * (1 - factorX) / 2);
		size = std::make_pair<uint16_t, uint16_t>(w * factorX , h * factorY);

		
		GE->FillRect({ startPos.first, startPos.second }, { size.first, size.second }, olc::WHITE);

		uint8_t textSpace = 16;

		uint8_t offset = (textSpace - 8) / 2;

		//DrawStrings
		//Title
		GE->DrawString({ int(startPos.first + size.first / 2.0f - input->GetTitle().size() * 4.0f), int(startPos.second + offset) }, input->GetTitle(), olc::BLACK);

		//Input
		GE->FillRect({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f), int(startPos.second + size.second - textSpace - offset * 2) }, { int(input->result.size() * 8 + offset * 2), textSpace }, olc::GREY);
		GE->DrawString({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f + offset + cursorPosition * 8 - 4), int(startPos.second + size.second - textSpace - offset) }, "|", olc::RED);
		GE->DrawString({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f + offset), int(startPos.second + size.second - textSpace - offset) }, input->result, olc::BLACK);


	}


	void System::DisplayOptions(std::vector<std::string>& entrys, bool AllowValue) {


		//WriteSize
		uint16_t h = GE->ScreenHeight();
		uint16_t w = GE->ScreenWidth();
		float factor = 0.6f;

		std::pair<uint16_t, uint16_t> startPos = std::make_pair<uint16_t, uint16_t>(w * (1 - factor) / 2, h * (1 - factor) / 2);
		std::pair<uint16_t, uint16_t> size = std::make_pair<uint16_t, uint16_t>(w * factor, h * factor);

		GE->FillRect({ startPos.first, startPos.second }, { size.first, size.second }, olc::GREY);
		
		factor *= 0.9;
		startPos = std::make_pair<uint16_t, uint16_t>(w * (1 - factor) / 2, h * (1 - factor) / 2);
		size = std::make_pair<uint16_t, uint16_t>(w * factor, h * factor);

		GE->FillRect({ startPos.first, startPos.second }, { size.first, size.second }, olc::WHITE);

		uint8_t textSpace = 16;

		uint8_t offset = (textSpace - 8) / 2;

		uint8_t OptionsOffset = 0;
		uint8_t max = std::min(int(entrys.size()), 10);
		//DrawStrings

		if (input->selection > 9 && input->selection < entrys.size()) {
			OptionsOffset = input->selection-8;
			max = std::min(int(entrys.size() - OptionsOffset), 10);
		}
		else if (input->selection >= entrys.size()) {
			input->selection = 0;
		}

		for (uint8_t i = 0; i < max; i++) {
			//Only Draw the string the in the alocated space, with a slight offset
			if (i + OptionsOffset == input->selection) {
				GE->FillRect({ startPos.first, startPos.second + textSpace * i }, { size.first,textSpace }, olc::BLUE);
				GE->DrawString({ startPos.first + offset, startPos.second + offset + textSpace * i }, entrys[i + OptionsOffset], olc::WHITE);
			}
			else {
				GE->DrawString({ startPos.first + offset, startPos.second + offset + textSpace * i }, entrys[i + OptionsOffset], olc::BLACK);
			}
		}

		if (AllowValue) {

			//Input
			GE->FillRect({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f), int(startPos.second + size.second - textSpace - offset * 2) }, { int(input->result.size() * 8 + offset * 2), textSpace }, olc::GREY);
			GE->DrawString({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f + offset + cursorPosition * 8 - 4), int(startPos.second + size.second - textSpace - offset) }, "|", olc::RED);
			GE->DrawString({ int(startPos.first + size.first / 2.0f - input->result.size() * 4.0f + offset), int(startPos.second + size.second - textSpace - offset) }, input->result, olc::BLACK);
		}


	}



	void System::DisplayNote(std::string& errorMessage) {


		//WriteSize
		uint16_t h = GE->ScreenHeight();
		uint16_t w = GE->ScreenWidth();
		float factor = 0.4f;

		std::pair<uint16_t, uint16_t> startPos = std::make_pair<uint16_t, uint16_t>(w * (1 - factor) / 2, h * (1 - factor) / 2);
		std::pair<uint16_t, uint16_t> size = std::make_pair<uint16_t, uint16_t>(w * factor, h * factor);

		GE->FillRect({ startPos.first, startPos.second }, { size.first, size.second }, olc::WHITE);

		uint8_t textSpace = 16;

		uint8_t offset = (textSpace - 8) / 2;

		GE->DrawString({ int(startPos.first + offset + size.first / 2 - errorMessage.size() * 4.0), int(startPos.second + offset + size.second / 2.0f )}, errorMessage, olc::BLACK);


	}

}



