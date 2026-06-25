server: build/app.o build/window.o
	g++ build/app.o build/window.o -o server `wx-config --cxxflags --libs`

build/app.o: headers/app.h src/app.cpp
	g++ -c src/app.cpp -o build/app.o `wx-config --cxxflags --libs`

build/window.o: headers/window.h src/gui.cpp src/socket.cpp src/buttons.cpp src/gui-helpers.cpp
	g++ -c src/gui.cpp -o build/window.o `wx-config --cxxflags --libs`

clean:
	rm build/*.o