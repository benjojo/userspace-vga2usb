package main

import (
	"math"
)

type xy struct {
	X, Y int
}

var payloadSizeSamples = map[int]xy{
	480000:  xy{X: 800, Y: 600},
	614400:  xy{X: 1024, Y: 600},
	786432:  xy{X: 1024, Y: 768},
	768000:  xy{X: 1280, Y: 600},
	921600:  xy{X: 1280, Y: 720},
	983040:  xy{X: 1280, Y: 768},
	1310720: xy{X: 1280, Y: 1024},
	1049088: xy{X: 1366, Y: 768},
	307200:  xy{X: 640, Y: 480},
}

func GuessTheRes(size int) xy {
	bestGuess := xy{X: 0, Y: 0}
	closeNess := 10000000
	for bytes, res := range payloadSizeSamples {
		close := int(math.Abs(float64((bytes)*3 - size)))
		// fmt.Printf("%#v is close by %d - best close is %d\n", res, close, closeNess)
		if close < closeNess {
			bestGuess = res
			closeNess = close
		}
	}

	return bestGuess
}
