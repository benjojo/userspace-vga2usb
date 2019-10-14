package main

import (
	"bufio"
	"encoding/hex"
	"flag"
	"fmt"
	"log"
	"os"
	"regexp"
	"strconv"
	"strings"
)

type usbControlPacket struct {
	ignore      bool
	RequestType uint8
	BRequest    uint8
	WValue      uint16
	WIndex      uint16
	Data        []byte
}

func main() {
	tcpdumpfile := flag.String("tcpdump", "", "the path to the tcpdump output")
	tsharkfile := flag.String("tshark", "", "the path to the tshark output")
	flag.Parse()

	tcpdumpfd, err := os.Open(*tcpdumpfile)
	if err != nil {
		log.Fatalf("Unable to open input text file %s", err.Error())
	}
	tsharkfd, err := os.Open(*tsharkfile)
	if err != nil {
		log.Fatalf("Unable to open input text file %s", err.Error())
	}

	tcpdumpbufio := bufio.NewReader(tcpdumpfd)
	tsharkbufio := bufio.NewReader(tsharkfd)

	for {
		uCP := usbControlPacket{
			Data: make([]byte, 0),
		}

		err := readTshark(tsharkbufio, &uCP)
		if err != nil {
			break
		}

		err = readTCPDump(tcpdumpbufio, &uCP)
		if err != nil {
			break
		}

		if !uCP.ignore {
			fmt.Printf("%#v\n", uCP)
		}

	}
}

// Frame 1: 64 bytes on wire (512 bits), 64 bytes captured (512 bits) on interface 0
var tsharkHeader = regexp.MustCompilePOSIX(`^Frame `)

// bmRequestType: 0x40
var tsharkbmRequestType = regexp.MustCompilePOSIX(`^    bmRequestType: 0x([0-9A-Fa-f]{2})`)

// bRequest: 160
var tsharkbRequest = regexp.MustCompilePOSIX(`^    bRequest: ([0-9]+)`)

// wValue: 0x25fd
var tsharkwValue = regexp.MustCompilePOSIX(`^    wValue: 0x([0-9A-Fa-f]{4})`)

// wIndex: 0 (0x0000)
var tsharkwIndex = regexp.MustCompilePOSIX(`^    wValue: ([0-9]+) `)

func readTshark(in *bufio.Reader, input *usbControlPacket) error {
	for {
		line, toobig, err := in.ReadLine()
		if err != nil {
			return err
		}
		if toobig {
			log.Fatalf("Packet too big")
		}

		// fmt.Printf("honk '%s'\n", line)
		if tsharkHeader.Match(line) {
			return nil
		}

		sline := string(line)
		if strings.Contains(sline, "URB_COMPLETE") {
			// It's an ack, who cares
			input.ignore = true
		}

		if tsharkbmRequestType.Match(line) {
			result := extractHexNumberOutOfRegexMatch(line, tsharkbmRequestType)
			input.RequestType = uint8(result)
		}

		if tsharkbRequest.Match(line) {
			result := extractNumberOutOfRegexMatch(line, tsharkbRequest)
			input.BRequest = uint8(result)
		}

		if tsharkwValue.Match(line) {
			result := extractHexNumberOutOfRegexMatch(line, tsharkwValue)
			input.WValue = uint16(result)
		}

		if tsharkwIndex.Match(line) {
			result := extractNumberOutOfRegexMatch(line, tsharkwIndex)
			input.WIndex = uint16(result)
		}

	}
}

func extractHexNumberOutOfRegexMatch(data []byte, re *regexp.Regexp) int {
	res := re.FindAllStringSubmatch(string(data), 1)
	i, err := strconv.ParseInt(res[0][1], 16, 64)
	if err != nil {
		log.Fatalf("blah")
	}

	return int(i)
}

func extractNumberOutOfRegexMatch(data []byte, re *regexp.Regexp) int {
	res := re.FindAllStringSubmatch(string(data), 1)
	i, err := strconv.ParseInt(res[0][1], 10, 64)
	if err != nil {
		log.Fatalf("blah")
	}

	return int(i)
}

// 20:40:59.021126 CONTROL SUBMIT to 3:10:0
var tcpdumpHeader = regexp.MustCompilePOSIX(`^[0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]+ .+ to [0-9]+:[0-9]+:[0-9]+`)

// 	0x0000:  1201 0002 ffff ff40 5555 8233 0000 0000
var tcpdumpData = regexp.MustCompilePOSIX(`^	0x[0-9a-f]+:  (.+)`)

func readTCPDump(in *bufio.Reader, input *usbControlPacket) error {
	for {
		line, toobig, err := in.ReadLine()
		if err != nil {
			return err
		}
		if toobig {
			log.Fatalf("Packet too big")
		}

		if tcpdumpHeader.Match(line) {
			return nil
		}

		if tcpdumpData.Match(line) {
			spacedHex := tcpdumpData.FindAllStringSubmatch(string(line), 1)[0][1]
			// spacedHex now has '0193 60bc a3ff 543f 30e5 0954 1ffe e493'
			spacedHex = strings.Replace(spacedHex, " ", "", -1)
			// spacedHex now has '019360bca3ff543f30e509541ffee493'
			b, err := hex.DecodeString(spacedHex)
			if err != nil {
				log.Fatalf("shit this isnt hex? %s \n %s", err.Error(), spacedHex)
			}

			input.Data = append(input.Data, b...)
		}
	}
}
