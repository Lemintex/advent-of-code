package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

var MIN, MAX float32

type Vec3 struct {
	X, Y, Z int64
}

type Hailstone struct {
	pos, v  Vec3
	a, b, c float32
}

var hailstones []Hailstone

func main() {
	MIN = 200000000000000
	MAX = 400000000000000
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
		X: int64(x),
		Y: int64(y),
		Z: int64(z),
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
		X: int64(vx),
		Y: int64(vy),
		Z: int64(vz),
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
	hOrigin := hailstones[0]
	h1, h2 := hailstones[1], hailstones[1]

	//get the hailstones 1 and 2 relatie to hailstone 0
	h1 = h1.relativeTo(hOrigin)
	h2 = h2.relativeTo(hOrigin)

	// viewed from h0, the collisions with the rock can be observed as:
	// c1 = h1.p + t1 * h1.v
	// c2 = h2.p + t2 * h2.v

	// the rock must collide at some t0 with hOrigin, which we'll call the origin still

	// origin > c1 > c2 must be a straight line, so collision vectors c1 and c2 must be colinear: meaning
	//	(h1.pos + t1 * h1.v) X (h2.pos + t2 * h2.v) == 0
	p1, p2 := hailstones[1].pos, hailstones[2].pos
	fmt.Println(p1, p2)
	v1, v2 := hailstones[1].v, hailstones[2].v
	fmt.Println(v1, v2)
	fmt.Println(p1, p2, p1.CrossProduct(p2))
	t1 := -((p1.CrossProduct(p2)).Mul(v2)) / (v1.CrossProduct(p2).Mul(v2))
	t2 := -((p1.CrossProduct(p2)).Mul(v1)) / (p1.CrossProduct(v2).Mul(v1))
	fmt.Println("time")
	fmt.Println(t1, t2)
	
	c1 := p1.Add(v1.MulFloat(t1))
	c2 := p2.Add(v2.MulFloat(t2))
	fmt.Println("col")
	fmt.Println(c1, c2)

	v := (c2.Sub(c1)).DivFloat(t2 - t1)
	pos := c1.Sub(v.MulFloat(t1))
	fmt.Print(pos.X + pos.Y + pos.Z)

}

func (h Hailstone) relativeTo(origin Hailstone) Hailstone {

	pos := Vec3{
		X: h.pos.X - origin.pos.X,
		Y: h.pos.Y - origin.pos.Y,
		Z: h.pos.Z - origin.pos.Z,
	}
	v := Vec3{
		X: h.v.X - origin.v.X,
		Y: h.v.Y - origin.v.Y,
		Z: h.v.Z - origin.v.Z,
	}
	return Hailstone {
		pos: pos,
		v: v,
	}
}

func (v Vec3) Add(v2 Vec3) Vec3 {
	return Vec3 {
		X: v.X + v2.X,
		Y: v.Y + v2.Y,
		Z: v.Z + v2.Z,
	}
}
func (v Vec3) Sub(v2 Vec3) Vec3 {
	return Vec3 {
		X: v.X - v2.X,
		Y: v.Y - v2.Y,
		Z: v.Z - v2.Z,
	}
}
func (v Vec3) Mul(v2 Vec3) int64 {
	return v.X * v2.X + v.Y * v2.Y + v.Z * v2.Z
}

func (v Vec3) Div (v2 Vec3) Vec3 {
	return Vec3 {
		X: v.X / v2.X,
		Y: v.Y / v2.Y,
		Z: v.Z / v2.Z,
	}
}
func (v Vec3) DivFloat (f int64) Vec3 {
	return Vec3 {
		X: int64(v.X / f),
		Y: int64(v.Y / f),
		Z: int64(v.Z / f),
	}
}
func (v Vec3) MulFloat(f int64) Vec3 {
	return Vec3{
		X: int64(v.X * f),
		Y: int64(v.Y * f),
		Z: int64(v.Z * f),
	}
}

func (v Vec3) Neg() Vec3 {
	return Vec3 {
		X: -v.X,
		Y: -v.Y,
		Z: -v.Z,
	}
}
func (v Vec3) CrossProduct(other Vec3) Vec3 {
	return Vec3 {
		X: v.Y*other.Z - v.Z*other.Y,
		Y: v.Z*other.X - v.X*other.Z,
		Z: v.X*other.Y - v.Y*other.X,
	}
}
// 1491912053751808 too high
// 869626401225989 too low
