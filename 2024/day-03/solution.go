package main

import (
	"fmt"
	"log"
	"os"
	"regexp"
	"strconv"
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
	aoc1Parsed := ParsePart1(input) // use these
	aoc2Parsed := ParsePart2(input)
	aoc1, aoc1Time := Part1(aocParsed)
	// bb1, bb1Time  := Part1()
	aoc2, aoc2Time := Part2(aoc2Parsed)
	// bb2, bb2Time := Part2()
	fmt.Println("==============================================")
	fmt.Println("Part 1:", aoc1, aoc1Time)
	// fmt.Println("Big Boy Part 1:", bb1)
	fmt.Println("Part 2:", aoc2, aoc2Time)
	// fmt.Println("Big Boy Part 2:", bb2)
}

func ParsePart1(in []string) [][]string { // return 2 parsed inputs
	reg, err := regexp.Compile(`mul\(\d{1,3},\d{1,3}\)`)
	if err != nil {
		fmt.Println(err)
	}
	ret := make([]string, 0)
	for _, s := range input {
		ret = append(ret, reg.FindAllString(s, -1)...)
	}
	arr := make([][]string, 0)
	for _, s := range ret {
		numsReg := regexp.MustCompile(`[0-9]{1,3}`)
		nums := numsReg.FindAllString(s, -1)
		arr = append(arr, nums)
	}

	return arr
}

func ParsePart2(in []string) [][]string { // return 2 parsed inputs
	reg, err := regexp.Compile(`mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\)`)
	if err != nil {
		fmt.Println(err)
	}
	ret := make([]string, 0)
	for _, s := range input {
		if s[0] == 'u' {

			s = "m" + s
		}
		ret = append(ret, reg.FindAllString(s, -1)...)
	}
	arr := make([][]string, 0)
	for _, s := range ret {
		if s[0] == 'd' {
			arr = append(arr, []string{s})
			continue
		}
		numsReg := regexp.MustCompile(`[0-9]{1,3}`)
		nums := numsReg.FindAllString(s, -1)
		arr = append(arr, nums)
	}
	return arr
}

func Part1(s [][]string) (int, time.Duration) {
	start := time.Now()
	ans := 0

	for _, str := range s {
		a, _ := strconv.Atoi(str[0])
		b, _ := strconv.Atoi(str[1])
		ans += a * b
	}
	return ans, time.Since(start)
}

func Part2(s [][]string) (int, time.Duration) {
	start := time.Now()
	ans := 0
	do := true
	for _, str := range s {
		if str[0] == "do()" {
			do = true
			continue
		} else if str[0] == "don't()" {
			do = false
			continue
		}
		if !do {
			continue
		}
		a, _ := strconv.Atoi(str[0])
		b, _ := strconv.Atoi(str[1])
		ans += a * b
	}
	return ans, time.Since(start)
}
