package main

// #cgo LDFLAGS: -lm
// extern int decompress_frames(int framel, char* blob, char* deblob);
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
