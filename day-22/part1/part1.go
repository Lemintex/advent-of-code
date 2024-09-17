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

type plane struct {
	v vec3
	length, width int
} 

type brick struct {
	top, bottom plane
	supporting []*brick
	relying []*brick
}

var minX, minY, maxX, maxY, lenX, lenY int
var bricks brick
var brickMap []string
func main() {
	minX, minY = 1000000, 1000000
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("err`=")
	}
	for _, l := range brickMap {
		ends := strings.Split(l, "~")
		// TODO: find proper fix
		if len(ends) == 1 { // temporary
			break
		}
		fmt.Println(ends)
		b1, b2 := strings.Split(ends[0], ","), strings.Split(ends[1], ",")
		v1X, _ := strconv.Atoi(b1[0])
		v1Y, _ := strconv.Atoi(b1[1])
		v1Z, _ := strconv.Atoi(b1[2])

		v2X, _ := strconv.Atoi(b2[0])
		v2Y, _ := strconv.Atoi(b2[1])
		v2Z, _ := strconv.Atoi(b2[2])

		minX = min(minX, v1X)
		minY = min(minY, v1Y)
		maxX = max(maxX, v2X)
		maxY = max(maxY, v2Y)
		v1 := vec3 {
			x: v1X,
			y: v1Y,
			z: v1Z,
		}
		p1 := plane {
			v: v1,
			length: v2X - v1X + 1,
			width: v2Y - v1Y + 1,
		}
		v2 := vec3 {
			x: v1X,
			y: v1Y,
			z: v2Z,
		}
		p2 := plane {
			v: v2,
			length: v2X - v1X + 1,
			width: v2Y - v1Y + 1,
		}
		brick := brick {
			bottom: p2,
			top: p1,
			supporting: []*brick{},
			relying: []*brick{},
		}
		brick.PrintBrick()
	}
	lenX = maxX - minX + 1
	lenY = maxY - minY + 1
	fmt.Println(maxX, minX, lenX)
	fmt.Println(maxY, minY, lenY)
}

func (b* brick) PrintBrick() {
	// fmt.Printf("\nStart = x: %d, y: %d, z: %d, Area: %d x %d | ", b.start.x, b.start.y, b.start.z, b.start.length, b.start.width)
	// fmt.Printf("End = x: %d, y: %d, z: %d, Area: %d x %d", b.end.x, b.end.y, b.end.z, b.end.length, b.end.width)
}

//ooooo
//o    <=
