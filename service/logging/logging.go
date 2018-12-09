package logging

import (
	"io"
	"log"
	"os"
)

var mux io.Writer = io.MultiWriter(os.Stdin)

func AddLog(w io.Writer) {
	mux = io.MultiWriter(mux, w)
	log.SetOutput(mux)
}

func Broadcast(r io.Reader) {
	io.Copy(mux, r)
}
