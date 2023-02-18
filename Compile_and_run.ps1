New-Item -ItemType Directory -Force -Path .\build | Out-null

g++ -I .\include .\src\Main.cpp .\src\gl_framework.cpp .\src\shader_util.cpp .\src\glad.c .\src\Cubes.cpp .\src\camera.cpp .\src\Input.cpp .\src\File_manager.cpp -o .\build\Vox_Render -L .\lib -lopengl32 -lgdi32 -lglfw3

cp .\src\simple_fs.glsl .\build\simple_fs.glsl
cp .\src\simple_vs.glsl .\build\simple_vs.glsl

cp .\src\line_fs.glsl .\build\line_fs.glsl
cp .\src\line_vs.glsl .\build\line_vs.glsl

cp .\src\cursor_fs.glsl .\build\cursor_fs.glsl

cd build
.\Vox_Render.exe
cd ..