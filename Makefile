STATICLIB=libcpputilib.a
TARGET=$(STATICLIB)
ROOT=./src
LIB=lib
INCLUDE=-I ./include -I $(ROOT)
FLAG=-Wall -O2 -std=c++11
TOBJS=config/config_manager.o \
     log/log.o \
     net/httpclient.o \
     time/time_utils.o 

OBJS=$(addprefix $(ROOT)/, $(TOBJS))

all:$(LIB) $(TARGET)

$(LIB):
	mkdir -p $(LIB)

$(STATICLIB):$(OBJS)
	ar rcs $(LIB)/$@ $^

.cpp.o:
	g++ -c -o $@ $^ $(INCLUDE) $(FLAG)

clean:
	rm -rf $(LIB) $(OBJS)

.PHONY:clean

vpath %.cpp $(ROOT)
vpath %.o ./lib
