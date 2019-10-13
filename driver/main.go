package main

import (
	"fmt"
	"log"

	"github.com/google/gousb"
)

func main() {
	fmt.Print(".")

	ctx := gousb.NewContext()
	defer ctx.Close()

	dev, err := ctx.OpenDeviceWithVIDPID(0x5555, 0x3382)
	if err != nil {
		log.Fatalf("Could not open a device: %v", err)
	}
	defer dev.Close()

}
