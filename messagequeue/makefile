.PHONY :all
	all :client server
client:client.cc commen.cc 
	gcc -o $@ $^
server:server.cc commen.cc 
	gcc -o $@ $^
.PHONY: clean 
clean : 
	rm -f client server 
