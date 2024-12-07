@REM SET SRC_DIR=..\src
@REM SET OBJ_DIR=..\obj
@REM SET LIB_DIR=..\lib
@REM SET INCLUDE_DIR=..\src\includes

@REM ::compile
@REM g++ -c -o %OBJ_DIR%\main.o -I %INCLUDE_DIR% %SRC_DIR%\main.cpp
@REM g++ -c -o %OBJ_DIR%\foo1.o -I %INCLUDE_DIR% %SRC_DIR%\foo1.cpp
@REM g++ -c -o %OBJ_DIR%\foo2.o -I %INCLUDE_DIR% %SRC_DIR%\foo2.cpp

@REM ::link
@REM g++ -o main.exe %OBJ_DIR%\main.o %OBJ_DIR%\foo1.o %OBJ_DIR%\foo2.o -l mylib -L%LIB_DIR%

g++ -std=c++11 -I./include -o test_simulation src/simulation/*.cpp
