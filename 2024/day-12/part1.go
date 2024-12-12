package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

type pos struct {
	r int
	c int
}

var input []string
var visited map[pos]struct{}

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
	visited = make(map[pos]struct{})
	ReadFile()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse(in []string) { //edit return type as needed
	// return some kind of data structure once the input has been parsed
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	for i, r := range input {
		for j, _ := range r {
			_, exists := visited[pos{r: i, c: j}]
			if exists {
				continue
			}
			visited[pos{r: i, c: j}] = struct{}{}
			area, perimeter := floodFill(pos{r: i, c: j})
			ans += area * perimeter
		}
	}

	return ans, time.Since(start)
}

func floodFill(init pos) (int, int) {
	area, perimeter := 1, 0
	plant := input[init.r][init.c]
	var directions [4]pos
	directions = [4]pos{pos{r: 1, c: 0}, pos{r: -1, c: 0}, pos{r: 0, c: 1}, pos{r: 0, c: -1}}

	var flood func(pos)
	flood = func(p pos) {
		for _, d := range directions {
			r, c := p.r+d.r, p.c+d.c
			newPos := pos{r: r, c: c}
			if r < 0 || r >= len(input) || c < 0 || c >= len(input[0]) {
				perimeter++
				continue
			}
			_, exists := visited[newPos]
			if plant != input[r][c] {
				perimeter++
			} else if !exists {
				area++
				visited[newPos] = struct{}{}
				flood(newPos)
			}
		}
	}
	flood(init)
	return area, perimeter
}
