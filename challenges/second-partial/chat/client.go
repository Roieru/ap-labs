// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"flag"
)

//!+
func main() {

	var username string
	flag.StringVar(&username, "user", "DefaultUsername", "Your username")

	var serverAdd string
	flag.StringVar(&serverAdd, "server", "localhost:8000", "The server's address")

	flag.Parse()

	conn, err := net.Dial("tcp", serverAdd)
	if err != nil {
		log.Fatal(err)
	}
	io.WriteString(conn,username)
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("You've been disconnected from the IRC server")
		os.Exit(1)
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
