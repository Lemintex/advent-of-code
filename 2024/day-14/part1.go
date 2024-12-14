package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

type robot struct {
	pos vec2
	vel vec2
}

type vec2 struct {
	x int
	y int
}

var input []string
var robots []robot
var GRID_HEIGHT, GRID_WIDTH int = 103, 101

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

func Parse() { //edit return type as needed
	format := func(r rune) rune {
		if (r >= '0' && r <= '9') || r == ' ' || r == ',' || r == '-' {
			return r
		}
		return -1
	}
	for _, r := range input {
		s := strings.Map(format, r)
		initialPos, velocity, _ := strings.Cut(s, " ")
		strInitX, strInitY, _ := strings.Cut(initialPos, ",")
		strVx, strVy, _ := strings.Cut(velocity, ",")
		initX, err := strconv.Atoi(strInitX)
		if err != nil {
			log.Fatal(err)
		}
		initY, err := strconv.Atoi(strInitY)
		if err != nil {
			log.Fatal(err)
		}
		vx, err := strconv.Atoi(strVx)
		if err != nil {
			log.Fatal(err)
		}
		vy, err := strconv.Atoi(strVy)
		if err != nil {
			log.Fatal(err)
		}

		r := robot{
			vec2{
				initX,
				initY,
			},
			vec2{
				vx,
				vy,
			},
		}

		robots = append(robots, r)
	}
}

func Solve() (int, time.Duration) {
	start := time.Now()
	ans := 0
	seconds := 100
	for range seconds {
		simulateSecond()
	}
	// fix edge cases
	for i, r := range robots {
		if r.pos.x == GRID_WIDTH {
			r.pos.x = 0
		}
		if r.pos.y == GRID_HEIGHT {
			r.pos.y = 0
		}
		robots[i] = r
	}
	ans = GetSafetyRatingProduct()
	return ans, time.Since(start)
}

func simulateSecond() {
	for i := range robots {
		stepRobot(i)
	}
}

func stepRobot(index int) {
	r := robots[index]
	x, y := r.pos.x+r.vel.x, r.pos.y+r.vel.y
	if x < 0 {
		x += GRID_WIDTH
	} else if x > GRID_WIDTH {
		x -= GRID_WIDTH
	}

	if y < 0 {
		y += GRID_HEIGHT
	} else if y > GRID_HEIGHT {
		y -= GRID_HEIGHT
	}
	robots[index].pos.x = x
	robots[index].pos.y = y
	// fmt.Println(robots[index])
}

func GetSafetyRatingProduct() int {
	robotMap := make(map[vec2]int)
	for _, r := range robots {
		robotMap[r.pos]++
	}
	Q1, Q2, Q3, Q4 := 0, 0, 0, 0

	for k, n := range robotMap {
		pos := k
		if pos.y < GRID_HEIGHT/2 && pos.x < GRID_WIDTH/2 {
			Q1 += n
		} else if pos.y < GRID_HEIGHT/2 && pos.x > GRID_WIDTH/2 {
			Q2 += n
		} else if pos.y > GRID_HEIGHT/2 && pos.x < GRID_WIDTH/2 {
			Q3 += n
		} else if pos.y > GRID_HEIGHT/2 && pos.x > GRID_WIDTH/2 {
			Q4 += n
		}
	}
	return Q1 * Q2 * Q3 * Q4
}
