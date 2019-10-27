all:
	gofmt -e -s -w .
	# go vet .
	$(CC) -g -L lib/ -lfrmgrab -fPIC -c -o lib/lib.o lib/wrapper.c
	$(CC) -g -L lib/ -lfrmgrab -fPIC -shared -o liblib.so lib/lib.o
	LD_LIBRARY_PATH=. go build main.go
	# cp liblib.so wrapper/
	# LD_LIBRARY_PATH=. go build main.go