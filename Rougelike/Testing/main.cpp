#include <iostream>
#include <fstream>
#include <chrono>

#include <string>

void intToCharArray(int num, char a[], int length);

int main() 
{
	//Out to PC
	std::ofstream myfile;
	myfile.open("test.txt");
	myfile << "Hell" << " " << "\n";
	myfile << "Hell" << __TIME__ << "\n";
	myfile << "Hell" << __TIME__ << "\n";
	myfile << "Hello text editor. " << __TIME__ << "\n";
	myfile.close();

	//Into code
	std::ifstream afile;
	std::string line;
	afile.open("test.txt");

	if (afile.is_open()) 
	{
		while ( getline(afile,line) )
		{
			std::cout << line << " " << line.size() << std::endl;
		}
	}
	else 
	{
		std::cout << "Unable to find file";
	}

	system("PAUSE");
	return 0;
}

void intToCharArray(int num, char a[], int length) 
{
	int total = num;
	int temp = num;

	bool isChecking = true;
	bool isFirstRun = true;


	std::cout << temp << std::endl;

	int iLength = 0;
	for (int i = 0; isChecking;  ++i)
	{
		std::cout << i << std::endl;
		std::cout << temp << std::endl;
		if (temp < 10) {

			if (isFirstRun) {
				iLength = i + 1;
				isFirstRun = false;
			}

			std::cout << "Temp " << temp << std::endl;
			std::cout << "I " << i << std::endl;
			std::cout << "pow(temp, i)" << pow(temp * 10, i) << std::endl;
			total = total - pow(temp * 10, i);
			a[i] = temp + 48;
			std::cout << "Total " << total << std::endl;
			temp = total;
			std::cout << "A[I] " << a[i] << std::endl;


			if (total == 0) {
				isChecking = false;
				break;
			}
			break;
			i = 0;
		}

		if (temp > 0) {
			temp = (int)(temp / 10);
		}
	}
}

char intToChar(int i) {
	return i + 48;
}