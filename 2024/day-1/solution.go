package main

import (
	"fmt"
	"log"
	"math"
	"os"
	"sort"
	"strconv"
	"strings"
	"time"
)

var input []string
var bigboy []string

var l, r []int
var bl, br []int

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
	for _, s := range input {
		leftID, rightID, _ := strings.Cut(s, "   ")
		left, err := strconv.Atoi(leftID)
		if err != nil {
			return
		}
		right, err := strconv.Atoi(rightID)
		if err != nil {
			return
		}
		l = append(l, left)
		r = append(r, right)
	}
}

func ParseBigBoy() {
	for _, s := range bigboy {
		leftID, rightID, _ := strings.Cut(s, "   ")
		left, err := strconv.Atoi(leftID)
		if err != nil {
			return
		}
		right, err := strconv.Atoi(rightID)
		if err != nil {
			return
		}
		bl = append(bl, left)
		br = append(br, right)
	}
}

func Part1() (int, int64) {
	return Part1AOC(), Part1BigBoy()
}

func Part1AOC() int {
	start := time.Now()
	ans := 0
	sort.Ints(l)
	sort.Ints(r)
	for i := range l {
		dist := float64(l[i]-r[i])
		ans += int(math.Abs(dist))
	}
	fmt.Println("AOC Part 1:", time.Since(start))
	return ans
}

func Part1BigBoy() int64 {
	start := time.Now()
	ans := int64(0)
	sort.Ints(bl)
	sort.Ints(br)
	for i := range bl {
		dist := float64(bl[i]-br[i])
		ans += int64(math.Abs(dist))
	}
	fmt.Println("Big Boy Part 1:", time.Since(start))
	return ans
}

func Part2() (int, int64) {
	return Part2AOC(), Part2BigBoy()
}

func Part2AOC() int {
	start := time.Now()
	ans := 0
	occurrences := make(map[int]int)
	for _, n := range r {
		occurrences[n]++
	}
	for _, n := range l {
		ans += n * occurrences[n]
	}
	fmt.Println("AOC Part 2:", time.Since(start))
	return ans
}

func Part2BigBoy() int64 {
	start := time.Now()
	ans := int64(0)
	occurrences := make(map[int]int)
	for _, n := range br {
		occurrences[n]++
	}
	for _, n := range bl {
		ans += int64(n * occurrences[n])
	}
	fmt.Println("Big Boy Part 2:", time.Since(start))
	return ans
}
