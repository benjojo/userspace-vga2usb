package main

import (
	"fmt"
	"log"
	"time"

	"github.com/google/gousb"
)

func main() {
	ctx := gousb.NewContext()
	defer ctx.Close()

	log.Printf("Opening device...")

	dev, err := ctx.OpenDeviceWithVIDPID(0x5555, 0x3382)
	if err != nil {
		log.Fatalf("Could not open a device: %v", err)
	}
	defer dev.Close()

	log.Printf("Claim default interface...")

	// Claim the default interface using a convenience function.
	// The default interface is always #0 alt #0 in the currently active
	// config.

	// intf, done, err := dev.DefaultInterface()
	// if err != nil {
	// 	log.Fatalf("%s.DefaultInterface(): %v", dev, err)
	// }
	// defer done()
	// intf.String()

	// log.Printf("Grapping endpoint to bitstream FPGA...")
	// bitStreamEndpoint, err := intf.OutEndpoint(0)
	// if err != nil {
	// 	log.Printf("Failed to open bitstreaming endpoint to upload firmware to. %v \n %s", intf, err.Error())
	// 	return
	// }

	deviceConfig, err := dev.Config(1)
	if err != nil {
		log.Printf("Failed to get config for device")
		return
	}

	for num, interf := range deviceConfig.Desc.Interfaces {
		fmt.Printf("\tINTERFACE #%d\n", num)

		for numAlt, altset := range interf.AltSettings {
			fmt.Printf("\t\t%d - Number: %d - Alt %d\n", numAlt, altset.Number, altset.Alternate)
			for numEndpoint, endpointset := range altset.Endpoints {
				fmt.Printf("\t\t\t%d - %#v\n", numEndpoint, endpointset)
			}
		}
	}

	// dev.ControlTimeout = time.Second

	log.Printf("Putting the device in programming mode I think?")

	fmt.Print("US")
	for _, packet := range usbInit {
		_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
		if err != nil {
			log.Printf("\nfailed to bitstream %s", err.Error())
		}

		// bitStreamEndpoint.Write(buf[:n])
		fmt.Print("B")
	}

	time.Sleep(time.Hour)

}

type usbControlPacket struct {
	ignore      bool
	RequestType uint8
	BRequest    uint8
	WValue      uint16
	WIndex      uint16
	Data        []byte
}
