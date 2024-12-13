package main

import (
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
	"time"
)

type vec2 struct {
	x float64
	y float64
}

type game struct {
	a vec2
	b vec2
	p vec2
}

var input []string
var games []game

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
	return strings.Split(in, "\n\n"), nil
}

func main() {
	ReadFile()
	Parse()
	answer, time := Solve()
	fmt.Println("Answer:", answer)
	fmt.Println("Time:", time)
}

func Parse() {
	format := func(r rune) rune {
		if (r >= '0' && r <= '9') || r == ',' {
			return r
		}
		return -1
	}
	for _, p := range input {
		strGame := strings.Split(p, "\n")
		buttonA := strings.Map(format, strGame[0])
		buttonB := strings.Map(format, strGame[1])
		prize := strings.Map(format, strGame[2])

		butA, butB, p := ParseVec2(buttonA), ParseVec2(buttonB), ParseVec2(prize)
		p.x += 10000000000000
		p.y += 10000000000000
		g := game{butA, butB, p}
		games = append(games, g)
	}
}

func ParseVec2(s string) vec2 {
	before, after, _ := strings.Cut(s, ",")
	a, err := strconv.Atoi(before)
	if err != nil {
		log.Fatal(err)
	}

	b, err := strconv.Atoi(after)
	if err != nil {
		log.Fatal(err)
	}
	v := vec2{
		x: float64(a),
		y: float64(b),
	}
	return v
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0

	// 2 equations:
	// ax*i + bx*j = px
	// ay*i + by*j = py

	// system of linear equations
	// ax*ay*j + bx*ay*j = px*ay
	// ax*ay*j + ax*by*j = py*ax

	//	bx*ay*j - ax*by*j = px*ay - py*ax
	//	j(bx*ay - ax*by) = px*ay - py*ax

	// j = (px*ay - py*ax)/(bx*ay - ax*by)

	// ax*i + bx*j = px
	// ax*i = px - bx*j
	// i = (px - bx*j) / ax
	for _, g := range games {
		a, b, p := g.a, g.b, g.p
		j := (p.x*a.y - p.y*a.x) / (b.x*a.y - a.x*b.y)
		i := (p.x - b.x*j) / a.x
		if math.Floor(i) == i && math.Floor(j) == j {
			ans += int(i)*3 + int(j)
		}
	}
	return ans, time.Since(start)
}
