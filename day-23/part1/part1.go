package main

import (
	"fmt"
	"os"
	"strings"
)

var trail []string
func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("AAH")
	}
	trail = strings.Split(string(f), "\n")
	for _, t := range trail {
		fmt.Println(t)
	}
}
