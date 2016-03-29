#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <thread>

#include "Functions.hpp"
#include "DataParser.hpp"
#include "TimeManager.hpp"

class Console
{
	public:
		class Message
		{
			private:
				std::string header;
				std::string message;
				sf::Color textColor;
				std::string type;
				unsigned long long int timestamp;
				bool useTimeStamp;
			public:
				Message(std::string header, std::string message, sf::Color textColor, std::string type, bool timestamped = true);
				std::string getFormatedMessage();
				std::string getHeader();
				std::string getMessage();
				sf::Color getColor();
				int getR();
				int getG();
				int getB();
				int getA();
				std::string getType();
				void setMessage(std::string newmessage);
				void setColor(int r, int g, int b, int a = 255);
		};

		class Stream
		{
			private:
				std::string streamName;
				Console* consolePointer;
				sf::Color streamColor;
				std::vector<std::string> queryList;
			public:
				Stream(std::string streamName, Console* consolePointer);
				Console::Message* streamPush(std::string message, int r = 255, int g = 255, int b = 255, int a = 255);
				void setColor(int r, int g, int b, int a = 255);
				sf::Color getColor();
				int getR();
				int getG();
				int getB();
				int getA();
		};
		Console();
		void handleCommands(std::string text);
		std::string getCommand();
		bool hasCommand();
		void scroll(int power);
		Message* pushMessage(std::string headerName, std::string message, int r = 255, int g = 255, int b = 255, int a = 255, std::string type = "DEFAULT", bool disableTimestamp = false);
		void inputKey(int keyCode);
		void clearInputBuffer();
		std::string getInputBuffer();
		void update();
		Stream* createStream(std::string streamName, bool enabled = true);
		Stream* getStream(std::string streamName);
		void display(sf::RenderWindow* surf);
		bool isConsoleVisible();
		void setConsoleVisibility(bool enabled);
		void moveCursor(int move);

	private:
		Console::Stream* scrEngineStream;
		Console::Stream* scrErrorStream;
		int virtualCursor = 0;
		bool consoleVisibility = false;
		int consoleScroll = 0;
		bool consoleAutoScroll = true;
		bool consoleMuted = false;
		bool commandReady = false;
		std::string currentCommand;
		std::string currentTheme;
		sf::Font font;
		std::string inputBuffer;
		std::map<std::string, Stream*> streamMap;
		std::map<std::string, std::string> aliasMap;
		std::vector<std::string> userCommandList;
		std::vector<std::string> streamList;
		std::vector<std::string> disabledStreams;
		std::vector<Message*> consoleText;
};