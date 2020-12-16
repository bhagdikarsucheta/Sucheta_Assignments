ReadMe -

All assignments Projects contains 3 files -
1.Main assignment project folder.
2.Output_Video of particular assignment.
3.ReadMe of particular project assignment regarding keys and mouse control after running .exe file.


-All assignments Projects run using Visual Studio 2019.
-Windows SDK Version - 10.0
-Platform ToolSet - Visual Studio 2019 (v142)
-All Projects are set to Debug mode and x64 as Solution Platforms

*Software Libraries used - 
-GLEW 2.1.0
-GLM
-Assimp
-SOIL2

*All above are given in separate Directory named Software libraries in case required.

**NOTE- 
For ALL Libraries NO need of any path setting,its generic wherever project directory resides -

Their ALL PATHS are ALREADY CONFIGURED using $(SolutionDir)/../  that is wherever .sln exits directory residing near to it will take that path so no need of any Hardcoded Paths for software libraries like Assimp,SOIL2 etc
Eg - $(SolutionDir)/../assimp_x64/include


1.SOIL2 - Kept in Local Directory in External Libraries Folder
2.glm - kept in Local Folder named as glm.
3.assimp_x64 - Kept in local folder named as assimp_x64 having include and lib.
4.Textures - All textures are kept in local directories and are embedded in resource file of particular project.
5.Model Loading - Assimp is used for model loading and SOIL2 for its Texturing and Lights Phong lighting model for lighting in OpenGL.
Locally Model directories are kept as well named as nanosuit for Model Loading
6.GLEW - GLEW version 2.1.0,also kept locally

glm and glew are set in SDK Path.
