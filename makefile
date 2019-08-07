cppFiles=main.cpp Diskstats.cpp
objFiles=$(cppFiles:.cpp=.o)


executable=main

$(executable) : $(objFiles)
	g++ -o $(executable) -g -Wall $(objFiles)

%.o : %.cpp
	g++ -o $@ -g -Wall -c $<

clean : 
	rm -rf $(objFiles) $(executable)
