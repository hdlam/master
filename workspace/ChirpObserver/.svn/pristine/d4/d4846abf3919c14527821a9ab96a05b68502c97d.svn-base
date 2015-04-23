CC = g++
CCFLAGS = -Wall -std=c++11
CFLAGS = -Wall 
EXEC_NAME = chirpObserver
INCLUDES =
LIBS = -lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc -lboost_program_options -lboost_system
OBJ_FILES = main.o functions.o Config.o ../camera/BoundingBoxHandler.o ../camera/CameraHandler.o ../camera/Led.o ../hungarian/Hungarian.o ../tracker/Chirp.o ../tracker/GeneralTracker.o ../tracker/Tracker.o ../rs232/rs232.o

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) $(CCFLAGS) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<
