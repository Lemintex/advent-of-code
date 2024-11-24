package main

import (
	"fmt"
	"log"
	"os"
	"strings"
)

var input []string

// only for getting the input into []string format
// put input parsing code in ParseInput()
func ReadInput() {
	s, err := os.ReadFile("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	input = strings.Split(in, "\n")
}

func main() {
	fmt.Println("Part 1:", Part1())
	fmt.Println("Part 2:", Part2())
}

// do all the stuff related to handling the input here
func ParseInput() {

}

func Part1() int {
	total := 0
	return total
}

func Part2() int {
	total := 0
	return total
}
