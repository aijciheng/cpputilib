app=test_stringutils
target+=$(app)
root_path=../..
include=-I $(root_path)/include
objs=$(app).o
flag=-Wall -Werror

$(app):$(objs)
	g++ -o $@ $^ -lcurl -std=c++11 $(flag)

.cpp.o:
	g++ -c -o $@ $^ -O2 -std=c++11 $(include) $(flag)

clean:
	rm -rf $(app) $(objs)

.PHONY:clean

vpath %.cpp ./
