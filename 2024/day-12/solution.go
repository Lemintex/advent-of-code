package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
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
	Read()
	Parse()
	aoc1, bb1 := Part1()
	aoc2, bb2 := Part2()
	fmt.Println("==============================================")
	fmt.Println("Part 1:", aoc1)
	fmt.Println("Big Boy Part 1:", bb1)
	fmt.Println("Part 2:", aoc2)
	fmt.Println("Big Boy Part 2:", bb2)
}

func Read() {
	ReadInput()
	ReadBigBoy()
}

func Parse() {
	ParseInput()
	ParseBigBoy()
}
// do all the stuff related to handling the input here

func ParseInput() {

}

func ParseBigBoy() {

}

func Part1() (int, int64) {
	return Part1AOC(), Part1BigBoy()
}

func Part1AOC() int {
	start := time.Now()
	ans := 0

	fmt.Println("AOC Part 1:", time.Since(start))
	return ans
}

func Part1BigBoy() int64 {
	start := time.Now()
	ans := int64(0)

	fmt.Println("Big Boy Part 1:", time.Since(start))
	return ans
}

func Part2() (int, int64) {
	return Part2AOC(), Part2BigBoy()
}

func Part2AOC() int {
	start := time.Now()
	ans := 0

	fmt.Println("AOC Part 2:", time.Since(start))
	return ans
}

func Part2BigBoy() int64 {
	start := time.Now()
	ans := int64(0)

	fmt.Println("Big Boy Part 2:", time.Since(start))
	return ans
}
