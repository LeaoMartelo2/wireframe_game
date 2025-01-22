# Wireframe Game / Game engine

Game engine originaly started in `C`, now being developed in `C++`, using [RayLib](https://github.com/raysan5/raylib).

The goal of the engine is to follow simplicity, have as little dependencies as possible,
have the compile process go smooth and easy, and have a `Wireframe` visual style

Due the simplicity goal, there is avoidance in the use of `"modern C++"`, and most of the C++ code is written in C style.

<img src="readme/1.png" width="400">

> [!TIP]
> Please also check out its brother project [Wireframe Editor](https://github.com/LeaoMartelo2/wireframe_editor).\
> A tool for generating Levels for this project.


## Compiling

### Linux
 - Dependencies
    - [Make](https://www.gnu.org/software/make/)
    - [gcc](https://gcc.gnu.org/)
    - Any other dependencies are baked in the project itself.

 - To compile, just run `make` at the project's root.
 - This will provide you with a 'wireframe' binary.

### Microsoft Windows

> [!WARNING]
> Build support for Microsoft Windows is not implemented yet.

> [!NOTE]
> You can cross-compile from Linux to Microsoft Windows.\
> Requires: [x86_64-w64-mingw32-g++](https://www.mingw-w64.org/) + everything from Linux dependencies.\
> Run `make win`, you'll be left out with `wireframe.exe`


## Engine structure

> [!IMPORTANT]
> This information is subject to get outdated at any time.

The engine structure goal can be represented as such:

<pre>
Engine Instance
 ┃
 ┣ Scene Manager 
 ┃      ┃
 ┃      ┣ Player
 ┃      ┃   ┗ ~ Player data ~
 ┃      ┃
 ┃      ┣ Scene (1)
 ┃      ┃   ┃
 ┃      ┃   ┗ Level
 ┃      ┃       ┗ ~ Level data ~
 ┃      ┃
 ┃      ┗ Scene (2)
 ┃          ┃
 ┃          ┗ (Other)
 ┃              ┗ ~ Some data relevant to the scene ~
</pre>

 

| Item          | Description                                                          | 
| ---           | ---                                                                  |
| Scene manager | Manages all the scenes in the game and the player                    |
| Scene         | Manages level data, such as the current level and current game logic |
| Player        | Player Character                                                     |
| (Other)       | Used to represent generic data in the board                          |


### The Scene layout

This structure allows you to have multiple scenes and they behave differently between eachother.\
Each scene can hold a level and data related to it. This way you can think of each scene as a different map you load in.

### Why is the player a member of the Scene manager?

The Player being a child of the Scene Manager instead of the current running scene, allows to easily have player data be persistent between scenes
otherwise you would have a need to create player copies, that would frequently get out of sync and get hard to manage.\
Doing it this way, in the other hand, allows you to just pass down a pointer of the player instance at the manager.


## Logging

This projects integrates with [LogNest](https://github.com/LeaoMartelo2/LogNest).

The engine, by default, writes its log to `latest.log` at the main executable path.


<img src="readme/2.png">

For convenience, it's also provided a [Shell script](./log.sh)\
This script will automatically print the log, while also coloring each log type with a corresponding color.
