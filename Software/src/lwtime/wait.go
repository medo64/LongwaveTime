package main

import (
	"time"
)

func waitForNextSecond() {
	var initialSecond int = getReferenceTime().Second()
	for { // spin-wait until second is 0
		var currentSecond int = getReferenceTime().Second()
		if initialSecond != currentSecond {
			return
		}
	}
}

func waitForAlmostNextMinute() {
	var initialSecond int = getReferenceTime().Second()

	for { // sleep until the second 59 of the minute
		var currentSecond int = getReferenceTime().Second()
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

func getReferenceTime() time.Time {
	return time.Now().Add(+860 * time.Millisecond)
}
