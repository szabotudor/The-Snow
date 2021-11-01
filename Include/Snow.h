#pragma once
#include<thread>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<Button.h>

using namespace std;

namespace ss {
	inline void process();
	thread process_thread(process);
}
