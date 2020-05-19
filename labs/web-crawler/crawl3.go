// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"flag"
	"github.com/adonovan/gopl.io/ch5/links"
)

type DepthLink struct{
	depth int
	url string
}

var maxDepth int

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(dLink DepthLink) []DepthLink {
	fmt.Println(dLink.url)

	if(dLink.depth == maxDepth){
		return make([]DepthLink,0)
	}

	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(dLink.url)
	<-tokens // release the token

	ret := make([]DepthLink,len(list))

	for i := 0; i < len(list); i++{
		ret[i] = DepthLink{dLink.depth + 1, list[i]}
	}

	if err != nil {
		log.Print(err)
	}
	return ret
}

//!-sema

//!+
func main() {

	flag.IntVar(&maxDepth, "depth", 0, "Max depth for crawling")

	flag.Parse()

	worklist := make(chan []DepthLink)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	initLinks := flag.Args()
	firstDL := make([]DepthLink, len(initLinks))
	for i := 0; i < len(initLinks); i++{
		firstDL[i] = DepthLink{0, initLinks[i]}
	}
	go func() { worklist <- firstDL }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, dLink := range list {
			if !seen[dLink.url] && dLink.depth <= maxDepth{
				seen[dLink.url] = true
				n++
				go func(dLink DepthLink) {
					worklist <- crawl(dLink)
				}(dLink)
			}
		}
	}
}

//!-
