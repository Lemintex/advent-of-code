package main


import (
	"fmt"
	"os"
	"strings"
	"strconv"
)

type vec3 struct {
	x, y, z int
}

type brick struct {
	start, end vec3
}

var bricks brick
var brickMap []string
func main() {
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("err`=")
	}
	brickMap = strings.Split(string(f), "\n")
	for _, l := range brickMap {
		ends := strings.Split(l, "~")
		fmt.Printf("\nStart: %v, End: %v", ends[0], ends[1])
		b1, b2 := strings.Split(ends[0], ","), strings.Split(ends[1], ",")
		v1X, _ := strconv.Atoi(b1[0])
		v1Y, _ := strconv.Atoi(b1[1])
		v1Z, _ := strconv.Atoi(b1[2])

		v2X, _ := strconv.Atoi(b2[0])
		v2Y, _ := strconv.Atoi(b2[1])
		v2Z, _ := strconv.Atoi(b2[2])

		v1 := vec3 {
			x: v1X,
			y: v1Y,
			z: v1Z,
		}
		v2 := vec3 {
			x: v2X,
			y: v2Y,
			z: v2Z,
		}
		brick := brick {
			start: v1,
			end: v2,
		}
		brick.PrintBrick()
	}
}

func (b* brick) PrintBrick() {
	fmt.Printf("\nStart = x: %d, y: %d, z: %d | ", b.start.x, b.start.y, b.start.z)
	fmt.Printf("End = x: %d, y: %d, z: %d | ", b.end.x, b.end.y, b.end.z)
}
