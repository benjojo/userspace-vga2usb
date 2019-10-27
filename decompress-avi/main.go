package main

// #cgo LDFLAGS: -lm
// extern int decompress_frames(int framel, char* blob, char* deblob);
import "C"
import (
	"encoding/binary"
	"flag"
	"io/ioutil"
	"log"
	"os"
	"time"
	"unsafe"
)

func main() {
	// fmt.Printf("I'm about to segfault")
	flag.Parse()
	filename := flag.Arg(0)
	f, _ := os.Open(filename)

	framelen := uint32(0)
	binary.Read(f, binary.BigEndian, framelen)
	log.Printf("framelen %d", framelen)

	buf, _ := ioutil.ReadAll(f)
	buf2 := make([]byte, 2048*2048*3)

	start := time.Now()
	// buf := make([]byte, 100)
	_ = C.decompress_frames(C.int(len(buf)), (*C.char)(unsafe.Pointer(&buf[0])), (*C.char)(unsafe.Pointer(&buf2[0])))

	log.Printf("took %s", time.Since(start))
	log.Printf("%x", buf2[:60])
}
