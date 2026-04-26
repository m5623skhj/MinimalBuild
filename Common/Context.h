#pragma once
#include <vector>

using ContextItemType = unsigned short;

struct Context
{
	explicit Context( ContextItemType type, std::vector<uint8_t> data )
		: type( type ), data( std::move( data ) )
	{
	}

	ContextItemType type;
	std::vector<uint8_t> data;
};
