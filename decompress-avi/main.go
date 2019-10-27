package main

/*
#include <lib/v2u_compression.h>
#include <lib/wrapper.h>
#cgo CFLAGS: -g -I ./ -I ./lib/
#cgo LDFLAGS: -L lib/ -lfrmgrab -L. -llib
*/
import "C"
import (
	"bytes"
	"unsafe"
)

func main() {
	buf := make([]byte, 100)
	bytes.NewBuffer(buf)
	b := buf.Bytes()

	i := C.decompress_frames(C.int(buf.Len()), unsafe.Pointer(&b[0]), unsafe.Pointer(&b[0]))
}

//
