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

func ReadFiles() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}

	bigboy, err = Read("bigboy.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
}

func Read(filename string) ([]string, error) {
	s, err := os.ReadFile(filename)
	if err != nil {
		log.Fatal(err)
		return nil, err
	}
	in := strings.TrimSpace(string(s))

	// depending on the grouping of the input we may want to change the seperator
	return strings.Split(in, "\n"), nil
}

func main() {
	ReadFiles()
	aocParsed, bigboyParsed := ParseInputs() // use these
	aoc1, aoc1Time := Part1()
	bb1, bb1Time  := Part1()
	aoc2, aoc2Time := Part2()
	bb2, bb2Time := Part2()
	fmt.Println("==============================================")
	fmt.Println("Part 1:", aoc1)
	fmt.Println("Big Boy Part 1:", bb1)
	fmt.Println("Part 2:", aoc2)
	fmt.Println("Big Boy Part 2:", bb2)
}


func ParseInputs() {// return 2 parsed inputs

}

func Parse(in []string) {//edit return type as needed
	// return some kind of data structure once the input has been parsed
}

func Part1() (int, time.Duration) {
	start := time.Now()
	ans := 0

	return ans, time.Since(start)
}

func Part2() (int, time.Duration) {
	start := time.Now()
	ans := 0

	return ans, time.Since(start)
}
