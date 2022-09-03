src = $(wildcard ./src/*.cpp)       			#提取所有的.c文件并赋给src
#obj = $(patsubst %.cpp,%.o,$(src))  			#将src文件中所有的.c文件替换为.o文件

CC = arm-linux-g++
RM = -rm -rf
TARGET = ./bin/client.exe
INCLUDE_PATH += -I ./inc/
SLIB = ./lib/libfont.a -lm
OPTION = -pthread
#DLIBRARY_PATH += -L ./lib/
#DLIB += -l(lib name)

ALL:$(TARGET)						     	 	#生成的最终目标
$(TARGET):$(src)
	$(CC) $^ -o $@	$(SLIB)  $(INCLUDE_PATH)   $(OPTION)	     
#$(obj):%.o:%.cpp						     	#静态模式规则
#	$(CC) -c $< -o $@  $(INCLUDE_PATH)   
clean:
	$(RM) $(obj) $(TARGET) 
.PHONY: clean ALL                        		#伪目标