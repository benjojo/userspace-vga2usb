package main

import (
	"log"
	"net/http"

	"golang.org/x/net/websocket"
)

var globalPNGBroker *pngBroker

func startWebserver(frameFeed chan []byte) {
	log.Print("Listening on :3723 for HTTP/Websocket connections")

	http.HandleFunc("/", serveRootPage)
	http.Handle("/ws", websocket.Handler(handlePNGwebsocket))

	globalPNGBroker = newpngBroker()
	go globalPNGBroker.Start()

	go func() {
		for {
			if *debugLogging {
				log.Printf("Waiting for frame")
			}
			frame := <-frameFeed
			globalPNGBroker.Publish(frame)
			if *debugLogging {
				log.Printf("Published frame(!)")
			}
		}
	}()

	err := http.ListenAndServe(":3723", nil)
	if err != nil {
		log.Fatalf("failed to listen on 3723, %s", err.Error())
	}
}

func serveRootPage(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte(webPage))
}

func handlePNGwebsocket(ws *websocket.Conn) {
	inputch := globalPNGBroker.Subscribe()
	defer globalPNGBroker.Unsubscribe(inputch)

	bframe, _ := ws.NewFrameWriter(websocket.BinaryFrame)

	for {
		frame := <-inputch

		_, err := bframe.Write(frame.([]byte))
		if err != nil {
			break
		}
	}
}

/*
	History suggests that I (Ben Cartwright-Cox) has never written a channel
	broadcaster without messing it up, so I will instead copy and paste one
	off Stackoverflow (https://stackoverflow.com/questions/36417199/how-to-broadcast-message-using-channel)
	instead of suffering more than I already do writing these kinds of programs.
*/

type pngBroker struct {
	stopCh    chan struct{}
	publishCh chan interface{}
	subCh     chan chan interface{}
	unsubCh   chan chan interface{}
}

func newpngBroker() *pngBroker {
	return &pngBroker{
		stopCh:    make(chan struct{}),
		publishCh: make(chan interface{}, 1),
		subCh:     make(chan chan interface{}, 1),
		unsubCh:   make(chan chan interface{}, 1),
	}
}

func (b *pngBroker) Start() {
	subs := map[chan interface{}]struct{}{}
	for {
		select {
		case <-b.stopCh:
			return
		case msgCh := <-b.subCh:
			subs[msgCh] = struct{}{}
		case msgCh := <-b.unsubCh:
			delete(subs, msgCh)
		case msg := <-b.publishCh:
			for msgCh := range subs {
				// msgCh is buffered, use non-blocking send to protect the broker:
				select {
				case msgCh <- msg:
				default:
				}
			}
		}
	}
}

func (b *pngBroker) Stop() {
	close(b.stopCh)
}

func (b *pngBroker) Subscribe() chan interface{} {
	msgCh := make(chan interface{}, 5)
	b.subCh <- msgCh
	return msgCh
}

func (b *pngBroker) Unsubscribe(msgCh chan interface{}) {
	b.unsubCh <- msgCh
}

func (b *pngBroker) Publish(msg interface{}) {
	b.publishCh <- msg
}
