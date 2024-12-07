package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var input []string
var posX, posY int

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
	return strings.Split(in, "\n"), nil
}

func main() {
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	for x, r := range input {
		for y, c := range r {
			if c == '^' {
				fmt.Println(x, y)
				posX, posY = x, y
				return
			}
		}
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := CountSteps()

	return ans, time.Since(start)
}

func CountSteps() int {
	n := 1
	steps := make(map[[2]int]struct{})
	dirQueue := [][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
	dir, dirQueue := PopAndReque(dirQueue)
	for posX + dir[0] >= 0 && posY + dir[1] >= 0 && posX + dir[0] < len(input) && posY + dir[1] < len(input[0]) {
		fmt.Println(posX, posY)
		_, exists := steps[[2]int{posX, posY}]
		if !exists {
			steps[[2]int{posX, posY}] = struct{}{}
			n++
		}

		if posX < 0 || posY < 0 || posX >= len(input) || posY >= len(input[0]) {
			return n
		}
		fmt.Println(string(input[posX + dir[0]][posY + dir[1]]))
		if input[posX + dir[0]][posY + dir[1]] == '#' {
			fmt.Println(dir)
			dir, dirQueue = PopAndReque(dirQueue)
			fmt.Println(dir)
		}
			
		posX += dir[0]
		posY += dir[1]
	}
	return n
}

func PopAndReque(queue [][2]int) ([2]int, [][2]int) {
	front := queue[0]
	queue = queue[1:]
	queue = append(queue, front)
	return front, queue
}
