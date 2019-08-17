cppFiles=main.cpp Diskstats.cpp

binDir=bin/
_objFiles=dir_main.o dir_Diskstats.o
objFiles=$(subst dir_,$(binDir),$(_objFiles))

executable=main

$(executable) : $(objFiles)
	g++ -o $(executable) -g -Wall $(objFiles)

$(binDir)%.o : %.cpp
	g++ -o $@ -g -Wall -c $<

clean : 
	rm -rf $(objFiles) $(executable)
