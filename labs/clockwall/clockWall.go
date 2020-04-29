package main

import (
	//"io"
	"log"
	"net"
	"os"
	"fmt"
	"bufio"
	"strings"
	"strconv"
)

func main() {
	done := make(chan int)
	for i := 1; i < len(os.Args); i++{
		go getClock(os.Args[i], done)
	}

	x := 1
	x = <-done // wait for background goroutine to finish
	log.Println("Channel Closed with value: ", x)
	close(done)
}

func getClock(clock string, done chan int) {
	split := strings.Split(clock, "=")
	if(len(split) != 2){
		fmt.Println("Error. Invalid argument "+clock)
		return
	}
	city := split[0]
	split2 := strings.Split(split[1], ":")
	port,_ := strconv.Atoi(split2[1])
	if (port > 0 && port <= 65535) || split2[1] == "0"{
		conn, err := net.Dial("tcp",split[1])
		if err != nil{
			return
		}
		scanner := bufio.NewScanner(conn)
		for scanner.Scan(){
			fmt.Println(city+"\t:\t"+scanner.Text())
		}
		log.Println("done")
		done <- 2
	} else {
		fmt.Println("Error. Invalid port "+split2[1])
		return
	}
}
