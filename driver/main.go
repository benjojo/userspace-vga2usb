package main

import (
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/png"
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

		dev.ControlTimeout = time.Second
		Bench := time.Now()
		inBuf := make([]byte, 12)
		dev.Control(0xc0, 177, 0, 0, inBuf)
		dev.ControlTimeout = time.Hour

		if time.Since(Bench) > time.Second ||
			(inBuf[0] == 0x00 && inBuf[1] == 0x00 && inBuf[2] == 0x00 && inBuf[3] == 0x00) {
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
		}

	} else {
		log.Fatalf("Set a -stage1 or -stage2")
	}

	fmt.Print("\r\n")
	log.Printf("Activating in 2 seconds")
	time.Sleep(time.Second * 2)

	log.Printf("Setting FPGA registers I think?...")

	for n, packet := range frameSetup {
		if n%4 == 0 {
			thrd := len(frameSetup) / 4
			switch n / thrd {
			case 0:
				fmt.Print("W")
			case 1:
				fmt.Print("O")
			case 2:
				fmt.Print("R")
			case 3:
				fmt.Print("K")
			}
		}

		_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
		if err != nil {
			log.Printf("\nfailed to setup device %s", err.Error())
		}
	}

	//  unable to grab default interface:
	// vid=5555,pid=3382,bus=3,addr=63,config=1,if=0,alt=0
	// does not have endpoint with address 0x80.
	// Available endpoints: [0x82(2,IN)]

	cfg, err := dev.Config(1)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab Config(1): %s", err.Error())
	}
	fmt.Print("Config: ")

	defaultinterface, err := cfg.Interface(0, 0)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab Interface(0, 0): %s", err.Error())
	}
	fmt.Print("Interface: ")

	// defaultInterface, done, err := dev.DefaultInterface()
	// defer done()
	// if err != nil {
	// 	log.Fatalf("Kaboom, unable to grab default interface: %s", err.Error())
	// }

	inputTest, err := defaultinterface.InEndpoint(2)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab default interface: %s", err.Error())
	}

	fmt.Print("Endpoint: ")

	// buf := make([]byte, 39)
	// _, err = dev.Control(0x40, 176, 0, 0, buf)
	// fmt.Print("Null 39 Control: ")

	inBuf := make([]byte, 12)
	_, err = dev.Control(0xc0, 177, 0, 0, inBuf)
	fmt.Print("Grab In 12 Control: \n")

	log.Printf("Here is a blob that I don't know what it does %x", inBuf)

	// now to send what i think activates this

	bytesRead := 0

	//                                  "\x04\x10\x00\x00\x08\x14\x04\x9e\x9f\x9e\x1f\x1f\x1f\x03\x03\x20\x00\xc8\x00\x01\x00\xc8\x00\x98\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x03\x20\x00\xc8"
	//	                                "\x04\x10\x00\x00\x08\x14\x04\x9b\x9e\x9e\x1f\x1f\x1f\x03\x03\x20\x02\x58\x00\x01\x02\x58\x00\x7c\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x03\x20\x02\x58"

	// dev.Control(0x40, 176, 0, 0, []byte("\x04\x10\x00\x00\x08\x14\x1c\xba\xba\xbb\x1f\x1f\x1f\x03\x03\x20\x02\x58\x00\x01\x02\x58\x00\x98\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x03\x20\x02\x58"))
	frameRequest := make(chan bool)
	frameOutput := make(chan ReadSkipper)
	go func() {
		for {
			<-frameRequest

			dev.Control(0x40, 176, 0, 0, []byte("\x04\x10\x00\x00\x08\x14\x1c\x90\x91\x91\x1f\x1f\x1f\x03\x03\x20\x02\x58\x00\x01\x02\x58\x00\x7c\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x03\x20\x02\x58"))
			dev.Control(0x40, 184, 0x0076, 0, []byte(""))

			dataBuf := make([]byte, 1280*1024*4)
			n, err := inputTest.Read(dataBuf)

			if err != nil {
				log.Printf("failed to read aa %s", err)
				// continue
			}

			bytesRead += n

			f, _ := os.Create("./debug")
			f.Write(dataBuf[:n])
			fmt.Printf("N=%d\n", n)

			rSk := ReadSkipper{
				Data:  dataBuf[:n],
				Bread: 4,
			}
			fmt.Printf("Best Guess %#v\n", GuessTheRes(n))
			frameOutput <- rSk

		}
	}()

	go func() {
		for {
			rSk := <-frameOutput

			Res := GuessTheRes(len(rSk.Data))
			img := image.NewNRGBA(image.Rect(0, 0, Res.X, Res.Y))
			// log.Printf("len= %d vs %d", len(PayloadB), 800*600)
			x, y := 0, 0
			// debugf, _ := os.Create("debug-hex")
			// rSk := ReadSkipper{
			// 	Data:  PayloadB,
			// 	Bread: 4,
			// }

			bench := time.Now()

			for {

				RGB := rSk.ReadPixel()
				R, G, B := color.YCbCrToRGB(RGB[0], RGB[1], RGB[2])
				img.Set(x, y, color.NRGBA{
					R: B,
					G: G,
					B: R,
					A: 255,
				})

				// debugf.WriteString(fmt.Sprintf("%d: %02x %02x %02x \n", y, RGB[0], RGB[1], RGB[2]))

				x++
				if x == Res.X {
					x = 0
					y++
				}
				if y == Res.Y {
					break
				}
			}

			log.Printf("took %s to process that", time.Since(bench).String())

			a, _ := os.Create(fmt.Sprintf("%d.png", time.Now().Unix()))
			png.Encode(a, img)
		}
	}()

	for {
		time.Sleep(time.Second)
		frameRequest <- true
	}

	// Now a vauge attempt to reconstruct into a image?
	// 01 30 01 00
	// PayloadB := PayloadB2
	// img := image.NewNRGBA(image.Rect(0, 0, 800, 600))
	// log.Printf("len= %d vs %d", len(PayloadB), 800*600)
	// x, y := 0, 0
	// // debugf, _ := os.Create("debug-hex")
	// rSk := ReadSkipper{
	// 	Data:  PayloadB,
	// 	Bread: 4,
	// }

	// bench := time.Now()

	// for {

	// 	RGB := rSk.ReadPixel()
	// 	R, G, B := color.YCbCrToRGB(RGB[0], RGB[1], RGB[2])
	// 	img.Set(x, y, color.NRGBA{
	// 		R: B,
	// 		G: G,
	// 		B: R,
	// 		A: 255,
	// 	})

	// 	// debugf.WriteString(fmt.Sprintf("%d: %02x %02x %02x \n", y, RGB[0], RGB[1], RGB[2]))

	// 	x++
	// 	if x == 800 {
	// 		x = 0
	// 		y++
	// 	}
	// 	if y == 600 {
	// 		break
	// 	}
	// }

	// log.Printf("took %s to process that", time.Since(bench).String())

	// a, _ := os.Create("lol.png")
	// png.Encode(a, img)

	log.Printf("Going to sleep, bye")
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

type ReadSkipper struct {
	Data  []byte
	Bread int
}

var debug = false

func (r *ReadSkipper) ReadPixel() [3]byte {
	var output [3]byte

	output[0] = r.readByte()
	output[1] = r.readByte()
	output[2] = r.readByte()
	return output
}

func (r *ReadSkipper) readByte() byte {
	r.Bread++
	if r.Bread%1028 == 0 {
		r.Bread += 4
	}

	if r.Bread > len(r.Data)-1 {
		return 0x00
	}

	return r.Data[r.Bread]
}
