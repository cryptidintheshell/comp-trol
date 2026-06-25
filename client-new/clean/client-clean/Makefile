CXX      = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -static-libgcc -static-libstdc++ -static -lws2_32

TARGET   = client.exe
SRC      = client-clean.cpp
HDR      = client-clean.h

# ── Targets ────────────────────────────────────────────────────────────────────

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

release: CXXFLAGS += -O2 -mwindows
release: $(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all release debug clean
