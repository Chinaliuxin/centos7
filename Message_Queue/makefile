.PHONY :all
all: client server
client :client.cc comm.cc 
	g++ -o $@ $^
server :server.cc comm.cc 
	g++ $^ -o $@
.PHONY :clean
clean:
	rm -f client server
##删除和创建
