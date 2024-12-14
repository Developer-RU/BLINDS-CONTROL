#pragma once

#include "main.hpp"

struct Relay
{
	bool one = OFF;
	bool two = OFF;
};

enum State : byte
{
	State_NotInit = 0x00,
	State_Init = 0x01,
	State_Ready = 0x02
};

enum Command : byte
{
	Command_Stop = 0x00,
	Command_Up = 0x01,
	Command_Down = 0x02
};
