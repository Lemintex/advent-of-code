package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

var input []string
var stones []int

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

	// depending on the grouping of the input we may want to change the seperator
	return strings.Split(in, " "), nil
}

func main() {
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	for _, s := range input {
		stone, err := strconv.Atoi(s)
		if err != nil {
			log.Fatal(err)
		}
		stones = append(stones, stone)
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	blinks := 25
	for range blinks {
		changeStones()
	}
	ans = len(stones)
	return ans, time.Since(start)
}

func changeStones() {
	var newStones []int
	newStones = make([]int, 0)
	for _, s := range stones {
		stone := s
		if s == 0 {
			stone = 1
			newStones = append(newStones, stone)
			continue
		}
		str := strconv.Itoa(s)
		if len(str)%2 == 0 {
			str := strconv.Itoa(stone)
			s1, s2 := str[:len(str)/2], str[len(str)/2:]
			stone1, err := strconv.Atoi(s1)
			if err != nil {
				log.Fatal(err)
			}

			stone2, err := strconv.Atoi(s2)
			if err != nil {
				log.Fatal(err)
			}
			newStones = append(newStones, stone1)
			newStones = append(newStones, stone2)
			continue
		}
		stone *= 2024
		newStones = append(newStones, stone)
	}
	stones = newStones
}
