package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	words := strings.Fields(s)
	ret := make(map[string]int)
	for i:=0; i<len(words); i++{
		ret[words[i]]++;
	}
	return ret
}

func main() {
	wc.Test(WordCount)
}

