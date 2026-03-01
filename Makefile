compile: 
	cd server && make
	cd client && make

run:
	cd server && make exec &
	cd client && make exec
