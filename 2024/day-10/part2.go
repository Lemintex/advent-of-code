package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type pos struct {
	r int
	c int
}

var input []string
var trailHeads []pos


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
	for r, row := range input {
		for c, col := range row {
			if col == '0' {
				pos := pos{
					r: r,
					c: c,
				}
				trailHeads = append(trailHeads, pos)
			}
		}
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0

	for _, p := range trailHeads {
		ans += FindTrailHeadScore(p)
	}
	return ans, time.Since(start)
}

func FindTrailHeadScore(p pos) int {
	peaks := make(map[[10]pos]struct{})
	dirs := [4]pos{pos{r: 0, c: 1}, pos{r: 0, c: -1}, pos{r: 1, c: 0}, pos{r: -1, c: 0}}
	var goThroughTrail func(int, pos, []pos)
	goThroughTrail = func(n int, position pos, path []pos) {
		path = append(path, position)
		if len(path) == 10 {
			peaks[[10]pos(path)] = struct{}{}
		}
		for _, d := range dirs {
			r, c := position.r+d.r, position.c+d.c
			
			if r < 0 || r >= len(input) || c < 0 || c >= len(input[0]) {
				continue
			}
			if input[r][c] == '.' {
				continue
			}
			h, err := strconv.Atoi(string(input[r][c]))
			if err != nil {
				log.Fatal(err)
			}
			if h != n + 1 {
				continue
			}
			newPos := pos{
				r: r,
				c: c,
			}
			goThroughTrail(n+1, newPos, path)
		}
	}
	var	path []pos
	goThroughTrail(0, p, path)
	return len(peaks)
}
