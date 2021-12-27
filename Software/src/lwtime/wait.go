package main

import (
	"time"
)

func waitForNextSecond() {
	var initialSecond int = time.Now().Second()
	for { // spin-wait until second is 0
		var currentSecond int = time.Now().Second()
		if initialSecond != currentSecond {
			return
		}
	}
}

func waitForAlmostNextMinute() {
	var initialSecond int = time.Now().Second()

	for { // sleep until the second 59 of the minute
		var currentSecond int = time.Now().Second()
		if currentSecond == 59 {
			return
		}
		if currentSecond < initialSecond {
			return
		}
		if currentSecond < 59 {
			time.Sleep(1 * time.Second)
		} // wait for one more second
	}
}
