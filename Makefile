ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: tp.exe

tp.exe: tp.o
	gcc -o tp.exe tp.o $(PATH_ALLEGRO)$(LIB_ALLEGRO) -Os -O2

tp.o: tp.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c tp.c -Os -O2
 
clean:
	del tp.o 
	del tp.exe