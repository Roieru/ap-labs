package main

import (
   	"io/ioutil"
	"flag"
	"time"
	"strconv"
	"fmt"
)

var pipeLimit int

func link(from, to chan int){
	l := <- from
	to <- l
}

func main(){

	flag.IntVar(&pipeLimit,"pipes",1000000,"Max number of pipes")

	flag.Parse()

	fmt.Println("Generating " + strconv.Itoa(pipeLimit) + " pipe stages.")

	start := make(chan int)
	input := start
	output := make(chan int)

	for i := 0; i <= pipeLimit; i++{
		go link(input,output)
		input = output
		output = make(chan int)
	}

	end := input

	startTime := time.Now()

	start <- 1

	endOutput := <-end

	result := "Excercise 9.4 report\n==========\nFinal output was: " + strconv.Itoa(endOutput) + "\nIt took " + time.Since(startTime).String() + " to go through " + strconv.Itoa(pipeLimit) + " stages.\n==========\n"

	close(start)
	close(output)
	close(end)

	d1 := []byte(result)
	ioutil.WriteFile("report94.txt",d1,0644)

	fmt.Println("The process has finished. The report was generated at report94.txt")

}
