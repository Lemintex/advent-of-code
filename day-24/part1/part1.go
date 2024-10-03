package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

var MIN, MAX float32

type Vec3 struct {
	X, Y, Z int
}

type Hailstone struct {
	pos, v  Vec3
	a, b, c float32
}

var hailstones []Hailstone

func main() {
	MIN =  200000000000000
	MAX =  400000000000000
	f, err := os.ReadFile("../input.txt")
	if err != nil {
		panic(err)
	}
	str := strings.Split(strings.TrimSpace(string(f)), "\n")
	for _, s := range str {
		parseHailstone(s)
	}
	CalculateIntersections()
}

func parseHailstone(s string) {
	parts := strings.Split(strings.TrimSpace(s), " @ ")
	// fmt.Println(parts[0], "|", parts[1])
	position := strings.Split(parts[0], ", ")
	x, errx := strconv.Atoi(position[0])
	if errx != nil {
		panic(errx)
	}
	y, erry := strconv.Atoi(position[1])
	if erry != nil {
		panic(erry)
	}
	z, errz := strconv.Atoi(position[2])
	if errz != nil {
		panic(errz)
	}

	pos := Vec3{
		X: x,
		Y: y,
		Z: z,
	}
	velocity := strings.Split(parts[1], ", ")
	vx, errvx := strconv.Atoi(velocity[0])
	if errvx != nil {
		panic(errvx)
	}
	vy, errvy := strconv.Atoi(velocity[1])
	if errvy != nil {
		panic(errvy)
	}
	vz, errvz := strconv.Atoi(velocity[2])
	if errvz != nil {
		panic(errvz)
	}
	vel := Vec3{
		X: vx,
		Y: vy,
		Z: vz,
	}
	a, b, c := float32(vy), float32(-vx), float32(vy*x-vx*y)
	stone := Hailstone{
		pos: pos,
		v:   vel,
		a:   a,
		b:   b,
		c:   c,
	}
	hailstones = append(hailstones, stone)
}

func CalculateIntersections() {
	total := 0
	for i, h1 := range hailstones {
		for _, h2 := range hailstones[i+1:] {
			if h1.Intersection(&h2) {
				total++
			}
		}
	}
	fmt.Println(total)
}

func (h *Hailstone) Intersection(other *Hailstone) bool {
	// if lines are parallel
	if h.a*other.b == other.a*h.b {
		return false
	}

	// get intersection x,y
	x, y := (h.c*other.b-other.c*h.b)/(h.a*other.b-other.a*h.b), (other.c*h.a-h.c*other.a)/(h.a*other.b-other.a*h.b)

	// if outwith bounding box
	if x < MIN || x > MAX || y < MIN || y > MAX {
		return false
	}
	//return if intersection point is after the starting point of both lines
	return (x-float32(h.pos.X))*float32(h.v.X) >= 0 && (y-float32(h.pos.Y))*float32(h.v.Y) >= 0 && (x-float32(other.pos.X))*float32(other.v.X) >= 0 && (y-float32(other.pos.Y))*float32(other.v.Y) >= 0

}
//11631 too low
//15982 too high
