app=test_json
target+=$(app)
root_path=../..
include=-I $(root_path)/include
objs=$(app).o

$(app):$(objs)
	g++ -o $@ $^ -ljansson -std=c++11

.cpp.o:
	g++ -c -o $@ $^ -O2 -std=c++11 $(include)

clean:
	rm -rf $(app) $(objs)

.PHONY:clean

vpath %.cpp ./ $(root_path)/src/json/
