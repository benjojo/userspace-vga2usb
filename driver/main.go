package main

import (
	"bytes"
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/jpeg"
	"image/png"
	"log"
	"os"
	"time"

	"github.com/google/gousb"
)

var debugLogging = flag.Bool("debug", false, "Enable debug output")

func main() {
	stage1 := flag.Bool("stage1", false, "USB mode")
	stage2 := flag.Bool("stage2", false, "FPGA mode and run time")
	resMode := flag.Int("resMode", 1, "1: 800*600, 2: 1024x768, 3: 1280x720")
	fps := flag.Int("fps", 2, "FPS you want to try and capture at")
	mjpegMode := flag.Bool("mjpeg", false, "output MJPEG so it can be processed by another program")
	webserver := flag.Bool("web", false, "if set, will start a webserver on 3723 with liveview and HTML5 recording")
	flag.Parse()

	ctx := gousb.NewContext()
	defer ctx.Close()

	if *debugLogging {
		log.Printf("Opening device...")
	}

	dev, err := ctx.OpenDeviceWithVIDPID(0x5555, 0x3382)
	if err != nil || dev == nil {
		log.Fatalf("Could not open a device: %v", err)
	}

	if *debugLogging {
		log.Printf("Claim default interface...")
	}

	deviceConfig, err := dev.Config(1)
	if err != nil {
		log.Printf("Failed to get config for device")
		return
	}
	defer dev.Close()

	if *debugLogging {
		for num, interf := range deviceConfig.Desc.Interfaces {
			os.Stderr.WriteString(fmt.Sprintf("\tINTERFACE #%d\n", num))

			for numAlt, altset := range interf.AltSettings {
				os.Stderr.WriteString(fmt.Sprintf("\t\t%d - Number: %d - Alt %d\n", numAlt, altset.Number, altset.Alternate))
				for numEndpoint, endpointset := range altset.Endpoints {
					os.Stderr.WriteString(fmt.Sprintf("\t\t\t%d - %#v\n", numEndpoint, endpointset))
				}
			}
		}
	}

	if *stage1 {
		log.Printf("Setting the USB Controller firmware")

		for n, packet := range usbInit {
			if *debugLogging {
				if n%10 == 0 {
					thrd := len(usbInit) / 3
					switch n / thrd {
					case 0:
						os.Stderr.WriteString(fmt.Sprintf("U"))
					case 1:
						os.Stderr.WriteString(fmt.Sprintf("S"))
					case 2:
						os.Stderr.WriteString(fmt.Sprintf("B"))
					}
				}
			}

			_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
			if err != nil {
				log.Printf("\nfailed to bitstream %s", err.Error())
			}
		}

		log.Printf("Set the USB Controller firmware")
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
				if *debugLogging {
					if n%60 == 0 {
						thrd := len(fpgaInit) / 4
						switch n / thrd {
						case 0:
							os.Stderr.WriteString(fmt.Sprintf("F"))
						case 1:
							os.Stderr.WriteString(fmt.Sprintf("P"))
						case 2:
							os.Stderr.WriteString(fmt.Sprintf("G"))
						case 3:
							os.Stderr.WriteString(fmt.Sprintf("A"))
						}
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

	if *debugLogging {
		os.Stderr.WriteString(fmt.Sprintf("\r\n"))
		log.Printf("Activating in 2 seconds")
	}
	time.Sleep(time.Second * 1)

	if *debugLogging {
		log.Printf("Setting FPGA registers I think?...")
	}

	for n, packet := range frameSetup {
		if *debugLogging {
			if n%4 == 0 {
				thrd := len(frameSetup) / 4
				switch n / thrd {
				case 0:
					os.Stderr.WriteString(fmt.Sprintf("W"))
				case 1:
					os.Stderr.WriteString(fmt.Sprintf("O"))
				case 2:
					os.Stderr.WriteString(fmt.Sprintf("R"))
				case 3:
					os.Stderr.WriteString(fmt.Sprintf("K"))
				}
			}
		}

		_, err = dev.Control(packet.RequestType, packet.BRequest, packet.WValue, packet.WIndex, packet.Data)
		if err != nil {
			log.Printf("\nfailed to setup device %s", err.Error())
		}
	}

	cfg, err := dev.Config(1)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab Config(1): %s", err.Error())
	}
	if *debugLogging {
		os.Stderr.WriteString(fmt.Sprintf("Config: "))
	}

	defaultinterface, err := cfg.Interface(0, 0)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab Interface(0, 0): %s", err.Error())
	}
	if *debugLogging {
		os.Stderr.WriteString(fmt.Sprintf("Interface: "))
	}

	inputTest, err := defaultinterface.InEndpoint(2)
	if err != nil {
		log.Fatalf("Kaboom, unable to grab default interface: %s", err.Error())
	}
	if *debugLogging {
		os.Stderr.WriteString(fmt.Sprintf("Endpoint: "))
	}

	inBuf := make([]byte, 12)
	_, err = dev.Control(0xc0, 177, 0, 0, inBuf)
	os.Stderr.WriteString(fmt.Sprintf("Grab In 12 Control: \n"))

	if *debugLogging {
		log.Printf("Here is a EDID blob I think? %x", inBuf)
	}

	// now to send what i think activates this

	frameRequest := make(chan bool)
	frameOutput := make(chan ReadSkipper, 1)
	go func() {
		for {
			<-frameRequest

			if *resMode == 3 {
				// 1280x720
				dev.Control(0x40, 176, 0, 0, []byte("\x06\x80\x00\x00\x08\x14\x08\xb9\xbc\xbb\x1f\x1f\x1f\x03\x05\x00\x02\xd0\x00\x01\x02\xd0\x00\xe0\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x05\x00\x02\xd0"))
			} else if *resMode == 2 {
				// 1024x768
				dev.Control(0x40, 176, 0, 0, []byte("\x05\x40\x00\x00\x08\x14\x08\xbb\xbc\xbb\x1f\x1f\x1f\x03\x04\x00\x03\x00\x00\x01\x03\x00\x00\xa4\x22\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x04\x00\x03\x00"))
			} else {
				// 800x600
				dev.Control(0x40, 176, 0, 0, []byte("\x04\x10\x00\x00\x08\x14\x1c\x90\x91\x91\x1f\x1f\x1f\x03\x03\x20\x02\x58\x00\x01\x02\x58\x00\x7c\x19\x01\x01\x80\x80\x05\x00\x00\x00\x00\x00\x03\x20\x02\x58"))
			}
			dev.Control(0x40, 184, 0x0076, 0, []byte(""))

			dataBuf := make([]byte, 1280*1024*4)
			n, err := inputTest.Read(dataBuf)

			if err != nil {
				log.Printf("failed to read aa %s", err)
				// continue
			}

			if *debugLogging {
				f, _ := os.Create("./debug")
				f.Write(dataBuf[:n])
				os.Stderr.WriteString(fmt.Sprintf("N=%d\n", n))
			}
			rSk := ReadSkipper{
				Data:  dataBuf[:n],
				Bread: 4,
			}
			frameOutput <- rSk
		}
	}()

	if *mjpegMode {
		os.Stdout.WriteString("--myboundary\nContent-Type: image/jpeg\n\n")
	}

	pngPipe := make(chan []byte)
	if *webserver {
		go startWebserver(pngPipe)
	}

	go func() {
		for {
			rSk := <-frameOutput

			Res := GuessTheRes(len(rSk.Data))
			img := image.NewNRGBA(image.Rect(0, 0, Res.X, Res.Y))
			x, y := 0, 0

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

				x++
				if x == Res.X {
					x = 0
					y++
				}
				if y == Res.Y {
					break
				}
			}

			if !*mjpegMode {

				if *webserver {
					var tmpBuffer bytes.Buffer
					err := png.Encode(&tmpBuffer, img)
					if err != nil {
						log.Printf("failed to encode PNG %s", err.Error())
						continue
					}

					if *debugLogging {
						log.Printf("Send a buf of len %d down pngPipe", tmpBuffer.Len())
					}

					pngPipe <- tmpBuffer.Bytes()

				} else {
					a, _ := os.Create(fmt.Sprintf("%d.png", time.Now().Unix()))
					png.Encode(a, img)
				}

				if *debugLogging {
					log.Printf("took %s to process that", time.Since(bench).String())
				}

				continue
			}

			// MJPEG mode.
			// Mostly stolen from how I did it back in the days of de-ip-hdmi
			// https://github.com/benjojo/de-ip-hdmi/blob/master/main.go

			os.Stdout.WriteString("\n--myboundary\nContent-Type: image/jpeg\n\n")
			jpeg.Encode(os.Stdout, img, nil)

			if *debugLogging {
				log.Printf("took %s to process that", time.Since(bench).String())
			}

		}
	}()

	for {
		time.Sleep(time.Second / time.Duration(*fps))
		frameRequest <- true
	}
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
