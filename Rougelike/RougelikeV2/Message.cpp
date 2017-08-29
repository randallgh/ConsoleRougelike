#include "Message.h"

Message::Message()
{

}

Message::Message(char a[], int length)
{
	for (int i = 0; i < length; ++i)
	{
		data[i] = a[i];
	}
}


Message::~Message()
{
}
