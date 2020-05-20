package main

import (
	"fmt"
	"time"
	"io/ioutil"
	"strconv"
)

func main() {

	fmt.Println("Generating channels for a one-second ping-pong")
	chan1 := make(chan int)
	chan2 := make(chan int)
	result := "Exercise 9.5 report\n==========\n"
	go link(chan1, chan2)
	go link(chan2, chan1)
	chan1 <- 1
	time.Sleep(1 * time.Second)
	select {
	case n := <-chan1:
		result += "A total of " + strconv.Itoa(n) + " messages were sent\n==========\n"
	case n := <-chan2:
		result += "A total of " + strconv.Itoa(n) + " messages were sent\n==========\n"
	}
	close(chan1)
	close(chan2)

	d1 := []byte(result)
	ioutil.WriteFile("report95.txt",d1,0664)

	fmt.Println("The process has finished. The report was generated at report95.txt")

}

func link(input, output chan int) {
	for val := range input {
		val++
		output <- val
	}
}
