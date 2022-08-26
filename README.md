**To play**
There is an .exe in the /bin folder.
Enjoy!


**To Re-build**
- in C++14 or above.
AND
- in x86 mode.

If issues arise:
1. Check that you have the SDL folder in the solution directory
2. Check that you have the SDL binary files (.dll) correctly placed in the project folder.
3. Check that the project settings are correctly set to:

-VC++ Directories:
	Include directories: `$(SolutionDir)SDL\include;$(IncludePath)`
	Library directories: `$(SolutionDir)SDL\lib\x86;$(LibraryPath)`

-Linker
	-Linker Input
		Additional Dependencies: `SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_mixer.lib;SDL2_ttf.lib;%(AdditionalDependencies)`
	-System
		SubSystem: `Console`
