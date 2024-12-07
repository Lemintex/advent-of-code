package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

var input []string
var obstacleMap map[[2]int]struct{}
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
	obstacleMap = make(map[[2]int]struct{})
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
	ans := 0
	steps := GetSteps(posX, posY)
	for k, _ := range steps {
		x, y := k[0], k[1]
		if input[x][y] != '.' {
			continue
		}
		dirQueue := [][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
		if DoesObstacleCauseLoop(dirQueue, posX, posY, x, y) {
			ans++
		}
	}
	fmt.Println(len(steps))
	return ans, time.Since(start)
}

func GetSteps(sx, sy int) map[[2]int]struct{} {
	steps := make(map[[2]int]struct{})
	steps[[2]int{sx, sy}] = struct{}{}
	dirQueue := [][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
	dir, dirQueue := PopAndReque(dirQueue)
	for {
		fmt.Println(posX, posY)
		_, exists := steps[[2]int{sx, sy}]
		if !exists {
			steps[[2]int{sx, sy}] = struct{}{}
		}

		if sx < 0 || sy < 0 || sx >= len(input) || sy >= len(input[0]) {
			return steps
		}
		fmt.Println(string(input[posX+dir[0]][posY+dir[1]]))
		if !(sx+dir[0] >= 0 && sy+dir[1] >= 0 && sx+dir[0] < len(input) && sy+dir[1] < len(input[0])) {
			return steps
		}
		if input[sx+dir[0]][sy+dir[1]] == '#' {
			fmt.Println(dir)
			dir, dirQueue = PopAndReque(dirQueue)
			fmt.Println(dir)
		}

		sx += dir[0]
		sy += dir[1]
	}
	return steps
}

func PopAndReque(queue [][2]int) ([2]int, [][2]int) {
	front := queue[0]
	queue = queue[1:]
	queue = append(queue, front)
	return front, queue
}

// getting 1662, answer is 1663, must figure out why
func DoesObstacleCauseLoop(dirQueue [][2]int, sx, sy, obsX, obsY int) bool {
	visited := make(map[[2]int]map[[2]int]struct{})
	visited[[2]int{sx, sy}] = make(map[[2]int]struct{})
	dir := [2]int{-1, 0}
	fmt.Println("Checking loop")
	i := 0
	for {

		if sx + dir[0] < 0 || sy + dir[1] < 0 || sx + dir[0] >= len(input) || sy + dir[1] >= len(input[0]) {
			return false
		}
		if input[sx+dir[0]][sy+dir[1]] == '#' || (sx+dir[0] == obsX && sy+dir[1] == obsY) {
			dir, dirQueue = PopAndReque(dirQueue)
			continue
		}
		_, exists := visited[[2]int{sx, sy}][[2]int{dir[0], dir[1]}]
		if !exists {
			_, obsExists := obstacleMap[[2]int{obsX, obsY}]
			if obsExists {
				return false
			}
			i++
			visited[[2]int{sx, sy}] = make(map[[2]int]struct{})
			visited[[2]int{sx, sy}][[2]int{dir[0], dir[1]}] = struct{}{}
		} else {
			obstacleMap[[2]int{obsX, obsY}] = struct{}{}
			return true
		}

		// fmt.Println(string(input[posX+dir[0]][posY+dir[1]]))

		sx += dir[0]
		sy += dir[1]
	}
	return false

}

// 1643 too low
// 1669 too high
// 1662 wrong
