all: menu cpp_sync

menu: menu.cc
	g++ menu.cc -o menu

cpp_sync: cpp_sync.cc
	g++ -lpthread cpp_sync.cc -o cpp_sync

