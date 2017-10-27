app=test_threadpool
target+=$(app)
root_path=../..
include=-I $(root_path)/include
objs=test_threadpool.o
libs=-lpthread

target=$(target)

$(app):$(objs)
	g++ -o $@ $^ $(libs) -lcurl -std=c++11

.cpp.o:
	g++ -c -o $@ $^ -O2 -std=c++11 $(include)

clean:
	rm -rf $(app) $(objs)

.PHONY:clean

vpath %.cpp ./ $(root_path)/src/thread/
