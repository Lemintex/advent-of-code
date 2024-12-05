package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var input []string
var partOfMAS map[[2]int]struct{}

func ReadFile() {
	var err error
	input, err = Read("input.txt")
	if err != nil {
		log.Fatal(err)
		return
	}
	partOfMAS = make(map[[2]int]struct{})
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
	ReadFile()
	answer, time := Part2()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Part2() (int, time.Duration) {
	start := time.Now()
	ans := 0

	for i := range input {
		for j := range input[i] {
			ans += SearchForWord(i, j, "MAS")
		}
	}
	return ans, time.Since(start)
}

func SearchForWord(i, j int, word string) int {
	ans := 0
	directions := [][]int{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
	for _, d := range directions {
		ax, ay := 0, 0
		x, y := i, j
		found := true
		exists := true
		for _, c := range word {
			if x < 0 || y < 0 || x >= len(input) || y >= len(input[0]) {
				found = false
				break
			}
			if input[x][y] != byte(c) {
				found = false
				break
			}
			if input[x][y] == 'A' {
				ax, ay = x, y
				_, exists = partOfMAS[[2]int{ax, ay}]
			}
			x += d[0]
			y += d[1]
		}
		if found && exists {
			ans++
		} else if found && !exists {
			partOfMAS[[2]int{ax, ay}] = struct{}{}
		}
	}
	return ans
}
