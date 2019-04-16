.PHONY:all 
thread :thread_cond.cc
	g++ -pthread -o $@ $^ 
.PHONY:clean
clean:
	rm -f thread 

