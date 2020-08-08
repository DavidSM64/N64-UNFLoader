# UNFLoader
**This project is in development and was only uploaded here to facilitate its development.**

UNFLoader is a USB ROM uploader (and debugging) tool designed to unify developer flashcarts. The goal of this project is to provide developers with USB I/O functions that work without needing to worry about the target flashcart, however I have implemented some useful features to boot (such as printf).
Currently supported devices:
* [64Drive Hardware 1.0 and 2.0](http://64drive.retroactive.be/)
* EverDrive 3.0 (No longer comercially sold)
* [EverDrive X7](https://krikzz.com/store/home/55-everdrive-64-x7.html)


### Current TODO list:
* **Library** - Finish cleaning up EverDrive 3.0 code for printf.
* **Library** - Make printf work on the EverDrive X7.
* **Library** - Write sample demos.
* **UNFLoader** - Implement text input.
* **UNFLoader** - Implement scrolling.
* **UNFLoader** - Implement command history.
* **UNFLoader** - Implement binary file uploading via USB.
* **UNFLoader** - Look into the possibility of implementing remote console reset.
* **UNFLoader + Library** - Implement binary data reading from USB for the 64Drive.
* **UNFLoader + Library** - Implement binary data reading from USB for the EverDrive 3.0.
* **UNFLoader + Library** - Implement binary data reading from USB for the EverDrive X7.
* **UNFLoader + Library** - Clean up the final code to ensure there are plenty of convenience functions (such as `debug_senddata(char type, int size, char* data)`).
* **UNFLoader + Library** - Implement screenshot functionality.
* **UNFLoader + Library** - Implement data checksum.


### Requirements:
* Windows XP or higher
* [This FDTI driver](http://www.ftdichip.com/Drivers/CDM/CDM21228_Setup.zip)


### Using UNFLoader
Simply execute the program for a full list of commands. If you run the program with the `-help` argument, you have access to even more information (such as how to upload via USB with your specific flashcart, how to debug thread faults, etc...). The most basic usage is `UNFLoader.exe -r PATH/TO/ROM.n64`. Append `-d` to enable debug mode, which allows you to receive/send input from/to the console (Assuming you're using the included debug library). Append `-l` to enable listen mode, which will automatically reupload a ROM once a change has been detected.


### Using the Debug Library
Assuming you are using libultra, simply include the debug.c and debug.h in your project. You can edit debug.h to enable/disable debug mode (which makes your ROM smaller if disabled).
Here are the included functions:
```c
// Pretty much your standard printf
void debug_printf(const char* message, ...);

// Call this in a loop somewhere to receive USB data
void debug_poll();

// Stop the program if the expression is false
#define debug_assert(expression)
```


### Building the UNFLoader program
Simply load the project file in Visual Studio 2019 or higher.
The Include folder should already have everything you need, but if you wish to build/retrieve the libraries yourself:

**pdcurses.lib**
* Grab the latest version of PDCurses from [here](https://github.com/wmcbrine/PDCurses).
* Extract the contents of the zip (preferrably somewhere with no spaces in the file path, like `c:\pdcurses`).
* Open the Visual Studio Command Prompt (Tools->Command Line->Developer Command Prompt).
* Run the command `set PDCURSES_SRCDIR=c:\PATH\TO\pdcurses`, obviously replacing the path with your one.
* CD into the `pdcurses/wincon` folder.
* Run the command `nmake -f Makefile.vc` to build pdcurses.
* Copy the `pdcurses.lib` that was compiled from the wincon folder to `UNFLoader/Include`, replacing the pdcurses library in there.
* Copy the `curses.h`, `curspriv.h`, and `panel.h` from the pdcurses directory and put them in `UNFLoader/Include`.
* Open `curses.h` and uncomment the line with `#define MOUSE_MOVED` to fix a warning due to `wincon.h`.

**ftd2xx.lib**
* Download the FTDI driver provided in the **Requirements** section and extract the executable from the zip.
* This is a self extracting executable, meaning you can open the .exe with with a zip program. 
* Grab `ftd2xx.h` and put it in `UNFLoader/Include`.
* Grab `ftd2xx.lib` from `i386` or `amd64` (depending on your CPU architecture) and put it in `UNFLoader/Include`.

Once you have all of these files built and put in the `Include` folder, you're set to compile!


### Extending the debug library/loader program
All data gets sent in the following manner:
* 4 Bytes with `'D' 'M' 'A' '@'` to signalize data start.
* 4 Bytes with 1 byte for the data type and 3 for the size of the data.
* N bytes with the data.
* 4 bytes with `'C' 'M' 'P' 'H'` to signalize data end.

The data type mentioned is up to the developer to implement. If you wish to add more data types so that the data is handled differently on the PC side, you must make changes to UNFLoader's `debug.c` file to add support for said command (check the `debug_main()` function's switch statement). Here's a list of default data types:
```c
#define DATATYPE_TEXT       0x01
#define DATATYPE_RAWBINARY  0x02
#define DATATYPE_SCREENSHOT 0x03
```
There is no checksum in place to detect the authenticity of the data. This might be implemented at a later date...

### Credits
Marshallh for providing the 64Drive USB application code which this program was based off of.

KRIKzz, saturnu and jsdf for providing sample code for the EverDrive 3.0 and/or X7.

networkfusion and fraser for all the help provided during the development of this project as well as their support.

The folk at N64Brew for being patient with me and helping test the program! Especially command_tab, CrashOveride, Gravatos, PerKimba, Manfried and Kivan117.
