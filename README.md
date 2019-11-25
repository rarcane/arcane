## License [EULA]
In accessing this repository and downloading it's contents you are agreeing to the EULA terms detailed in the LICENCE file as well as guidelines and conventions detailed in this README.

The application code of this game is built on top of Ryan Fleury's Telescope engine, subsequently **at no point** are you allowed to attempt to extract the underlying engine and use it personally.

# Arcane
G'day mate, welcome to the Arcane repo. 


First of all, you're probably only seeing this because you've become a Patreon; so thanks for supporting my work and helping me bring Arcane to life <3

If you *only* want to be able to compile the game and run it, then follow the steps detailed below. If you're interested in contributing to the source though, be sure to read the contribution guidelines before creating any requests.

## Compiling (Windows x64 only)
1. Clone/download this repository.
2. Install the MSVC compiler. (The easiest way to go about this is by just installing Visual Studio <https://visualstudio.microsoft.com/vs/features/cplusplus/>)
3. Call Arcane's build.bat **through** the developer command prompt of Visual Studio.
4. **Optional:** for a more permanent solution, setup your own .bat file that first calls `vcvars64.bat` before the build so it can recognise the `cl` command. (This batch file can be found in your Microsoft Visual Studio path, my path looks like `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build` but yours may be different).

**If you have any issues feel free to send me a DM/email and I'll help you out.**

# Contributions
An in-depth contribution model/guideline is in development. For now though if you really want to contribute to the source just DM me on Discord or send an email to <TJRandall27@gmail.com> and we'll have a chat about it.