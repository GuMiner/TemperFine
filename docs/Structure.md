# TemperFine Structure
## File Structure
-----------------

Assets are *itailicized*, source code is **bolded**.

* bin -- External dependency DLLs.
* *config* -- In-game configuration files.
* docs -- **TemperFine** documentation & game design.
* *fonts* -- In-game fonts.
* *images* -- In-game images.
* **include** -- **TemperFine** & external dependency headers.
* lib -- Library files of external dependencies
* licenses -- Licenses of external dependencies
* *maps* -- Voxel terrain maps
* *models* -- In-game and design 3D models.
* **shaders** -- GLSL vertex/geometry/fragment shaders
* **src** -- **TemperFine** CPP files
* Root Folder -- *CodeBlocks* project files and basics

## Code Structure
-----------------

### Virtual Folders
-------------------------
TemperFine uses CodeBlocks virtual folders to split apart four types of game C++ classes:

####Config
Classes and Headers ending in **Config* manage the translation of **Config.txt* files into static constants usable by the rest of TemperFine.

**Config* classes should be included in *TemperFine.h*, linked to the text file in *TemperFine::TemperFine()*, initialized within *TemperFine::Initialize()*

####Manager

Classes and Headers ending in **Manager* simplify a particular aspect of TemperFine, such as image loading, voxel map loading, etc. **Manager* classes should also be included in *TemperFine.h* and initialized either within *TemperFine::Initialize()* or *TemperFine::LoadAssets()*. 

####Utility
Classes and Headers in the **Utility** virtual folder hold convenience methods to simplify string manipulation, string->number conversion, etc. Unlike **Manager** classes, **Utility** classes do not manage a collection of resources. However, **Utility** classes are also not core games classes.

#### (None)
Core game classes and headers are in the **Sources** and **Headers** folders within CodeBlocks. These classes handle a particular aspect of the TemperFine game itself.

###Game Operation
-----------------
####Startup & Setup
**TemperFine** starts when the *main()* method in *TemperFine.cpp* is called. This method initializes the *Logger* and *Constants* global, static objects, creates a new *TemperFine* object, and then calls *TemperFine::Initialize()* and *TemperFine::Run()*. 

**TemperFine** stops when *TemperFine::Run()* exits, after which *TemperFine::Deinitialize()* is called and the *TemperFine* object is destructed.

**TemperFine::Initialize()** is used to setup assets and structures that **do not** require an OpenGL context.

**TemperFine::LoadAssets()** is used to setup assets that **do** require an OpenGL context.

####Game Loops
**TemperFine::Run()** initializes the main OpenGL context (using SFML) and runs the ReadEvents-DrawFrame-DisplayFrame render loop. 

Physics events and timed updates should be handled from within the **Physics::Run()** loop, which runs at a slower framerate separately from the render loop. However, because the physics of **TemperFine** run on a separate thread, OpenGL updates cannot be performed from this thread -- see the [OpenGL Concepts] (./OpenGL4.md) section for more information.

###Global Structures
---------------------
*Logger* helps simplify writing to a log file. Logging is highly encouraged, as long as you don't write to the log file every frame.

*Constants* holds in-game constants that should not be user-configurable (such as the aspect ratio).

Random number generation is also handled by the *Constants* class because the random number generation seed is set upon startup, which means that random numbers are predictably generated as contants.