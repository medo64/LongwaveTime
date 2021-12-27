package main

import "time"

func getAsWWWB(time time.Time) string { // https://en.wikipedia.org/wiki/WWVB
	var out string = ""

	out += "M" // Frame reference marker

	// Minutes
	var minute = time.Minute()
	if minute >= 40 {
		minute -= 40
		out += "1"
	} else {
		out += "0"
	}
	if minute >= 20 {
		minute -= 20
		out += "1"
	} else {
		out += "0"
	}
	if minute >= 10 {
		minute -= 10
		out += "1"
	} else {
		out += "0"
	}
	out += "0"
	if minute >= 8 {
		minute -= 8
		out += "1"
	} else {
		out += "0"
	}
	if minute >= 4 {
		minute -= 4
		out += "1"
	} else {
		out += "0"
	}
	if minute >= 2 {
		minute -= 2
		out += "1"
	} else {
		out += "0"
	}
	if minute >= 1 {
		minute -= 1
		out += "1"
	} else {
		out += "0"
	}

	out += "M" // Marker

	out += "0" // Unused
	out += "0" // Unused

	// Hours
	var hour = time.Hour()
	if hour >= 20 {
		hour -= 20
		out += "1"
	} else {
		out += "0"
	}
	if hour >= 10 {
		hour -= 10
		out += "1"
	} else {
		out += "0"
	}
	out += "0"
	if hour >= 8 {
		hour -= 8
		out += "1"
	} else {
		out += "0"
	}
	if hour >= 4 {
		hour -= 4
		out += "1"
	} else {
		out += "0"
	}
	if hour >= 2 {
		hour -= 2
		out += "1"
	} else {
		out += "0"
	}
	if hour >= 1 {
		hour -= 1
		out += "1"
	} else {
		out += "0"
	}

	out += "M" // Marker

	out += "0" // Unused
	out += "0" // Unused

	// Day of year
	var day = time.YearDay()
	if day >= 200 {
		day -= 200
		out += "1"
	} else {
		out += "0"
	}
	if day >= 100 {
		day -= 100
		out += "1"
	} else {
		out += "0"
	}
	out += "0"
	if day >= 80 {
		day -= 80
		out += "1"
	} else {
		out += "0"
	}
	if day >= 40 {
		day -= 40
		out += "1"
	} else {
		out += "0"
	}
	if day >= 20 {
		day -= 20
		out += "1"
	} else {
		out += "0"
	}
	if day >= 10 {
		day -= 10
		out += "1"
	} else {
		out += "0"
	}
	out += "M" // Marker
	if day >= 8 {
		day -= 8
		out += "1"
	} else {
		out += "0"
	}
	if day >= 4 {
		day -= 4
		out += "1"
	} else {
		out += "0"
	}
	if day >= 2 {
		day -= 2
		out += "1"
	} else {
		out += "0"
	}
	if day >= 1 {
		day -= 1
		out += "1"
	} else {
		out += "0"
	}

	out += "0" // Unused
	out += "0" // Unused

	// DUT1 sign (TODO)
	out += "0"
	out += "0"
	out += "0"

	out += "M" // Marker

	// DUT1 value (TODO)
	out += "0"
	out += "0"
	out += "0"
	out += "0"

	out += "0" // Unused

	// Year
	var year = time.Year() % 100
	if year >= 80 {
		year -= 80
		out += "1"
	} else {
		out += "0"
	}
	if year >= 40 {
		year -= 40
		out += "1"
	} else {
		out += "0"
	}
	if year >= 20 {
		year -= 20
		out += "1"
	} else {
		out += "0"
	}
	if year >= 10 {
		year -= 10
		out += "1"
	} else {
		out += "0"
	}
	out += "M" // Marker
	if year >= 8 {
		year -= 8
		out += "1"
	} else {
		out += "0"
	}
	if year >= 4 {
		year -= 4
		out += "1"
	} else {
		out += "0"
	}
	if year >= 2 {
		year -= 2
		out += "1"
	} else {
		out += "0"
	}
	if year >= 1 {
		year -= 1
		out += "1"
	} else {
		out += "0"
	}

	out += "0" // Unused

	// Leap year indicator
	if year%4 == 0 && year%100 != 0 || year%400 == 0 {
		out += "1"
	} else {
		out += "0"
	}

	// 	Leap second at end of month (TODO)
	out += "0"

	// DST status value
	if time.Local().IsDST() {
		if time.Local().AddDate(0, 0, 1).IsDST() {
			out += "11" // DST in effect.
		} else {
			out += "01" // DST end today
		}
	} else {
		if time.Local().AddDate(0, 0, 1).IsDST() {
			out += "10" // DST begins today
		} else {
			out += "00" // DST not in effect
		}
	}

	out += "M" // Marker

	return out
}
