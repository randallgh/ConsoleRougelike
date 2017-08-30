#include "Message.h"

Message::Message()
{

}

//Message::Message(char a[], int length)
//{
//	for (int i = 0; i < length; ++i)
//	{
//		data[i] = a[i];
//
//	}
//}

Message::Message(std::string str)
{
	str.resize(length);
	for (int i = 0; i < length; ++i)
	{
		data[i] = str.at(i);

	}
}


Message::~Message()
{
}
