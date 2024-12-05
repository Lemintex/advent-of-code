package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var input []string

func ReadFile() {
	var err error
	input, err = Read("input.txt")
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

	return strings.Split(in, "\n"), nil
}

func main() {
	ReadFile()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for i := range input {
		for j := range input[i] {
			ans += SearchForWord(i, j, "XMAS")
		}
	}
	return ans, time.Since(start)
}

func SearchForWord(i, j int, word string) int {
	ans := 0
	directions := [][]int{{0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {-1, -1}}
	for _, d := range directions {
		x, y := i, j
		found := true
		for _, c := range word {
			if x < 0 || y < 0 || x >= len(input) || y >= len(input[0]) {
				found = false
				break
			}
			if input[x][y] != byte(c) {
				found = false
				break
			}
			x += d[0]
			y += d[1]
		}
		if found {
			ans++
		}
	}
	return ans
}
