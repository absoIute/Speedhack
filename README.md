# Speedhack
Lightweight speedhacking source.
A "speedhack" is a way of manipulating the time of a program, usually used in games, it can slow down, speed up or even hault an entire program.

## Usage
To use this, you must have [detours express 3.0](https://www.microsoft.com/en-us/download/details.aspx?id=52586). Add it to your include directories and link with the library.
The code will __only__ have affect internally, to use on other programs, compile as a DLL and inject the it into the target process.

```cpp
int main(int argc, char *argv[])
{
	Speedhack::Setup();	//call this once
	Speedhack::speed = 0.5; //change this whenever you like

	return 0;
}
```