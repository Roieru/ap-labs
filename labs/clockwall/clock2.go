// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"fmt"
	"strconv"
)

func handleConn(c net.Conn) {
	defer c.Close()
	loc, _ := time.LoadLocation(os.Getenv("TZ"))
	for {
		_, err := io.WriteString(c, time.Now().In(loc).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	var listener net.Listener
	var err error

	if len(os.Args) == 1 {
		listener, err = net.Listen("tcp", "localhost:9090")
	} else if len(os.Args) >= 3 && os.Args[1] == "-port"{
		i,_ := strconv.Atoi(os.Args[2])
		if (i > 0 && i <= 65535) || os.Args[2] == "0"{
			fmt.Println("localhost:"+os.Args[2])
			listener, err = net.Listen("tcp","localhost:"+os.Args[2])
		} else{
			fmt.Println("Error. Invalid port number.")
			return
		}
	} else {
		fmt.Println("Error. Usage: clock2.go -port [portNo]")
		return
	}

	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}

