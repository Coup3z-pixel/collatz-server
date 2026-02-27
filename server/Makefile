compile:
	gcc -Wall -o server main.c connection_pool/addr_pool.c collatz/verify.c db/database.c db/pages.c connection_pool/index_queue.c -lm

exec:
	./server server db_storage
