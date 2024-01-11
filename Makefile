CXX = g++ --std=c++20
CXXFLAGS = -Wall -g
DEFINE = 
DEPS = 
IDIR = -Iinclude -Iinclude/soil2
LIBS = -lglfw3 -lglew32 -lopengl32 -lglu32 -lsoil2
LDIR = -L./
SRC = ./src
CHAPTER = $(SRC)/Chapter

default:
	@echo -ne "\nPlease define a chapter to make e.g. make Chapter$(%)"

Chapter1: $(CHAPTER)1/main.cxx
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)1/main.cxx $(DEPS) $(IDIR) $(LIBS) $(LDIR)

Chapter2-1:
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)2/2-1/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter2 $(LIBS) $(LDIR)

Chapter2-2:
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)2/2-2/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter2 $(LIBS) $(LDIR)

Chapter2-3:
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)2/2-3/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter2 $(LIBS) $(LDIR)

Chapter3-1:
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)3/3-1/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter3 $(LIBS) $(LDIR)

Chapter3-2:
	$(CXX) $(CXXFLAGS) $(DEFINE) -o ./bin/$@.exe $(CHAPTER)3/3-2/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter3 $(LIBS) $(LDIR)

Chapter3-3:
	$(CXX) $(CXXFLAGS) -o ./bin/$@.exe $(CHAPTER)3/3-3/main.cxx $(DEPS) $(IDIR) -Iinclude/Chapter3 $(LIBS) $(LDIR)
