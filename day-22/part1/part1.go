package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
)

type vec3 struct {
	x, y, z int
}

type brick struct {
	ID                    rune
	bottom, top           vec3
	length, width, height int
	bricksHoldingMeUp     *int
}

type mapInfo struct {
	height, brickID int
}

var minX, minY, maxX, maxY, lenX, lenY int
var bricks []*brick
var brickMap []string
var mapHeight [][]mapInfo

func main() {
	minX, minY = 1000000, 1000000
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic("err`=")
	}
	brickMap = strings.Split(string(f), "\n")
	for i, l := range brickMap {
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

		v1 := vec3{
			x: v1X,
			y: v1Y,
			z: v1Z,
		}

		v2 := vec3{
			x: v2X,
			y: v2Y,
			z: v2Z,
		}
		brick := brick{
			ID:                rune('A' + i),
			bottom:            v1,
			top:               v2,
			length:            v2X - v1X + 1,
			width:             v2Y - v1Y + 1,
			height:            v2Z - v1Z + 1,
			bricksHoldingMeUp: new(int),
		}
		brick.PrintBrick()
		bricks = append(bricks, &brick)
	}
	lenX = maxX - minX + 1
	lenY = maxY - minY + 1
	// fmt.Println(maxX, minX, lenX)
	// fmt.Println(maxY, minY, lenY)
	slices.SortFunc(bricks, func(a, b *brick) int {
		return a.bottom.z - b.bottom.z
	})
	CreateStackOnGround()
	GetNumberOfBricksThatCanBeDisintegrated()
}

func (b *brick) PrintBrick() {
	// fmt.Printf("\nStart = x: %d, y: %d, z: %d, Area: %d x %d | ", b.start.x, b.start.y, b.start.z, b.start.length, b.start.width)
	// fmt.Printf("End = x: %d, y: %d, z: %d, Area: %d x %d", b.end.x, b.end.y, b.end.z, b.end.length, b.end.width)
}

func CreateStackOnGround() {
	for i, b := range bricks {
		for j, u := range bicks[:i-1] {

		}
	}

	for _, b := range bricks {
		fmt.Println(b)
	}
}

func DoBricksIntersect(falling, stationary *brick) bool {
	//TODO:implement
	return max(falling.bottom.x, stationary.bottom.x) <= min(falling.top.x, stationary.top.x) && max(falling.bottom.y, stationary.bottom.y) <= min(falling.top.y, stationary.top.y)
}

func GetNumberOfBricksThatCanBeDisintegrated() {

	total := 0
	for _, b := range bricks {
		if b.CanDisintegrate() {
			fmt.Println("Disintegrate!", b.ID)
			total++
		}
	}
	fmt.Println("Total:", total)
}
func (b *brick) CanDisintegrate() bool {
	//TODO: implement
	return *b.bricksHoldingMeUp > 1
}
