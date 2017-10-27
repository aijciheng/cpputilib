app=test_timeutils
target+=$(app)
root_path=../..
include=-I $(root_path)/include
objs=$(app).o time_utils.o

$(app):$(objs)
	g++ -o $@ $^ -lcurl -std=c++11

.cpp.o:
	g++ -c -o $@ $^ -O2 -std=c++11 $(include)

clean:
	rm -rf $(app) $(objs)

.PHONY:clean

vpath %.cpp ./ $(root_path)/src/time/
