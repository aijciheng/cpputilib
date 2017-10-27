target+=test_httpclient
root_path=../..
include=-I $(root_path)/include
objs=test_httpclient.o httpclient.o

test_httpclient:$(objs)
	g++ -o $@ $^ -lcurl -std=c++11

.cpp.o:
	g++ -c -o $@ $^ -O2 -std=c++11 $(include)

clean:
	rm -rf test_httpclient $(objs)

.PHONY:clean

vpath %.cpp ./ $(root_path)/src/net/
