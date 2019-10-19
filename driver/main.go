package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"time"

	"github.com/google/gousb"
)

func main() {
	stage1 := flag.Bool("stage1", false, "USB mode")
	stage2 := flag.Bool("stage2", false, "FPGA mode and run time")
	flag.Parse()

	ctx := gousb.NewContext()
	defer ctx.Close()

	log.Printf("Opening device...")

	dev, err := ctx.OpenDeviceWithVIDPID(0x5555, 0x3382)
	if err != nil {
		log.Fatalf("Could not open a device: %v", err)
	}
	defer dev.Close()

	log.Printf("Claim default interface...")

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
	if *stage1 {
		log.Printf("Setting the USB Controller firmware I think...")

		for n, packet := range usbInit {
			if n%10 == 0 {
				thrd := len(usbInit) / 3
				switch n / thrd {
				case 0:
					fmt.Print("U")
				case 1:
					fmt.Print("S")
				case 2:
					fmt.Print("B")
				}
			}

			_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
			if err != nil {
				log.Printf("\nfailed to bitstream %s", err.Error())
			}
		}

		time.Sleep(time.Second)
		os.Exit(0)
	} else if *stage2 {
		buf := make([]byte, 64)
		_, err = dev.Control(0xc0, 185, 0x0021, 0, buf)

		log.Printf("Setting the FPGA bitstream I think...")

		for n, packet := range fpgaInit {
			if n%60 == 0 {
				thrd := len(fpgaInit) / 4
				switch n / thrd {
				case 0:
					fmt.Print("F")
				case 1:
					fmt.Print("P")
				case 2:
					fmt.Print("G")
				case 3:
					fmt.Print("A")
				}
			}

			_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
			if err != nil {
				log.Printf("\nfailed to bitstream %s", err.Error())
			}
		}

	} else {
		log.Fatalf("Set a -stage1 or -stage2")
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
