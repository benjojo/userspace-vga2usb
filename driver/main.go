package main

import (
	"fmt"
	"log"
	"os"
	"time"

	"github.com/google/gousb"
)

func main() {
	bitstreamFirmware, err := os.Open("./AA240.bitstream")
	if err != nil {
		log.Printf("Could not open the bitstream file, failing %s", err)
		return
	}

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

	/*
			func (d *Device) DefaultInterface() (intf *Interface, done func(), err error) {
			cfgNum, err := d.ActiveConfigNum()
			if err != nil {
				return nil, nil, fmt.Errorf("failed to get active config number of device %s: %v", d, err)
			}
			cfg, err := d.Config(cfgNum)
			if err != nil {
				return nil, nil, fmt.Errorf("failed to claim config %d of device %s: %v", cfgNum, d, err)
			}
			i, err := cfg.Interface(0, 0)
			if err != nil {
				cfg.Close()
				return nil, nil, fmt.Errorf("failed to select interface #%d alternate setting %d of config %d of device %s: %v", 0, 0, cfgNum, d, err)
			}
			return i, func() {
				intf.Close()
				cfg.Close()
			}, nil
		}
	*/

	// // intf, err := deviceConfig.Interface(0, 1)
	// if err != nil {
	// 	log.Printf("kaboom! %s", err.Error())
	// 	return
	// }

	// time.Sleep(time.Hour)

	// GOOD?
	/*
		GOOD
		0000  80 67 f5 de 28 94 ff ff  53 02 80 19 03 00 00 3c   .g..(... S......<
		0010  4b 3d a2 5d 00 00 00 00  c8 0e 0b 00 8d ff ff ff   K=.].... ........
		0020  40 00 00 00 00 00 00 00  c0 b9 21 00 00 00 40 00   @....... ..!...@.
		0030  00 00 00 00 00 00 00 00  00 02 00 00 00 00 00 00   ........ ........

		BAD
		0000  00 86 8e b9 2d 92 ff ff  53 02 80 0d 03 00 00 3c   ....-... S......<
		0010  d7 93 a3 5d 00 00 00 00  42 39 0a 00 8d ff ff ff   ...].... B9......
		0020  40 00 00 00 00 00 00 00  c0 b9 21 00 00 00 40 00   @....... ..!...@.
		0030  00 00 00 00 00 00 00 00  00 02 00 00 00 00 00 00   ........ ........

		BAD
		0000  00 70 bd 74 2b 92 ff ff  53 02 80 0e 03 00 00 3c   .p.t+... S......<
		0010  62 94 a3 5d 00 00 00 00  e4 ff 0d 00 8d ff ff ff   b..].... ........
		0020  40 00 00 00 00 00 00 00  c0 b9 21 00 00 00 40 00   @....... ..!...@.
		0030  00 00 00 00 00 00 00 00  00 02 00 00 00 00 00 00   ........ ........
	*/

	dev.ControlTimeout = time.Second

	log.Printf("Putting the device in programming mode I think?")
	buf := make([]byte, 64)
	_, err = dev.Control(0xc0, 185, 0x0021, 0, buf)
	// _, err = dev.Control(0x40, 185, 0x0021, 0, make([]byte, 0))
	if err != nil {
		log.Printf("\nfailed to send programming frame? %s", err.Error())
	}

	fmt.Print("FPG")
	for {
		buf := make([]byte, 64)
		// we are bitstreaming 64 bytes at a time
		n, err := bitstreamFirmware.Read(buf)
		if err != nil {
			break
		}

		_, err = dev.Control(0x40, 185, 0x0023, 512, buf[:n])
		if err != nil {
			log.Printf("\nfailed to bitstream %s", err.Error())
		}

		// bitStreamEndpoint.Write(buf[:n])
		fmt.Print("A")
	}

	time.Sleep(time.Hour)
	// // Open an OUT endpoint.
	// ep, err := intf.OutEndpoint(7)
	// if err != nil {
	// 	log.Fatalf("%s.OutEndpoint(7): %v", intf, err)
	// }

	// // Generate some data to write.
	// data := make([]byte, 5)
	// for i := range data {
	// 	data[i] = byte(i)
	// }

	// // Write data to the USB device.
	// numBytes, err := ep.Write(data)
	// if numBytes != 5 {
	// 	log.Fatalf("%s.Write([5]): only %d bytes written, returned error is %v", ep, numBytes, err)
	// }
	// fmt.Println("5 bytes successfully sent to the endpoint")

}
