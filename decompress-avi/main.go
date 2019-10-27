package main

// #cgo LDFLAGS: -lm
// extern int decompress_frames(int framel, char* blob, char* deblob);
import "C"
import (
	"unsafe"
)

func main() {
	buf := make([]byte, 100)
	_ = C.decompress_frames(C.int(len(buf)), (*C.char)(unsafe.Pointer(&buf[0])), (*C.char)(unsafe.Pointer(&buf[0])))
}
