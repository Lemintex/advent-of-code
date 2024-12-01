package main

import (
	"fmt"
	"log"
	"os"
	"strings"
)

var input []string
var bigboy []string

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

func ReadBigBoy() {
	s, err := os.ReadFile("bigboy.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	bigboy = strings.Split(in, "\n")
}

func main() {
	p1, bb1 := Part1()
	p2, bb2 := Part2()
	fmt.Println("Part 1:", p1)
	fmt.Println("Big Boy Part 1:", bb1)
	fmt.Println("Part 2:", p2)
	fmt.Println("Big Boy Part 2:", bb2)
}

// do all the stuff related to handling the input here
func ParseInput() {

}

func Part1() (int, int) {
	part1, bigboy := 0, 0
	return part1, bigboy
}

func Part2() (int, int) {
	part2, bigboy := 0, 0
	return part2, bigboy
}
